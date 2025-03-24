#include <stdio.h>
#include <string.h>

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
    return a/b;
}

int main(){
    float a = 0, b = 0;
    int option;
    int cmd = 1;
    while (cmd) {
        printf("Menu\n");
        printf("1. Summary of two nubmers\n");
        printf("2. Subtract b from a\n");
        printf("3. Multiply a and b\n");
        printf("4. Divide a by b\n");
        printf("0. Exit\n");
        printf("Enter Option: ");
        scanf("%d", &cmd);
        fseek(stdin, 0, SEEK_END);
        switch (cmd) {
            case 1:
                printf("Enter your numbers: ");
                scanf("%f %f", &a, &b);
                printf("Sum = %f\n", sum(a,b));
                break;
            case 2:
                printf("Enter your numbers: ");
                scanf("%f %f", &a, &b);
                printf("Subtraction = %f\n", sub(a,b));
                break;
            case 3:
                printf("Enter your numbers: ");
                scanf("%f %f", &a, &b);
                printf("Multiplication = %f\n", mult(a,b));
                break;
            case 4:
                printf("Enter your numbers: ");
                scanf("%f %f", &a, &b);
                if (b == 0) printf("Can't divide by zero\n"); else printf("Divided = %f\n", div(a,b));
                break;
            default:
                break;
        }
    }
}