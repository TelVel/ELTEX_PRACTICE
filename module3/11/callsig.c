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
#include <semaphore.h>
#include <sys/mman.h>

sem_t *mutex;          // Protects `read_count`
sem_t *rw_mutex;       // Ensures exclusive write access
sem_t *writer_block;   // Prevents writer starvation
int *read_count;       // Shared counter for active readers

int log_fd = -1;

void write_log(const char *message) {
    if (log_fd != -1) {
        write(log_fd, message, strlen(message));
    }
}

void reader(int id, int num_iterations) {
    for (int i = 0; i < num_iterations; i++) {
        sem_wait(writer_block);
        sem_wait(mutex);
        (*read_count)++;
        if (*read_count == 1) {
            sem_wait(rw_mutex);
        }
        sem_post(mutex);
        sem_post(writer_block);
        char log_msg[256];
        int fd = open("shared.txt", O_RDONLY);
        if (fd == -1) {
            perror("reader: open");
            exit(EXIT_FAILURE);
        }
        char buf[256];
        ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
        close(fd);
        if (bytes_read > 0) {
            buf[bytes_read] = '\0';
            snprintf(log_msg, sizeof(log_msg), "\nReader %d read: %s\n\n", id, buf);
            write_log(log_msg);
            
        }
        sem_wait(mutex);
        (*read_count)--;
        if (*read_count == 0) {
            sem_post(rw_mutex);
        }
        sem_post(mutex);
        usleep(rand() % 500000);
    }
}

void writer(int id, int num_iterations) {
    for (int i = 0; i < num_iterations; i++) {
        sem_wait(writer_block);
        sem_wait(rw_mutex);
        char log_msg[256];
        int fd = open("shared.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1) {
            perror("writer: open");
            exit(EXIT_FAILURE);
        }
        char buf[64];
        int num = rand() % 100;
        int len = snprintf(buf, sizeof(buf), "Writer %d wrote: %d\n", id, num);
        write(fd, buf, len);
        close(fd);
        snprintf(log_msg, sizeof(log_msg), "Writer %d wrote: %d\n", id, num);
        write_log(log_msg);
        sem_post(rw_mutex);
        sem_post(writer_block);
        usleep(rand() % 500000);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <readers> <writers> <iterations>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    if (num_readers <= 0 || num_writers <= 0 || iterations <= 0) {
        fprintf(stderr, "All arguments must be positive integers\n");
        exit(EXIT_FAILURE);
    }
    read_count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *read_count = 0;
    mutex = sem_open("/mutex", O_CREAT, 0644, 1);
    rw_mutex = sem_open("/rw_mutex", O_CREAT, 0644, 1);
    writer_block = sem_open("/writer_block", O_CREAT, 0644, 1);
    if (mutex == SEM_FAILED || rw_mutex == SEM_FAILED || writer_block == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    log_fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (log_fd == -1) {
        perror("open log.txt");
        exit(EXIT_FAILURE);
    }
    int fd = open("shared.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open shared.txt");
        exit(EXIT_FAILURE);
    }
    close(fd);
    for (int i = 0; i < num_readers; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            srand(time(NULL) ^ getpid());
            reader(i + 1, iterations);
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_writers; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            srand(time(NULL) ^ getpid());
            writer(i + 1, iterations);
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_readers + num_writers; i++) {
        wait(NULL);
    }
    sem_close(mutex);
    sem_close(rw_mutex);
    sem_close(writer_block);
    sem_unlink("/mutex");
    sem_unlink("/rw_mutex");
    sem_unlink("/writer_block");
    munmap(read_count, sizeof(int));
    close(log_fd);
}