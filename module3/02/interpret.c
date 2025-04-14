#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main ()
{
    int cmd = 1;
    while (cmd)
    {
    printf("> ");
    char func[10];
    int cnt;
    char temp[20];
    scanf("%s", func);
    if (strcmp(func, "exit") == 0)
    {
        cmd = -1;
        break;
    }
    scanf("%d", &cnt);
    if (cnt <= 0) {printf("No Args\n"); exit(EXIT_FAILURE);}
    char** argv = malloc((cnt+1) * sizeof(char*));
    for (int i = 0; i < cnt; i++) {
        scanf("%s", temp);
        argv[i] = malloc(strlen(temp) * sizeof(char));
        strcpy(argv[i], temp);
    } 
    argv[cnt] = NULL;
    char* funccat = malloc((strlen(func)+2)*sizeof(char));
    strcpy(funccat, "./");
    funccat = strncat(funccat, func, strlen(func));
    pid_t pid;
    switch (pid = fork())
    {
    case 0:
        execvp(funccat, argv);
        perror("execvp fail:");
        exit(EXIT_FAILURE);
        break;
    case -1:
        perror("fork fail");
        exit(EXIT_FAILURE);
        break;
    default:
        waitpid(pid, NULL, 0);
        break;
    }

    for (int i = 0; i < cnt; i++) free(argv[i]);
    free(argv);
    free(funccat);
}
}