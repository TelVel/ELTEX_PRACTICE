#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>

#define SIZE 128
#define QUEUE_NAME "/queue"

int main() {
    mqd_t pos;
    struct mq_attr queue_attr;
    char msg[SIZE];
    int prio;
    
    queue_attr.mq_maxmsg = 10;
    queue_attr.mq_msgsize = SIZE;
    queue_attr.mq_flags = 0;
    mq_unlink(QUEUE_NAME);
    
    if((pos = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0600, &queue_attr)) == (mqd_t)-1){
        perror("create q");
        exit(EXIT_FAILURE);
    }

    while(1){
        ssize_t bytes = mq_receive(pos, msg, SIZE, &prio);
        if(bytes == -1) {
            perror("receive error");
            continue;
        }
        if(prio == 255)
        {
            break;
        }
        msg[bytes] = '\0';
        printf("Received: %s \n", msg);
        printf("Enter message: ");
        fflush(stdout);
        fgets(msg, SIZE, stdin);
        msg[strcspn(msg, "\n")] = 0;
        if(strcmp(msg, "exit") == 0){
            mq_send(pos, msg, strlen(msg), 255);
            break;
        }
        if (mq_send(pos, msg, strlen(msg), 1) == -1){
            perror("Send Error");
            continue;
        }
    }

    mq_close(pos);
    mq_unlink(QUEUE_NAME);
    return 0;
}