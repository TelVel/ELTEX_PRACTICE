#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>

#define SHM_SIZE 1024

typedef struct {
    int numbers[SHM_SIZE];
    int count;
    int max;
    int min;
    int processed_sets;
} shared_data;

volatile sig_atomic_t stop_flag = 0;

void handle_sigint(int sig) {
    stop_flag = 1;
}

int main() {
    signal(SIGINT, handle_sigint);
    srand(time(NULL));
    const char *shm_name = "/my_shared_memory";
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shm_fd, sizeof(shared_data)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    shared_data *data = mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    data->processed_sets = 0;
    while (!stop_flag) {
        data->count = 5 + rand() % 16;
        printf("Родитель сгенерировал %d чисел: ", data->count);
        for (int i = 0; i < data->count; i++) {
            data->numbers[i] = rand() % 1000;
            printf("%d ", data->numbers[i]);
        }
        printf("\n");
        pid_t pid = fork();
        if (pid == 0) {
            if (data->count == 0) {
                exit(EXIT_SUCCESS);
            }
            data->min = data->numbers[0];
            data->max = data->numbers[0];
            
            for (int i = 1; i < data->count; i++) {
                if (data->numbers[i] < data->min) {
                    data->min = data->numbers[i];
                }
                if (data->numbers[i] > data->max) {
                    data->max = data->numbers[i];
                }
            }
            data->processed_sets++;
            munmap(data, sizeof(shared_data));
            close(shm_fd);
            exit(EXIT_SUCCESS);
        } else if (pid > 0) {
            wait(NULL);
            printf("Дочерний процесс нашел: min=%d, max=%d\n", data->min, data->max);
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    printf("\nОбработано наборов данных: %d\n", data->processed_sets);
    munmap(data, sizeof(shared_data));
    close(shm_fd);
    shm_unlink(shm_name);

    return 0;
}