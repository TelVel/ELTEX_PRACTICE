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

#define MAX_READERS 5

int log_fd = -1;
int sem_id;

void write_log(const char *message) {
    if (log_fd != -1) {
        write(log_fd, message, strlen(message));
    }
}

void sem_wait(int sem_id, int sem_num) {
    struct sembuf op = {sem_num, -1, SEM_UNDO};
    if (semop(sem_id, &op, 1) == -1) {
        perror("semop wait");
        exit(EXIT_FAILURE);
    }
}

void sem_signal(int sem_id, int sem_num) {
    struct sembuf op = {sem_num, 1, SEM_UNDO};
    if (semop(sem_id, &op, 1) == -1) {
        perror("semop signal");
        exit(EXIT_FAILURE);
    }
}

void reader_lock(int sem_id) {
    sem_wait(sem_id, 2); 
    sem_wait(sem_id, 0); 

    int readers = semctl(sem_id, 3, GETVAL);
    if (readers == 0) {
        sem_wait(sem_id, 1);
    }
    sem_signal(sem_id, 3);
    
    sem_signal(sem_id, 0);
    sem_signal(sem_id, 2); 
}

void reader_unlock(int sem_id) {
    sem_wait(sem_id, 2);  
    
    int readers = semctl(sem_id, 3, GETVAL) - 1;
    if (readers == 0) {
        sem_signal(sem_id, 1); 
    }
    sem_wait(sem_id, 3);
    
    sem_signal(sem_id, 2);
}

void writer_lock(int sem_id) {
    sem_wait(sem_id, 1);
    sem_wait(sem_id, 0);
}

void writer_unlock(int sem_id) {
    sem_signal(sem_id, 0); 
    sem_signal(sem_id, 1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <reader_count> <writer_count>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (atoi(argv[1]) > MAX_READERS) {
        fprintf(stderr, "Too many readers, max readers: %d\n", MAX_READERS);
        exit(EXIT_FAILURE);
    }
    int reader_count = atoi(argv[1]);
    int writer_count = atoi(argv[2]);
    if (reader_count <= 0 || writer_count <= 0) {
        fprintf(stderr, "Counts must be positive\n");
        exit(EXIT_FAILURE);
    }
    
    log_fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (log_fd == -1) {
        perror("open log.txt");
        exit(EXIT_FAILURE);
    }
    
    key_t key = ftok(".", 'S');
    sem_id = semget(key, 4, IPC_CREAT | 0666);  // 4
    if (sem_id == -1) {
        perror("semget");
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    
    arg.val = 1;
    if (semctl(sem_id, 0, SETVAL, arg) == -1) {
        perror("semctl SETVAL 0");
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    
    arg.val = 1;
    if (semctl(sem_id, 1, SETVAL, arg) == -1) {
        perror("semctl SETVAL 1");
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    
    arg.val = 1;
    if (semctl(sem_id, 2, SETVAL, arg) == -1) {
        perror("semctl SETVAL 2");
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    
    arg.val = 0;
    if (semctl(sem_id, 3, SETVAL, arg) == -1) {
        perror("semctl SETVAL 3");
        close(log_fd);
        exit(EXIT_FAILURE);
    }
    
    int fd = open("shared.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open shared.txt");
        exit(EXIT_FAILURE);
    }
    close(fd);
    
    for (int i = 0; i < reader_count; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork reader");
            continue;
        }
        if (pid == 0) {
            srand(time(NULL) ^ getpid());
            for (int j = 0; j < 3; j++) {
                sleep(rand() % 3 + 1);
                reader_lock(sem_id);
                
                char log_msg[300];
                snprintf(log_msg, sizeof(log_msg), "Reader %d (PID %d) starts reading\n", i+1, getpid());
                write_log(log_msg);
                
                fd = open("shared.txt", O_RDONLY);
                if (fd == -1) {
                    perror("reader open");
                    exit(EXIT_FAILURE);
                }
                
                char buf[256];
                ssize_t bytes_read = read(fd, buf, sizeof(buf) - 1);
                close(fd);
                
                if (bytes_read > 0) {
                    buf[bytes_read] = '\0';
                    snprintf(log_msg, sizeof(log_msg), "Reader %d read: \n%s\n", i+1, buf);
                    write_log(log_msg);
                }
                
                snprintf(log_msg, sizeof(log_msg), "Reader %d (PID %d) finishes reading\n\n", i+1, getpid());
                write_log(log_msg);
                
                reader_unlock(sem_id);
            }
            close(log_fd);
            exit(EXIT_SUCCESS);
        }
    }
    
    for (int i = 0; i < writer_count; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork writer");
            continue;
        }
        if (pid == 0) {
            srand(time(NULL) ^ getpid());
            
            for (int j = 0; j < 2; j++) {
                sleep(rand() % 3 + 1);   
                writer_lock(sem_id);
                
                char log_msg[100];
                snprintf(log_msg, sizeof(log_msg), "Writer %d (PID %d) starts writing\n", i+1, getpid());
                write_log(log_msg);
                
                fd = open("shared.txt", O_WRONLY | O_APPEND);
                if (fd == -1) {
                    perror("writer open");
                    exit(EXIT_FAILURE);
                }
                
                char buf[64];
                int num = rand() % 100;
                int len = snprintf(buf, sizeof(buf), "Writer %d: %d\n", i+1, num);
                write(fd, buf, len);
                close(fd);
                
                snprintf(log_msg, sizeof(log_msg), "Writer %d wrote: %d\n", i+1, num);
                write_log(log_msg);
                
                snprintf(log_msg, sizeof(log_msg), "Writer %d (PID %d) finishes writing\n\n", i+1, getpid());
                write_log(log_msg);
                
                writer_unlock(sem_id);
            }
            close(log_fd);
            exit(EXIT_SUCCESS);
        }
    }
    
    for (int i = 0; i < reader_count + writer_count; i++) {
        wait(NULL);
    }
    
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID");
    }
    close(log_fd);
    return 0;
}