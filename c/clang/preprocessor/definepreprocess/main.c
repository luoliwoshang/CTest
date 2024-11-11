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

#ifdef __BLOCKS__
void debug_func(void);
#endif

#ifdef not_defined
void not_defined_func(void);
#endif

int main() { return 0; }
// clang -E main.c
// llcppsigfetch --extract "./main.c" -temp=false -cpp=false -out=true
// #define A 1
// #define OS "unix" (因为 WINDOWS 未定义)
// #define B 2 (因为 A 等于 1)
// 不会看到 #define OS "windows" 或 #define B 3

// 并且对于 #ifdef __BLOCKS__ 的宏定义，对应的
// debug_func 由于默认__BLOCKS__是开启状态，所以也会输出这个定义，对于clang -E 的输出以及 llcppsigfetch
// 的输出，都会输出这个定义 并对于 not_defined 对应的代码块，由于并没有声明该宏定义，所以不会输出 not_defined_func
// 的定义
