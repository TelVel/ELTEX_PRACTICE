#include <stdlib.h>
#include <stdio.h>

int main (int argc, char* argv[]){
    long long int max = 0;
    for (int i = 0; i < argc; i++){
        if (atoi(argv[i]) > max) max = atoll(argv[i]);
    }
    printf("%lld \n", max);
    exit(EXIT_SUCCESS);
}