#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main (int argc, char* argv[])
{
    pid_t pid;
    if (argc < 2)
    {
        printf("Squares where?\n");
        return -1;
    }
    int half = (argc) / 2;
    switch (pid = fork())
    {
    case 0:
        for (int i = 1; i < half+1; i++)
        {
          printf("Child of %d: %d\n", atoi(argv[i]), atoi(argv[i])*atoi(argv[i]));
        }
        exit(EXIT_SUCCESS);
        break;
    case -1:
        perror("error fork");
        exit(EXIT_FAILURE);
        break;
    default:
        for (int i = half+1; i < (argc); i++)
        {
            printf("Parent of %d: %d\n",  atoi(argv[i]), atoi(argv[i])*atoi(argv[i]));
        }
        exit(EXIT_SUCCESS);   
        break;
    }
}