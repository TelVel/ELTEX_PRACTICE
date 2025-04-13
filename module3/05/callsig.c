#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

volatile sig_atomic_t file_access_allowed = 1;
int log_fd = -1;

void write_log(const char *message) {
    if (log_fd != -1) {
        write(log_fd, message, strlen(message));
    }
}

void sigusr1_handler(int sig) {
    file_access_allowed = 0;
}

void sigusr2_handler(int sig) {
    file_access_allowed = 1;
}

int main(int argc, char *argv[]) {
    char log_msg[270];
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
        struct sigaction sa;
        sa.sa_handler = sigusr1_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGUSR1, &sa, NULL);
        sa.sa_handler = sigusr2_handler;
        sigaction(SIGUSR2, &sa, NULL);

        srand(time(NULL));

        for (int i = 0; i < num_count; i++) {
            while (!file_access_allowed) {
                pause();
            }

            int fd = open("shared.txt", O_RDONLY);
            if (fd == -1) {
                perror("child: open");
                close(log_fd);
                exit(EXIT_FAILURE);
            }

            char buf[256];
            ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
            close(fd);
            while (!file_access_allowed) {
                pause(); 
            }
            if (bytes_read > 0) {
                buf[bytes_read] = '\0';
                snprintf(log_msg, sizeof(log_msg), "Child read: %s\n", buf);
                write_log(log_msg);
            }
            int num = rand() % 100;
            write(pipefd[1], &num, sizeof(num));
            snprintf(log_msg, sizeof(log_msg), "Child sent: %d\n\n", num);
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
            snprintf(log_msg, sizeof(log_msg), "Parent received: %d\n\n", num);
            write_log(log_msg);

            kill(pid, SIGUSR1);
            write_log("Parent locked file access\n");

            fd = open("shared.txt", O_WRONLY | O_APPEND);
            if (fd == -1) {
                perror("parent: open for append");
                exit(EXIT_FAILURE);
            }

            char buf[64];
            int len = snprintf(buf, sizeof(buf), "%d\n", num);
            write(fd, buf, len);
            close(fd);

            kill(pid, SIGUSR2);
            write_log("Parent unlocked file access\n");
        }

        close(pipefd[0]);
        wait(NULL);
    }
}