#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

uint32_t ipToUint(const char *ip)
{
    uint32_t value = 0;
    uint8_t quad;
    char *ipc= strdup(ip);
    char *piece = strtok(ipc, ".");
    for (int i = 0; i < 4; ++i){
        quad = (uint8_t)atoi(piece);
        value = (value << 8) | quad;
        piece = strtok(NULL, ".");
    }
    free(ipc);
    return value;
}

int SubnetTest(uint32_t ip, uint32_t subnet, uint32_t mask){
    return ((subnet & mask) == (ip & mask)) ? 1 : 0;
}

int main (int argc, char* argv[]){
    char *ip= argv[1];
    char *mask = argv[2];
    int N = atoi(argv[3]);
    float cnt = 0;
    srand(time(NULL));
    uint32_t iptemp;
    uint32_t subnet = ipToUint(ip);
    uint32_t maskuint = ipToUint(mask);
    for (int i = 0; i < N; i++){
        for (int j = 4; j > 0; --j){ 
            uint8_t gquad= (uint8_t)(rand() % 255);
            iptemp = (iptemp << 8) | gquad;
        }
        cnt += SubnetTest(iptemp, subnet, maskuint);
    }
    printf("Subnet address count: %f    ", cnt);
    printf("Subnet address percent: %f %%\n", (cnt / N)*100);
    printf("Non-Subnet address count: %f    ", N - cnt);
    printf("Non-Subnet address percent: %f %%\n", ((N - cnt) / N)*100);

}