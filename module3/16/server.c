#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

void dostuff(int);
void error(const char *msg);
void printusers();
void receive_file(int sock, const char *filename);

int* nclients;

// Math operations
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

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void receive_file(int sock, const char *filename) {
    FILE *fp;
    long file_size;
    char buffer[1024];
    ssize_t bytes_received;
    size_t total_bytes = 0;

    // Receive file size first
    if (read(sock, &file_size, sizeof(file_size)) < 0) {
        error("ERROR receiving file size");
    }
    file_size = ntohl(file_size);

    printf("Receiving file: %s (Size: %ld bytes)\n", filename, file_size);
    char* s1 = "servdir/";
    char* filename_cool = malloc(strlen(s1)+strlen(filename) + 1);
    strcpy(filename_cool, s1);
    strcat(filename_cool, filename);
    fp = fopen(filename_cool, "wb");
    if (fp == NULL) {
        error("ERROR creating file");
    }

    while (total_bytes < file_size) {
        bytes_received = read(sock, buffer, sizeof(buffer));
        if (bytes_received < 0) {
            error("ERROR receiving file");
        }
        fwrite(buffer, 1, bytes_received, fp);
        total_bytes += bytes_received;
    }
    free(filename_cool);
    fclose(fp);
    printf("File received successfully\n");
}

int main(int argc, char *argv[]) {
    printf("TCP SERVER DEMO (with file transfer)\n");
    
    int sockfd, newsockfd, portno, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    
    // Initialize shared memory
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
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
     
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) error("ERROR on accept");
        
        (*nclients)++;
        printusers();
        
        pid = fork();
        if (pid < 0) error("ERROR on fork");
        
        if (pid == 0) {
            close(sockfd);
            dostuff(newsockfd);
            (*nclients)--;
            printusers();
            close(newsockfd);
            exit(0);
        } else {
            close(newsockfd);
        }
    }
    
    munmap(nclients, sizeof(int));
    close(shm_id);
    shm_unlink("/shm_nclients");
    close(sockfd);
    return 0;
}

void dostuff(int sock) {
    int bytes_recv;
    float a, b, result;
    char buff[1024];
    char operation[10];
    char filename[256];
    
    write(sock, "S: Choose mode:\n1. Math operations\n2. File transfer\n", 53);
    
    bytes_recv = read(sock, buff, sizeof(buff)-1);
    if (bytes_recv < 0) error("ERROR reading from socket");
    buff[bytes_recv] = '\0';
    
    if (strcmp(buff, "1\n") == 0) {

        write(sock, "S: Enter operation (+, -, *, /): ", 33);
        bytes_recv = read(sock, buff, sizeof(buff)-1);
        if (bytes_recv < 0) error("ERROR reading from socket");
        buff[bytes_recv] = '\0';
        strncpy(operation, buff, sizeof(operation));
        
        write(sock, "S: Enter first number: ", 23);
        bytes_recv = read(sock, buff, sizeof(buff)-1);
        if (bytes_recv < 0) error("ERROR reading from socket");
        buff[bytes_recv] = '\0';
        a = atof(buff);
        
        write(sock, "S: Enter second number: ", 24);
        bytes_recv = read(sock, buff, sizeof(buff)-1);
        if (bytes_recv < 0) error("ERROR reading from socket");
        buff[bytes_recv] = '\0';
        b = atof(buff);
        
        switch(operation[0]) {
            case '+': result = sum(a, b); break;
            case '-': result = sub(a, b); break;
            case '*': result = mult(a, b); break;
            case '/': result = divide(a, b); break;
            default: 
                write(sock, "S: Invalid operation\n", 21);
                close(sock);
                return;
        }
        
        snprintf(buff, sizeof(buff), "S: Result: %.2f\n", result);
        write(sock, buff, strlen(buff));
    }
    else if (strcmp(buff, "2\n") == 0) {

        write(sock, "S: Enter filename to save: ", 27);
        bytes_recv = read(sock, filename, sizeof(filename)-1);
        if (bytes_recv < 0) error("ERROR reading filename");
        filename[bytes_recv] = '\0';
        
        filename[strcspn(filename, "\n")] = 0;
        
        write(sock, "S: Ready to receive file\n", 25);
        receive_file(sock, filename);
    }
    else {
        write(sock, "S: Invalid mode selected\n", 25);
    }
    
    close(sock);
}