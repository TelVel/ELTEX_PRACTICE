#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0)
    {
        perror("socket: ");
        exit(EXIT_FAILURE);
    }
    int status;
    struct sockaddr_in server_addr, client_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9001);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(9002);
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    status = bind(udp_socket, (struct sockaddr*)&client_addr, sizeof(client_addr));
    if (status < 0)
    {
        perror("bind: ");
        exit(EXIT_FAILURE);
    }
    char buf[1024];
    ssize_t size = 0;

    while(1){
        printf("OUT_MSG->");
        fgets(buf, 1024, stdin);
        buf[strcspn(buf, "\n")] = 0;
        size = sendto(udp_socket, buf, strlen(buf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (size == -1)
        {
            perror("sendto: ");
            exit(EXIT_FAILURE);
        }
        if(strcmp(buf, "exit") == 0) break;
        size = recvfrom(udp_socket, buf, sizeof(buf), 0, NULL, NULL);
        buf[size] = 0;
        if(strcmp(buf, "exit") == 0) break;
        printf("IN_MSG->%s\n", buf);
    }
}