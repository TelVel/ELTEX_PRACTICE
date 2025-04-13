#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
   int length = 0;
   char* result;
   for (int i = 0; i < argc; i++) 
        if (strlen(argv[i]) > length) 
        {
            length = strlen(argv[i]);
            result = argv[i];
        }
    printf("string '%s' with length of %d\n", result, length);
    exit(EXIT_SUCCESS);
}