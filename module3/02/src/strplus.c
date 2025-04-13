#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char* argv[]){
    size_t length = 0;
    for (int i = 0; i < argc; i++) length += strlen(argv[i]);
    length += 1;
    char *result = (char *)malloc(length);
    strcpy(result, argv[0]);
    for (int i = 1; i < argc; i++) strcat(result, argv[i]);
    printf("%s \n", result);
    free(result);
    exit(EXIT_SUCCESS);
}