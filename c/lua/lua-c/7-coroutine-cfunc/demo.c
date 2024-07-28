#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

// Lua函数:一个简单的协程
static int l_coroutine_function(lua_State *L) {
    printf("Coroutine started\n");
    lua_yield(L, 0); // 暂停协程
    printf("Coroutine resumed\n");
    return 0;
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // 创建一个新的协程
    lua_State *co = lua_newthread(L);

    // 将C函数压入Lua栈
    lua_pushcfunction(co, l_coroutine_function);

    // 第一次恢复协程
    printf("Resuming coroutine for the first time\n");
    int nres;
    int result = lua_resume(co, L, 0, &nres);

    if (result == LUA_YIELD) {
        printf("Coroutine yielded\n");

        // 第二次恢复协程
        printf("Resuming coroutine for the second time\n");
        result = lua_resume(co, L, 0, &nres);

        if (result == 0) {
            printf("Coroutine finished\n");
        }
    }

    lua_close(L);
    return 0;
}