#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_EVENTS 50
#define BUFFER_SIZE 1024

void error(const char *msg);
void printusers();
void receive_file(int sock, const char *filename);
void handle_client(int sock);

int* nclients;

float sum(float a, float b) { return a + b; }
float sub(float a, float b) { return a - b; }
float mult(float a, float b) { return a * b; }
float divide(float a, float b) { 
    if (b != 0) return a / b; 
    else return 0; 
}

void printusers() { 
    if(nclients) {
        printf("%d user(s) on-line\n", *nclients);
    } else {
        printf("No users online\n");
    }
}

void set_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void receive_file(int sock, const char *filename) {
    FILE *fp;
    long file_size;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    size_t total_bytes = 0;

    if (read(sock, &file_size, sizeof(file_size)) < 0) {
        error("ERROR receiving file size");
    }
    file_size = ntohl(file_size);

    printf("Receiving file: %s (Size: %ld bytes)\n", filename, file_size);
    char filename_cool[256];
    snprintf(filename_cool, sizeof(filename_cool), "servdir/%s", filename);
    fp = fopen(filename_cool, "wb");
    if (fp == NULL) {
        error("ERROR creating file");
    }

    while (total_bytes < file_size) {
        bytes_received = read(sock, buffer, sizeof(buffer));
        if (bytes_received < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
            error("ERROR receiving file");
        }
        if (bytes_received == 0) break;
        fwrite(buffer, 1, bytes_received, fp);
        total_bytes += bytes_received;
    }
    fclose(fp);
    printf("File received successfully\n");
}

void handle_client(int sock) {
    char buff[BUFFER_SIZE];
    int bytes_recv;
    float a, b, result;
    char operation[10];
    char filename[256];

    bytes_recv = read(sock, buff, sizeof(buff)-1);
    if (bytes_recv <= 0) {
        if (bytes_recv == 0) printf("Client disconnected\n");
        else perror("read");
        close(sock);
        (*nclients)--;
        printusers();
        return;
    }
    buff[bytes_recv] = '\0';
    
    if (strcmp(buff, "1\n") == 0) {
        write(sock, "S: Enter operation (+, -, *, /): ", 33);
        bytes_recv = read(sock, buff, sizeof(buff)-1);
        if (bytes_recv <= 0) goto client_error;
        buff[bytes_recv] = '\0';
        strncpy(operation, buff, sizeof(operation));
        
        write(sock, "S: Enter first number: ", 23);
        bytes_recv = read(sock, buff, sizeof(buff)-1);
        if (bytes_recv <= 0) goto client_error;
        buff[bytes_recv] = '\0';
        a = atof(buff);
        
        write(sock, "S: Enter second number: ", 24);
        bytes_recv = read(sock, buff, sizeof(buff)-1);
        if (bytes_recv <= 0) goto client_error;
        buff[bytes_recv] = '\0';
        b = atof(buff);
        
        switch(operation[0]) {
            case '+': result = sum(a, b); break;
            case '-': result = sub(a, b); break;
            case '*': result = mult(a, b); break;
            case '/': result = divide(a, b); break;
            default: 
                write(sock, "S: Invalid operation\n", 21);
                goto client_error;
        }
        
        snprintf(buff, sizeof(buff), "S: Result: %.2f\n", result);
        write(sock, buff, strlen(buff));
    }
    else if (strcmp(buff, "2\n") == 0) {
        write(sock, "S: Enter filename to save: ", 27);
        bytes_recv = read(sock, filename, sizeof(filename)-1);
        if (bytes_recv <= 0) goto client_error;
        filename[bytes_recv] = '\0';
        filename[strcspn(filename, "\n")] = 0;
        
        write(sock, "S: Ready to receive file\n", 25);
        receive_file(sock, filename);
    }
    else {
        write(sock, "S: Invalid mode selected\n", 25);
    }
    
    write(sock, "S: Choose mode:\n1. Math operations\n2. File transfer\n", 53);
    return;

client_error:
    close(sock);
    (*nclients)--;
    printusers();
}

int main(int argc, char *argv[]) {
    printf("TCP SERVER DEMO (epoll, single-threaded)\n");

    int sockfd, newsockfd, portno, ep_fd, n_ready;
    struct epoll_event ev, events[MAX_EVENTS];
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    // Shared memory setup
    int shm_id = shm_open("/shm_nclients", O_CREAT | O_RDWR, 0666);
    if (shm_id == -1) error("shm_open failed");
    if (ftruncate(shm_id, sizeof(int)) == -1) error("ftruncate failed");
    nclients = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
    if (nclients == MAP_FAILED) error("mmap failed");
    *nclients = 0;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (sockfd < 0) error("ERROR opening socket");

    // Bind socket
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, SOMAXCONN);

    // Create epoll instance
    ep_fd = epoll_create1(0);
    if (ep_fd == -1) error("epoll_create1 failed");

    // Add server socket to epoll
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sockfd;
    if (epoll_ctl(ep_fd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
        error("epoll_ctl failed");

    printf("Server started on port %d\n", portno);

    while (1) {
        n_ready = epoll_wait(ep_fd, events, MAX_EVENTS, -1);
        if (n_ready == -1) {
            perror("epoll_wait");
            continue;
        }

        for (int i = 0; i < n_ready; i++) {
            if (events[i].data.fd == sockfd) {
                // New connection
                clilen = sizeof(cli_addr);
                newsockfd = accept4(sockfd, (struct sockaddr *) &cli_addr, &clilen, SOCK_NONBLOCK);
                if (newsockfd == -1) {
                    perror("accept4");
                    continue;
                }

                printf("New connection\n");
                (*nclients)++;
                printusers();

                // Add new client to epoll
                ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                ev.data.fd = newsockfd;
                if (epoll_ctl(ep_fd, EPOLL_CTL_ADD, newsockfd, &ev) == -1) {
                    perror("epoll_ctl");
                    close(newsockfd);
                    continue;
                }

                // Send initial menu
                write(newsockfd, "S: Choose mode:\n1. Math operations\n2. File transfer\n", 53);
            } 
            else {
                // Client activity
                if (events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
                    // Client disconnected
                    printf("Client disconnected\n");
                    close(events[i].data.fd);
                    (*nclients)--;
                    printusers();
                } 
                else if (events[i].events & EPOLLIN) {
                    // Handle client request
                    handle_client(events[i].data.fd);
                }
            }
        }
    }

    close(ep_fd);
    close(sockfd);
    munmap(nclients, sizeof(int));
    close(shm_id);
    shm_unlink("/shm_nclients");
    return 0;
}