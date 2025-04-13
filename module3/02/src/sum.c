#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]){
    long result = 0;
    for(int i = 0; i < argc; i++) result += atoi(argv[i]);
    printf("%ld\n", result);
    exit(EXIT_SUCCESS);
}