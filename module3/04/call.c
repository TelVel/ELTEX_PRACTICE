#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

    int num_count = atoi(argv[1]);

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t pid;
    switch (pid = fork())
    {
    case 0:
        close(pipefd[0]);
        srand(time(NULL));
        for (int i = 0; i < num_count; i++) {
            int num = rand() % 1000;
            write(pipefd[1], &num, sizeof(num));
        }
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
        break;
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;
    default:
        close(pipefd[1]);
        int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < num_count; i++) {
            int num;
            read(pipefd[0], &num, sizeof(num));
            char buffer[16];
            int len = snprintf(buffer, sizeof(buffer), "%d ", num);
            write(STDOUT_FILENO, buffer, len);
            write(fd, buffer, len);
        }
        write(STDOUT_FILENO, "\n", 1);
        write(fd, "\n", 1);
        close(fd);
        close(pipefd[0]);
        wait(NULL);
        break;
    }
}