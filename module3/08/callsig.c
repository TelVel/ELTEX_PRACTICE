#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <string.h>

int log_fd = -1;

void write_log(const char *message) {
    if (log_fd != -1) {
        write(log_fd, message, strlen(message));
    }
}

void sem_lock(int sem_id) {
    struct sembuf op = {0, -1, SEM_UNDO};
    if (semop(sem_id, &op, 1) == -1) {
        perror("semop lock");
        exit(EXIT_FAILURE);
    }
}

void sem_unlock(int sem_id) {
    struct sembuf op = {0, 1, SEM_UNDO};
    if (semop(sem_id, &op, 1) == -1) {
        perror("semop unlock");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    char log_msg[270];
    int sem_id;
    log_fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (log_fd == -1) {
        perror("open log.txt");
        exit(EXIT_FAILURE);
    }
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_count>\n", argv[0]);
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    int num_count = atoi(argv[1]);
    if (num_count <= 0) {
        fprintf(stderr, "Number count must be positive\n");
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    key_t key = ftok(".", 'S');
    sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    if (semctl(sem_id, 0, SETVAL, 1) == -1) {
        perror("semctl SETVAL");
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        close(pipefd[0]);
        srand(time(NULL));

        for (int i = 0; i < num_count; i++) {
            sem_lock(sem_id);
            write_log("Child locked semaphore\n");
            int fd = open("shared.txt", O_RDONLY);
            if (fd == -1) {
                perror("child: open");
                close(log_fd);
                exit(EXIT_FAILURE);
            }
            char buf[256];
            ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
            close(fd);
            if (bytes_read > 0) {
                buf[bytes_read] = '\0';
                snprintf(log_msg, sizeof(log_msg), "\tChild read: %s\n", buf);
                write_log(log_msg);
            }
            sem_unlock(sem_id);
            write_log("Child unlocked semaphore\n\n");

            int num = rand() % 100;
            write(pipefd[1], &num, sizeof(num));
            snprintf(log_msg, sizeof(log_msg), "\tChild sent: %d\n", num);
            write_log(log_msg);
        }

        close(pipefd[1]);
        close(log_fd);
        exit(EXIT_SUCCESS);
    } else {
        close(pipefd[1]);
        int fd = open("shared.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("parent: open");
            exit(EXIT_FAILURE);
        }
        close(fd);
        for (int i = 0; i < num_count; i++) {
            int num;
            read(pipefd[0], &num, sizeof(num));
            snprintf(log_msg, sizeof(log_msg), "\tParent received: %d\n\n", num);
            write_log(log_msg);
            sem_lock(sem_id);
            write_log("Parent locked semaphore\n");
            fd = open("shared.txt", O_WRONLY | O_APPEND);
            if (fd == -1) {
                perror("parent: open for append");
                exit(EXIT_FAILURE);
            }
            char buf[64];
            int len = snprintf(buf, sizeof(buf), "%d\n", num);
            write(fd, buf, len);
            write_log("\tParent wrote a number\n");
            close(fd);
            sem_unlock(sem_id);
            write_log("Parent unlocked semaphore\n\n");
        }
        close(pipefd[0]);
        wait(NULL);
        if (semctl(sem_id, 0, IPC_RMID) == -1) {
            perror("semctl IPC_RMID");
        }
    }
    close(log_fd);
    return 0;
}