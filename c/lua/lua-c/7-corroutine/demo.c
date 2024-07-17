#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>

#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

void printStack(lua_State *L, const char *msg) {
    int top = lua_gettop(L);  // 获取栈顶索引
    printf("%s\n", msg);
    printf("Stack size: %d\n", top);
    for (int i = 1; i <= top; i++) {  // 栈索引从1开始
        int t = lua_type(L, i);
        printf("%d: %s - ", i, lua_typename(L, t));
        switch (t) {
            case LUA_TBOOLEAN:
                printf("%s\n", lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:
                if (lua_isinteger(L, i)) {
                    printf("%lld\n", lua_tointeger(L, i));
                } else {
                    printf("%g\n", lua_tonumber(L, i));
                }
                break;
            case LUA_TSTRING:
                printf("%s\n", lua_tostring(L, i));
                break;
            default:
                printf("%s\n", lua_typename(L, t));  // 输出类型名称
                break;
        }
    }
}


int main() {
    // 初始化 Lua 环境
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // 加载并执行 Lua 脚本，定义一个 Lua 函数作为协程的主体
    const char *lua_code = 
        "function coro_func() "
        "   for i = 1, 5 do "
        "       coroutine.yield(i) "
        "   end "
        "end";
    luaL_loadstring(L, lua_code);
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        fprintf(stderr, "Error loading Lua script: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return -1;
    }

    // 获取前面定义的函数
    lua_getglobal(L, "coro_func");

    // 创建一个协程
    lua_State *co = lua_newthread(L);
    lua_pushvalue(L, -2);  // 将 coro_func 函数复制到栈顶
    lua_xmove(L, co, 1);  // 将函数从主线程的栈移动到协程的栈
    printStack(L, "主程序的栈");
    printStack(co, "携程的栈");

    // 运行协程
    int status, nresults;
    while ((status = lua_resume(co, NULL, 0, &nresults)) == LUA_YIELD) {
        int yield_value = lua_tointeger(co, -1);  // 获取协程 yield 的值
        printf("Yield value: %d\n", yield_value);
        lua_pop(co, 1);  // 清理栈
    }
    if (status != LUA_OK) {
        fprintf(stderr, "Error running coroutine: %s\n", lua_tostring(co, -1));
    }

    // 关闭 Lua 环境
    lua_close(L);
    return 0;
}
