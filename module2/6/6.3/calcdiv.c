#include "calc.h"
#include <math.h>

float div (float a, float b) {
    if (b == 0) return INFINITY;
    return a/b;
}