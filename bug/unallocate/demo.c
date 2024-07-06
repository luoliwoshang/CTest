#include <stdio.h>
#include <stdlib.h>

void free_pointer(void *ptr)
{
    free(ptr);
}

void memory_error_demo()
{
    char *str = "This is a string literal";
    free_pointer(str); // 错误：试图释放一个字符串字面量
}

int main()
{
    printf("Demonstrating memory error...\n");
    memory_error_demo();
    printf("This line will not be executed due to the error.\n");
    return 0;
}
