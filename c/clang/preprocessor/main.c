#include <stdio.h>

#define SQUARE(x) ((x) * (x))

int main() {
    int a = 5;
    size_t s;
    printf("Square of %d is %d\n", a, SQUARE(a));
    return 0;
}
// clang -E main.c 只做预处理

// https://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html
