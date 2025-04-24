#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>

#define SHM_SIZE 1024

typedef struct {
    int numbers[100];
    int count;
    int max;
    int min;
    int ready;
    int processed;
} SharedData;

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    keep_running = 0;
}

int main() {
    int shmid;
    SharedData *shared_data;
    signal(SIGINT, handle_sigint);
    
    key_t key = ftok(".", 'S');
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    shared_data = (SharedData*)shmat(shmid, NULL, 0);
        if (shared_data == (void*)-1) {
            perror("shmat");
            exit(EXIT_FAILURE);
        }
    
    shared_data->ready = 0;
    shared_data->processed = 0;
    
    srand(time(NULL));
    int sets_processed = 0;
    
    while (keep_running) {
        shared_data->count = 5 + rand() % 16;
        
        printf("Parent: generated %d numbers: ", shared_data->count);
        for (int i = 0; i < shared_data->count; i++) {
            shared_data->numbers[i] = rand() % 1000;
            printf("%d ", shared_data->numbers[i]);
        }
        printf("\n");
        shared_data->ready = 1;
        pid_t pid = fork();
        
        if (pid == 0) {
            if (shared_data->ready) {
                shared_data->min = shared_data->numbers[0];
                shared_data->max = shared_data->numbers[0];
                for (int i = 1; i < shared_data->count; i++) {
                    if (shared_data->numbers[i] < shared_data->min) {
                        shared_data->min = shared_data->numbers[i];
                    }
                    if (shared_data->numbers[i] > shared_data->max) {
                        shared_data->max = shared_data->numbers[i];
                    }
                }
                shared_data->processed = 1;
            }
            exit(EXIT_SUCCESS);
        } else if (pid > 0) { 
            while (!shared_data->processed && keep_running) {
                usleep(1000);
            }
            if (keep_running) {
                printf("Parent: min = %d, max = %d\n", shared_data->min, shared_data->max);
                sets_processed++;
                shared_data->ready = 0;
                shared_data->processed = 0;
            }
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
    printf("\nCombinations processed: %d\n", sets_processed);
    shmdt(shared_data);  

}