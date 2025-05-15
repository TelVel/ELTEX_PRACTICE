#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define SERVDIR "servdir"
#define INVALID_FD -1

typedef struct {
    int state;
    int file_fd;
    char filename[256];
    long bytes_received;
    char operation;
    float num1;
} client_state;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void set_nonblocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) error("fcntl F_GETFL");
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) error("fcntl F_SETFL");
}

void send_prompt(int sock, const char *prompt) {
    if (sock <= 0) return;
    if (write(sock, prompt, strlen(prompt)) < 0) {
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
            perror("write prompt");
        }
    }
}

void ensure_servdir_exists() {
    struct stat st = {0};
    if (stat(SERVDIR, &st) == -1 && mkdir(SERVDIR, 0700) == -1) {
        error("Failed to create servdir");
    }
}

void reset_client_state(client_state *state) {
    if (state->file_fd != INVALID_FD) {
        close(state->file_fd);
        state->file_fd = INVALID_FD;
    }
    memset(state, 0, sizeof(client_state));
    state->file_fd = INVALID_FD;
}

int main(int argc, char *argv[]) {
    ensure_servdir_exists();
    int sockfd, portno, first = 0;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    struct pollfd fds[MAX_CLIENTS + 1];
    client_state states[MAX_CLIENTS + 1];
    int nfds = 1;
    int timeout = 1000;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(1);
    }
    for (int i = 0; i <= MAX_CLIENTS; i++) {
        fds[i].fd = INVALID_FD;
        fds[i].events = 0;
        reset_client_state(&states[i]);
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    if (sockfd == 0) {
        int newfd = dup(sockfd);
        close(sockfd);
        sockfd = newfd;
        if (sockfd == 0) error("Failed to get non-zero fd");
    }
    
    set_nonblocking(sockfd);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    
    listen(sockfd, 5);
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
    printf("Server listening on port %d\n", portno);
    
    while (1) {
        int ret = poll(fds, nfds, timeout);
        if (ret < 0) error("ERROR in poll");
        
        if (fds[0].revents & POLLIN) {
            clilen = sizeof(cli_addr);
            int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0) {
                if (errno != EWOULDBLOCK && errno != EAGAIN) perror("accept");
                continue;
            }
            if (newsockfd == 0) {
                int newfd = dup(newsockfd);
                close(newsockfd);
                newsockfd = newfd;
                if (newsockfd == 0) {
                    close(newsockfd);
                    continue;
                }
            }
            int slot = -1;
            for (int i = 1; i <= MAX_CLIENTS; i++) {
                if (fds[i].fd == INVALID_FD) {
                    slot = i;
                    break;
                }
            }
            
            if (slot != -1) {
                set_nonblocking(newsockfd);
                fds[slot].fd = newsockfd;
                fds[slot].events = POLLIN | POLLHUP | POLLERR;
                reset_client_state(&states[slot]);
                states[slot].state = 0;
                if (slot >= nfds) nfds = slot + 1;
                printf("New client connected (fd: %d)\n", newsockfd);
                send_prompt(newsockfd, "S: Choose mode:\n1. Math operations\n2. File transfer\n");
            } else {
                send_prompt(newsockfd, "S: Server is busy, try again later\n");
                close(newsockfd);
            }
        }
        
        for (int i = 1; i < nfds; i++) {
            if (fds[i].fd == INVALID_FD) continue;
            
            if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                printf("Client (fd: %d) disconnected\n", fds[i].fd);
                reset_client_state(&states[i]);
                close(fds[i].fd);
                fds[i].fd = INVALID_FD;
                continue;
            }
            
            if (fds[i].revents & POLLIN) {
                char buffer[BUFFER_SIZE];
                ssize_t n = read(fds[i].fd, buffer, sizeof(buffer)-1);
                
                if (n <= 0) {
                    if (n == 0 || errno == ECONNRESET) {
                        if (states[i].state == 5) {printf("\n");}
                        printf("Client (fd: %d) disconnected\n", fds[i].fd);
                        reset_client_state(&states[i]);
                        close(fds[i].fd);
                        fds[i].fd = INVALID_FD;
                    }
                    continue;
                }
                
                buffer[n] = '\0';
                if (states[i].state !=5) {printf("Client %d: %s", fds[i].fd, buffer);}
                
                switch (states[i].state) {
                    case 0:
                        if (strcmp(buffer, "1\n") == 0) {
                            send_prompt(fds[i].fd, "S: Enter operation (+, -, *, /): ");
                            states[i].state = 1;
                        } else if (strcmp(buffer, "2\n") == 0) {
                            send_prompt(fds[i].fd, "S: Enter filename to save: ");
                            states[i].state = 2;
                        } else {
                            send_prompt(fds[i].fd, "S: Invalid choice, try again\n");
                        }
                        break;
                        
                    case 1:
                        if (strchr("+-*/", buffer[0]) && buffer[1] == '\n') {
                            states[i].operation = buffer[0];
                            send_prompt(fds[i].fd, "S: Enter first number: ");
                            states[i].state = 3;
                        } else {
                            send_prompt(fds[i].fd, "S: Invalid operation, try again (+, -, *, /): ");
                        }
                        break;
                        
                        case 2: { 
                            buffer[strcspn(buffer, "\n")] = '\0';
                            char fullpath[1032];
                            snprintf(fullpath, sizeof(fullpath), "%s/%s", "servdir", buffer);
                            states[i].file_fd = open(fullpath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            if (states[i].file_fd == -1) {
                                send_prompt(fds[i].fd, "S: Error opening file for writing\n");
                                states[i].state = 0;
                            } else {
                                strncpy(states[i].filename, fullpath, sizeof(states[i].filename));
                                strcpy(buffer, "");
                                send_prompt(fds[i].fd, "S: Ready to receive file data. Send file now.\n");
                                states[i].state = 5;
                                states[i].bytes_received = 0; 
                             }
                            break;
                        }
                        
                    case 3:
                        states[i].num1 = atof(buffer);
                        send_prompt(fds[i].fd, "S: Enter second number: ");
                        states[i].state = 4;
                        break;
                        
                    case 4: {
                        float num2 = atof(buffer);
                        float result = 0;
                        switch (states[i].operation) {
                            case '+': result = states[i].num1 + num2; break;
                            case '-': result = states[i].num1 - num2; break;
                            case '*': result = states[i].num1 * num2; break;
                            case '/': result = num2 != 0 ? states[i].num1 / num2 : 0; break;
                        }
                        char response[BUFFER_SIZE];
                        snprintf(response, sizeof(response), "S: Result: %.2f\n", result);
                        send_prompt(fds[i].fd, response);
                        break;
                    }
                    
                    case 5: {
                        if (states[i].file_fd != -1) {
                            if (first == 0) {n = 0; first++;};
                            ssize_t written = write(states[i].file_fd, buffer, n);
                            if (written == -1) {
                                send_prompt(fds[i].fd, "S: Error writing to file\n");
                                close(states[i].file_fd);
                                states[i].file_fd = -1;
                                states[i].state = 0;
                            } else {
                                states[i].bytes_received += written;
                            }
                        }
                        break;
                    }
                }
            }
            while (nfds > 1 && fds[nfds-1].fd == INVALID_FD) {nfds--;}
        }
    }
    close(sockfd);
    return 0;
}
