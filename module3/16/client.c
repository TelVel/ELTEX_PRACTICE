#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/stat.h>

void error(const char *msg);
void send_file(int sock, const char *filename);

void error(const char *msg) {
    perror(msg);
    exit(0);
}

void send_file(int sock, const char *filename) {
    FILE *fp;
    char buffer[1024];
    size_t bytes_read;
    long file_size;
    struct stat st;

    if (stat(filename, &st) != 0) {
        perror("ERROR getting file info");
        return;
    }
    file_size = st.st_size;

    // Send file size first
    long net_file_size = htonl(file_size);
    if (write(sock, &net_file_size, sizeof(net_file_size)) < 0) {
        error("ERROR sending file size");
    }

    printf("Sending file: %s (Size: %ld bytes)\n", filename, file_size);

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        error("ERROR opening file");
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        if (write(sock, buffer, bytes_read) < 0) {
            error("ERROR sending file");
        }
    }

    fclose(fp);
    printf("File sent successfully\n");
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    char filename[256];
    
    printf("TCP CLIENT DEMO (with file transfer)\n");
    
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) error("ERROR reading from socket");
    printf("%s", buffer);
    
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) error("ERROR writing to socket");
    
    if (strcmp(buffer, "1\n") == 0) {
 
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) error("ERROR reading from socket");
        printf("%s", buffer);
        
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) error("ERROR writing to socket");
        
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) error("ERROR reading from socket");
        printf("%s", buffer);
        
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) error("ERROR writing to socket");
        
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) error("ERROR reading from socket");
        printf("%s", buffer);
        
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) error("ERROR writing to socket");
        
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) error("ERROR reading from socket");
        printf("%s", buffer);
    }
    else if (strcmp(buffer, "2\n") == 0) {

        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) error("ERROR reading from socket");
        printf("%s", buffer);
        
        bzero(filename, 256);
        fgets(filename, 255, stdin);
        filename[strcspn(filename, "\n")] = 0;
        
        n = write(sockfd, filename, strlen(filename));
        if (n < 0) error("ERROR writing to socket");
        
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) error("ERROR reading from socket");
        printf("%s", buffer);
        
        send_file(sockfd, filename);
    }
    else {
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) error("ERROR reading from socket");
        printf("%s", buffer);
    }
    
    close(sockfd);
    return 0;
}