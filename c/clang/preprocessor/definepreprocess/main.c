#include <stdio.h>

#define A 1

#ifdef WINDOWS
#define OS "windows"
#else
#define OS "unix"
#endif

#if A
#define B 2
#else
#define B 3
#endif

int main() { return 0; }
// clang -E -dM main.c
// #define A 1
// #define OS "unix" (因为 WINDOWS 未定义)
// #define B 2 (因为 A 等于 1)
// 不会看到 #define OS "windows" 或 #define B 3

// 首先解析所有条件指令
// 根据条件判断保留或移除相关代码块
// 只有实际生效的宏定义才会被显示
