#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <sys/types.h>
#include <unistd.h>

#define NETLINK_USER 31
#define MAX_LOAD 1024

void init_src_addr();
void init_dst_addr();
void init_header();
void construct_msg(const char*);
struct sockaddr_nl src_addr, dst_addr;
struct nlmsghdr* net_header = NULL;
struct iovec vector;
struct msghdr msg_header;
int sock_fd;

void init_src_addr() {
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();
}
void init_dst_addr() {
    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.nl_family = AF_NETLINK;
    dst_addr.nl_pid = 0; 
    dst_addr.nl_groups = 0;
}
void init_header() {
    net_header = (struct nlmsghdr*) malloc(NLMSG_SPACE(MAX_LOAD));
    memset(net_header, 0, NLMSG_SPACE(MAX_LOAD));
    net_header->nlmsg_len = NLMSG_SPACE(MAX_LOAD);
    net_header->nlmsg_pid = getpid();
    net_header->nlmsg_flags = 0;
}

void construct_msg(const char* msg) {
    strcpy(NLMSG_DATA(net_header), msg);
    vector.iov_base = (void *)net_header;
    vector.iov_len = net_header->nlmsg_len;
    msg_header.msg_name = (void *)&dst_addr;
    msg_header.msg_namelen = sizeof(dst_addr);
    msg_header.msg_iov = &vector;
    msg_header.msg_iovlen = 1;
}

int main() {
    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
    if(sock_fd < 0) {
        perror("socket");
        return -1;
    }
    init_src_addr();
    bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));
    init_dst_addr();

    init_header();

    char* mesg;
    fgets(mesg, MAX_LOAD, stdin);
    construct_msg(mesg);


    printf("Sending message to kernel\n");
    sendmsg(sock_fd, &msg_header, 0);
    printf("Waiting for message from kernel\n");

    recvmsg(sock_fd, &msg_header, 0);
    printf("Received message payload: %s\n", (char*)NLMSG_DATA(net_header));
    close(sock_fd);
}