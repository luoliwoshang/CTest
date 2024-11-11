#include <stdio.h>

#define SQUARE(x) ((x) * (x))

int main() {
    int a = 5;
    printf("Square of %d is %d\n", a, SQUARE(a));
    return 0;
}
// clang -E main.c