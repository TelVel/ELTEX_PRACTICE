#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

    void IntToBin(int num, char *binary_int) {
        for (int i = 0; i < 3; i++)
        {
            int digit = num % 10;
            for (int j = 0; j < 3; j++) {
                binary_int[2-j+i*3] = (digit & (1<<j)) ? '1' : '0';
            }
            num /=10;
        }
        binary_int[9] = '\0';
    }
    void CharToBin(const char* perm, char *binary_ch) {
        const char *rwx = "rwx";
        for (int i = 0; i < 9; i++)
            {
                    if (strchr(rwx, perm[i])){
                        binary_ch[i] ='1';
                    } else {
                        binary_ch[i] ='0';
                    }
            }
        binary_ch[9] = '\0';
    }

    void PermToString (mode_t mode, char *permission) {
        permission[0] = (mode & S_IRUSR) ? 'r' : '-';
        permission[1] = (mode & S_IWUSR) ? 'w' : '-';
        permission[2] = (mode & S_IXUSR) ? 'x' : '-';
        permission[3] = (mode & S_IRGRP) ? 'r' : '-';
        permission[4] = (mode & S_IWGRP) ? 'w' : '-';
        permission[5] = (mode & S_IXGRP) ? 'x' : '-';
        permission[6] = (mode & S_IROTH) ? 'r' : '-';
        permission[7] = (mode & S_IWOTH) ? 'w' : '-';
        permission[8] = (mode & S_IXOTH) ? 'x' : '-';
        permission[9] = '\0';
    }
    void PermToBin (mode_t mode, char *permission) {
        permission[0] = (mode & S_IRUSR) ? '1' : '0';
        permission[1] = (mode & S_IWUSR) ? '1' : '0';
        permission[2] = (mode & S_IXUSR) ? '1' : '0';
        permission[3] = (mode & S_IRGRP) ? '1' : '0';
        permission[4] = (mode & S_IWGRP) ? '1' : '0';
        permission[5] = (mode & S_IXGRP) ? '1' : '0';
        permission[6] = (mode & S_IROTH) ? '1' : '0';
        permission[7] = (mode & S_IWOTH) ? '1' : '0';
        permission[8] = (mode & S_IXOTH) ? '1' : '0';
        permission[9] = '\0';
    }
    int PermToInt (mode_t mode)
    {
        int user = (((mode & S_IRUSR) ? 4 : 0)+((mode & S_IWUSR) ? 2 : 0)+((mode & S_IXUSR) ? 1 : 0))*100;
        int group = ((((mode & S_IRGRP) ? 4 : 0)+((mode & S_IWGRP) ? 2 : 0)+((mode & S_IXGRP) ? 1 : 0))*10);
        int other = ((mode & S_IROTH) ? 4 : 0)+((mode & S_IWOTH) ? 2 : 0)+((mode & S_IXOTH) ? 1 : 0);
        return user+group+other;
    }

    mode_t ModifyPerm(mode_t mode, const char *command){
        if (command[1] == '+'){
            if (command[0] == 'u' || command[0] == 'a'){
                for (int i = 2; command[i]; i++){
                    switch (command[i]){
                        case 'r': mode |= S_IRUSR; break;
                        case 'w': mode |= S_IWUSR; break;
                        case 'x': mode |= S_IXUSR; break;
                    }
                }
            } 
            if (command[0] == 'g' || command[0] == 'a'){
                for (int i = 2; command[i]; i++){
                    switch (command[i]){
                        case 'r': mode |= S_IRGRP; break;
                        case 'w': mode |= S_IWGRP; break;
                        case 'x': mode |= S_IXGRP; break;
                    }
                }
            } 
            if (command[0] == 'o' || command[0] == 'a'){
                for (int i = 2; command[i]; i++){
                    switch (command[i]){
                        case 'r': mode |= S_IROTH; break;
                        case 'w': mode |= S_IWOTH; break;
                        case 'x': mode |= S_IXOTH; break;
                    }
                }
            }
        }
        else if (command[1] == '-'){
            if (command[0] == 'u' || command[0] == 'a'){
                for (int i = 2; command[i]; i++){
                    switch (command[i]){
                        case 'r': mode &= ~S_IRUSR; break;
                        case 'w': mode &= ~S_IWUSR; break;
                        case 'x': mode &= ~S_IXUSR; break;
                    }
                }
            }
            if (command[0] == 'g' || command[0] == 'a'){
                for (int i = 2; command[i]; i++){
                    switch (command[i]){
                        case 'r': mode &= ~S_IRGRP; break;
                        case 'w': mode &= ~S_IWGRP; break;
                        case 'x': mode &= ~S_IXGRP; break;
                    }
                }
            } 
            if (command[0] == 'o' || command[0] == 'a'){
                for (int i = 2; command[i]; i++){
                    switch (command[i]){
                        case 'r': mode &= ~S_IROTH; break;
                        case 'w': mode &= ~S_IWOTH; break;
                        case 'x': mode &= ~S_IXOTH; break;
                    }
                }
            }
        }
 /*       else if (command[1] == '='){
            if (command[0] == 'u' || command[0] == 'a'){
                for (int i = 2; command[i]; i++){
                    switch (command[i]){
                        case 'r': mode = S_IRUSR; break;
                        case 'w': mode = S_IWUSR; break;
                        case 'x': mode = S_IXUSR; break;
                    }
                }
            } 
            if (command[0] == 'g' || command[0] == 'a'){
                for (int i = 2; command[i]; i++){
                    switch (command[i]){
                        case 'r': mode = S_IRGRP; break;
                        case 'w': mode = S_IWGRP; break;
                        case 'x': mode = S_IXGRP; break;
                    }
                }
            }
            if (command[0] == 'o' || command[0] == 'a'){
                for (int i = 2; command[i]; i++){
                    switch (command[i]){
                        case 'r': mode = S_IROTH; break;
                        case 'w': mode = S_IWOTH; break;
                        case 'x': mode = S_IXOTH; break;
                    }
                }
            } 
        } */
        return mode;
    }

    int main(int argc, char* argv[]) {
        char perm[10];
        char *ptr = argv[1];
        long num;
        num = strtol(ptr, &ptr, 10);
        if (argc == 2){
            if (*ptr == '\0'){
                char binary_int[10];
                int UB = atoi(argv[1]);
                IntToBin(UB, binary_int);
                printf("%s \n", binary_int);
            } else {
                if (strlen(argv[1])!=9) {
                    struct stat buffer;
                    const char* filename = argv[1];
                    stat(filename, &buffer);
                    PermToString(buffer.st_mode, perm);
                    printf("%s\n", perm);
                    PermToBin(buffer.st_mode, perm);
                    printf("%s\n", perm);
                    printf("%d\n", PermToInt(buffer.st_mode));
                    } else {
                        char binary_char[10];
                        const char *perm_ch = argv[1];
                        CharToBin(perm_ch, binary_char);
                        printf("%s \n", binary_char);
                    }
            }
        }
        char command[10];
        if (argc == 3){
            struct stat buffer;
            mode_t mode;
            const char* filename = argv[2];
            stat(filename, &buffer);
            PermToString(buffer.st_mode, perm);
            printf("Default string permissions: %s\n", perm);
            PermToBin(buffer.st_mode, perm);
            printf("Default binary permissions: %s\n", perm);
            printf("Default number permissions: %d\n", PermToInt(buffer.st_mode));
            const char* command = argv[1];
            mode = ModifyPerm(buffer.st_mode, command);
            PermToString(mode, perm);
            printf("Change string permissions: %s\n", perm);
            PermToBin(mode, perm);
            printf("Change binary permissions: %s\n", perm);
            printf("Change number permissions: %d\n", PermToInt(mode));       
            }
        }
    
