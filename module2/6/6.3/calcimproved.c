#include <stdio.h>
#include <string.h>
#include <math.h>
#include "calc.h"
#include <dlfcn.h>

int main(){
    float a = 0, b = 0;
    int option;
    void* handle;
    int cmd = 1;
    float (*operations)(float, float);

    
    while (cmd) {
        printf("Menu\n");
        printf("1. Summary of two nubmers\n");
        printf("2. Subtract b from a\n");
        printf("3. Multiply a and b\n");
        printf("4. Divide a by b\n");
        printf("5. Power a by b\n");
        printf("0. Exit\n");
        printf("Enter Option: ");
        scanf("%d", &cmd);
        fseek(stdin, 0, SEEK_END);
        if (cmd != 0)
        {
            switch (cmd)
            {
            case 1:
                handle = dlopen("lib/libsum.so", RTLD_LAZY);
                if (!handle) {
                    fputs (dlerror(), stderr);
                    }
                printf("Enter your numbers: ");
                scanf("%f %f", &a, &b);
                operations = dlsym(handle, "sum");
                b = operations(a, b);
                if (b != INFINITY) printf("Result: %f\n ", b); else printf("Can't divide by zero\n"); 
                dlclose(handle);
                break;
            case 2:
            handle = dlopen("lib/libsub.so", RTLD_LAZY);
            if (!handle) {
                fputs (dlerror(), stderr);
                }
            printf("Enter your numbers: ");
            scanf("%f %f", &a, &b);
            operations = dlsym(handle, "sub");
            b = operations(a, b);
            if (b != INFINITY) printf("Result: %f\n ", b); else printf("Can't divide by zero\n"); 
            dlclose(handle);
                break;
             case 3:
             handle = dlopen("lib/libmult.so", RTLD_LAZY);
             if (!handle) {
                fputs (dlerror(), stderr);
                }
             printf("Enter your numbers: ");
             scanf("%f %f", &a, &b);
             operations = dlsym(handle, "mult");
             b = operations(a, b);
             if (b != INFINITY) printf("Result: %f\n ", b); else printf("Can't divide by zero\n"); 
             dlclose(handle);
                break;
           case 4:
           handle = dlopen("lib/libdiv.so", RTLD_LAZY);
           if (!handle) {
            fputs (dlerror(), stderr);
            }
           printf("Enter your numbers: ");
           scanf("%f %f", &a, &b);
           operations = dlsym(handle, "div");
           b = operations(a, b);
           if (b != INFINITY) printf("Result: %f\n ", b); else printf("Can't divide by zero\n"); 
           dlclose(handle);
                break;
          case 5:
          handle = dlopen("lib/libpower.so", RTLD_LAZY);
          if (!handle) {
            fputs (dlerror(), stderr);
            }
          printf("Enter your numbers: ");
          scanf("%f %f", &a, &b);
          operations = dlsym(handle, "powa");
          b = operations(a, b);
          if (b != INFINITY) printf("Result: %f\n ", b); else printf("Can't divide by zero\n"); 
          dlclose(handle);
                break;
            default:
                break;
            }

        }
    }
}