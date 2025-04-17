#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_SIZE 136
#define TERMINATE_PRIO 255

struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    key_t key = ftok("chat", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    struct msg_buffer message;
    printf("User1 started. Waiting for messages...\n");

    while (1) {

        ssize_t bytes = msgrcv(msgid, &message, MSG_SIZE, 0, 0);
        if (bytes == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        if (message.msg_type == TERMINATE_PRIO) {
            printf("Termination signal received. Exiting...\n");
            break;
        }
        printf("Received from User2: %s\n", message.msg_text);
        printf("User1: ");
        fgets(message.msg_text, MSG_SIZE, stdin);
        message.msg_text[strcspn(message.msg_text, "\n")] = 0;
        message.msg_type = 1;

        if (strcmp(message.msg_text, "exit") == 0) {
            message.msg_type = TERMINATE_PRIO;
            strcpy(message.msg_text, "TERMINATE");
            if (msgsnd(msgid, &message, MSG_SIZE, 0) == -1) {
                perror("msgsnd failed");
                exit(EXIT_FAILURE);
            }
            break;
        }
        if (msgsnd(msgid, &message, MSG_SIZE, 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
    }
    msgctl(msgid, IPC_RMID, NULL);
}