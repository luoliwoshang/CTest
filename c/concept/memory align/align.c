#include <stdio.h>
#include <stddef.h>

struct AlignedStruct {
    int a;    // 4 字节对齐
    char b;   // 1 字节对齐
    float c;  // 4 字节对齐
};

int main() {
    printf("Size of AlignedStruct: %lu\n", sizeof(struct AlignedStruct));
    printf("Offset of a: %lu\n", offsetof(struct AlignedStruct, a));
    printf("Offset of b: %lu\n", offsetof(struct AlignedStruct, b));
    printf("Offset of c: %lu\n", offsetof(struct AlignedStruct, c));

    return 0;
}
