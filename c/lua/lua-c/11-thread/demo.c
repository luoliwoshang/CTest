#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    printf("%.1f", lua_version(L));

    // 创建一个新线程
    lua_State *thread = lua_newthread(L);
    printf("1. Created a new thread\n");

    // 将线程推入栈中
    int isMain = lua_pushthread(L);
    printf("2. Pushed thread onto its own stack. Is main thread? %s\n", isMain ? "Yes" : "No");

    // 从栈中获取线程
    lua_State *retrieved_thread = lua_tothread(thread, -1);
    if (retrieved_thread == thread) {
        printf("3. Successfully retrieved thread from stack\n");
    } else {
        printf("3. Failed to retrieve thread from stack\n");
    }

    // 关闭线程
    lua_closethread(L, thread);
    printf("4. Closed the thread\n");

    lua_close(L);
    return 0;
}