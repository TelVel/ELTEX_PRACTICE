#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>

#define SIZE 128
#define QUEUE_NAME "/queue"

int main(){
    mqd_t pos;
    char msg[SIZE];
    int prio;

    if((pos = mq_open(QUEUE_NAME, O_RDWR)) == (mqd_t)-1){
        perror("open q");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Enter message: ");
        fflush(stdout);
        fgets(msg, SIZE, stdin);

        msg[strcspn(msg, "\n")] = 0;
        if(strcmp(msg, "exit") == 0){
            mq_send(pos, msg, strlen(msg), 255);
            break;
        }
        if (mq_send(pos, msg, strlen(msg), 2) == -1){
            perror("Send Error");
            continue;
        }
        
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
    }

    mq_close(pos);
    return 0;
}