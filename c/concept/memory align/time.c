#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h>

struct AlignedStruct {
    int a;
    char b;
    float c;
};

struct __attribute__((packed)) UnalignedStruct {
    char a;
    int b;
    float c;
};

#define SIZE 1000000
#define ITERATIONS 4000

int main() {
    struct AlignedStruct *alignedArr = (struct AlignedStruct *)malloc(SIZE * sizeof(struct AlignedStruct));
    struct UnalignedStruct *unalignedArr = (struct UnalignedStruct *)malloc(SIZE * sizeof(struct UnalignedStruct));
    if (alignedArr == NULL || unalignedArr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    clock_t start, end;
    double cpu_time_used;

    // 测试对齐结构体
    start = clock();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (int i = 0; i < SIZE; i++) {
            alignedArr[i].a = i;
            alignedArr[i].b = i % 256;
            alignedArr[i].c = i * 0.5f;
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for aligned array: %f seconds\n", cpu_time_used);

    // 测试未对齐结构体
    start = clock();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (int i = 0; i < SIZE; i++) {
            unalignedArr[i].a = i % 256;
            unalignedArr[i].b = i;
            unalignedArr[i].c = i * 0.5f;
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for unaligned array: %f seconds\n", cpu_time_used);

    free(alignedArr);
    free(unalignedArr);

    return 0;
}
