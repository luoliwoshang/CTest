#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static int l_yield_wrapper(lua_State *L) {
    lua_Integer n = luaL_checkinteger(L, 1);
    printf("Yielding with value: %lld\n", (long long)n);
    return lua_yield(L, 1);
}

static int my_coroutine(lua_State *L) {
    // 将 yield_wrapper 函数注册为全局函数
    lua_pushcfunction(L, l_yield_wrapper);
    lua_setglobal(L, "yield_wrapper");

    // 加载并运行 Lua 代码
    const char* lua_code = 
        "local function my_yield(n)\n"
        "    return coroutine.yield(yield_wrapper(n))\n"
        "end\n"
        "local function coro()\n"
        "    print('Coroutine started')\n"
        "    my_yield(10)\n"
        "    print('Coroutine resumed')\n"
        "    my_yield(20)\n"
        "    print('Coroutine finished')\n"
        "end\n"
        "return coro\n";  // 返回协程函数，而不是协程对象

    if (luaL_dostring(L, lua_code) != LUA_OK) {
        printf("Error loading Lua code: %s\n", lua_tostring(L, -1));
        return lua_error(L);
    }

    return 1;  // 返回协程函数
}

void handle_resume_result(lua_State *L, int status) {
    switch (status) {
        case LUA_OK:
            printf("Coroutine finished successfully\n");
            break;
        case LUA_YIELD:
            if (lua_gettop(L) > 0) {
                printf("Yielded value: %lld\n", (long long)lua_tointeger(L, -1));
            }
            break;
        case LUA_ERRRUN:
            printf("Runtime error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
            break;
        case LUA_ERRMEM:
            printf("Memory allocation error\n");
            break;
        case LUA_ERRERR:
            printf("Error while running the error handler function\n");
            break;
        default:
            printf("Unknown error occurred\n");
    }
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushcfunction(L, my_coroutine);
    lua_call(L, 0, 1);  // 调用 my_coroutine 获取协程函数

    // 创建新的协程
    lua_State *co = lua_newthread(L);
    lua_pushvalue(L, -2);  // 复制协程函数
    lua_xmove(L, co, 1);   // 将协程函数移动到新线程

    int nres;
    int status;

    // 首次恢复
    status = lua_resume(co, NULL, 0, &nres);
    printf("After first resume: %d\n", status);
    handle_resume_result(co, status);

    if (status == LUA_YIELD) {
        // 第二次恢复
        status = lua_resume(co, NULL, 0, &nres);
        printf("After second resume: %d\n", status);
        handle_resume_result(co, status);

        if (status == LUA_YIELD) {
            // 第三次恢复
            status = lua_resume(co, NULL, 0, &nres);
            printf("After third resume: %d\n", status);
            handle_resume_result(co, status);
        }
    }

    lua_close(L);
    return 0;
}