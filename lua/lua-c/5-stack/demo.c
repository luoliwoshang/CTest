#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>

void printStack(lua_State *L) {
    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++) {
        printf("%s, ", lua_tostring(L, i));
    }
    printf("\n");
}

int main(void) {
    lua_State *L = luaL_newstate();   // 创建 Lua 状态机
    luaL_openlibs(L);                 // 打开 Lua 标准库

    // 压入一些初始值
    lua_pushstring(L, "Hello");       // 索引 1
    lua_pushstring(L, "World");       // 索引 2
    lua_pushnumber(L, 42);            // 索引 3

    // 显示当前栈顶位置
    printf("Initial top: %d\n", lua_gettop(L));// Initial top: 3

    // 使用 lua_absindex 确保索引是正的
    int idx = lua_absindex(L, -2);
    printf("Absolute index of 'World': %d\n", idx); //Absolute index of 'World': 2

    // 复制 'World' 到栈顶
    lua_pushvalue(L, -2);
    printf("Top after pushing 'World': %d\n", lua_gettop(L));// Top after pushing 'World': 4

    // 打印旋转前的栈
    printf("Stack before rotate: "); 
    printStack(L); //  Hello, World, 42.0, World, 
    // 旋转栈顶三个元素
    lua_rotate(L, 1, -1);

    // 打印旋转后的栈
    printf("Stack after rotate: ");
    printStack(L);// World, 42.0, World, Hello, 

    // 复制栈顶元素到索引2位置
    lua_copy(L, -1, 2);//
    printStack(L);// World, Hello, World, Hello, 

    // 检查并扩展栈空间
    if (!lua_checkstack(L, 2)) {
        fprintf(stderr, "Cannot grow stack\n");
        return -1;
    }

    // 再压入两个元素
    lua_pushnumber(L, 3.14);
    lua_pushstring(L, "Lua");
    printStack(L);// World, Hello, World, Hello, 3.14, Lua, 


    // 设置栈顶，清除额外元素
    lua_settop(L, 5); 
    printf("Top after settop 5: %d\n", lua_gettop(L));
    printStack(L);// World, Hello, World, Hello, 3.14, 

    // 创建第二个状态机
    lua_State *L1 = luaL_newstate();

    // 移动两个元素到新的状态机
    lua_xmove(L, L1, 2);
    printf("Top after xmove: %d\n", lua_gettop(L));// Top after xmove: 3
    printStack(L);// World, Hello, World, 


    printf("L1 top: %d\n", lua_gettop(L1));// L1 top: 2
    printStack(L1);// Hello, 3.14,

    // 清理并关闭 Lua 状态机
    lua_close(L);
    lua_close(L1);
    return 0;
}
