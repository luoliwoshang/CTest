#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 正常情况下分配和释放内存
void normal_memory_handling()
{
    char *str = (char *)malloc(25 * sizeof(char));
    if (str == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    strcpy(str, "This is a dynamic string");
    printf("Normal case: %s\n", str);
    free(str);
}

// 错误情况下尝试释放字符串字面量
void memory_error_demo()
{
    char *str = "This is a string literal";
    free(str); // 错误：试图释放一个字符串字面量
}

int main()
{
    printf("Demonstrating normal memory handling...\n");
    normal_memory_handling();

    printf("Demonstrating memory error...\n");
    memory_error_demo(); // 这行会导致程序崩溃

    printf("This line will not be executed due to the error.\n");
    return 0;
}
