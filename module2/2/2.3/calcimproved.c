#include <stdio.h>
#include <string.h>
#include <math.h>

float sum (float a, float b) {
    return a+b;
}
float sub (float a, float b) {
    return a-b;
}
float mult (float a, float b) {
    return a*b;
}
float div (float a, float b) {
    if (b == 0) return INFINITY;
    return a/b;
}
float powa(float a, float b){
    return pow(a, b);
}

int main(){
    float a = 0, b = 0;
    int option;
    int cmd = 1;
    float (*operations[])(float, float) = {sum, sub, mult, div, powa};
    int n = sizeof(operations)/sizeof(operations[0]);
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
        printf("Enter your numbers: ");
        scanf("%f %f", &a, &b);
        b = operations[cmd-1](a, b);
        if (b != INFINITY) printf("Result: %f\n ", b); else printf("Can't divide by zero\n"); 
        }
    }
}