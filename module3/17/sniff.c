#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define BUFFER_SIZE 65536
int log_fd = -1;

void process_packet(unsigned char *buffer, int size) {
    struct iphdr *ip_header = (struct iphdr *)buffer;
    unsigned short iphdrlen = ip_header->ihl * 4;
    
    if (ip_header->protocol == IPPROTO_UDP) {
        struct udphdr *udp_header = (struct udphdr *)(buffer + iphdrlen);
        char src_ip[INET_ADDRSTRLEN];
        char dst_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ip_header->saddr), src_ip, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ip_header->daddr), dst_ip, INET_ADDRSTRLEN);
        
        unsigned short src_port = ntohs(udp_header->source);
        unsigned short dst_port = ntohs(udp_header->dest);
        int udp_len = ntohs(udp_header->len);
        int payload_len = udp_len - sizeof(struct udphdr);
        unsigned char *payload = buffer + iphdrlen + sizeof(struct udphdr);
        char log_msg[1024];
        int log_pos = 0;
        log_pos += snprintf(log_msg + log_pos, sizeof(log_msg) - log_pos, "Пакет от %s:%d -> %s:%d, длина: %d байт\n", src_ip, src_port, dst_ip, dst_port, payload_len);
        log_pos += snprintf(log_msg + log_pos, sizeof(log_msg) - log_pos, "Содержимое: ");
        for (int i = 0; i < (payload_len < 64 ? payload_len : 64); i++) {
            if (payload[i] >= 32 && payload[i] <= 126) {
                log_pos += snprintf(log_msg + log_pos, sizeof(log_msg) - log_pos, "%c", payload[i]);
            } else {
                log_pos += snprintf(log_msg + log_pos, sizeof(log_msg) - log_pos, "[%02X]", payload[i]);
            }
        }
        log_pos += snprintf(log_msg + log_pos, sizeof(log_msg) - log_pos, "\n\n");
        write(log_fd, log_msg, log_pos);
        printf("%s", log_msg);
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Использование: %s <IP сервера> <1 порт> <2 порт>\n", argv[0]);
        return 1;
    } 
    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int client_port = atoi(argv[3]);

    int raw_socket;
    unsigned char buffer[BUFFER_SIZE];
    log_fd = open("dump.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (log_fd == -1) {
        perror("Ошибка открытия файла дампа");
        exit(EXIT_FAILURE);
    }

    raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_socket < 0) {
        perror("Ошибка создания сокета");
        close(log_fd);
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    if (setsockopt(raw_socket, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval)) < 0) {
        perror("Ошибка setsockopt");
        close(raw_socket);
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    printf("Захват UDP-пакетов для %s:%d и %s:%d\n", server_ip, server_port, server_ip, client_port);
    printf("Дамп пакетов сохраняется в chat_dump.bin\n");
    printf("Для выхода нажмите Ctrl+C\n\n");
    
    while (1) {
        int packet_size = recvfrom(raw_socket, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (packet_size < 0) {
            perror("Ошибка при получении пакета");
            close(log_fd);
            break;
        }
        
        struct iphdr *ip_header = (struct iphdr *)buffer;
        struct udphdr *udp_header = (struct udphdr *)(buffer + (ip_header->ihl * 4));
        
        char dst_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ip_header->daddr), dst_ip, INET_ADDRSTRLEN);
        
        if (strcmp(dst_ip, server_ip) == 0 && (ntohs(udp_header->dest) == server_port || ntohs(udp_header->dest) == client_port)) {
            process_packet(buffer, packet_size);
        }
    }
    //
    close(raw_socket);
    close(log_fd);
    exit(EXIT_SUCCESS);
}