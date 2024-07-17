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


void startCoroutine(lua_State *L) {
    lua_getglobal(L, "coro_func");  // 假设协程函数名为 coro_func

    lua_State *co = lua_newthread(L);  // 创建一个新的协程
    lua_pushvalue(L, -2);  // 复制函数到栈顶
    lua_xmove(L, co, 1);  // 把函数从主线程移动到协程

	printStack(L, "主线程栈信息");
	printStack(co, "协程栈信息");
    printf("-------\n");

    int nresults;
    int status = lua_resume(co, NULL, 0, &nresults);  // 启动协程

	printStack(L, "主线程栈信息");
	printStack(co, "协程栈信息");

    if (status == LUA_YIELD || status == LUA_OK) {
        printf("Coroutine returned %d results.\n", nresults);
        for (int i = 1; i <= nresults; ++i) {
            if (lua_isnumber(co, -i)) {
                printf("Result %d: %f\n", i, lua_tonumber(co, -i));
            }
            // 检查并处理其他可能的返回类型...
        }
    }
}

int main() {
    lua_State *L = luaL_newstate();  // 创建一个新的 Lua 状态
    luaL_openlibs(L);  // 打开所有 Lua 标准库

    // 在这里加载你的 Lua 脚本或直接设置 Lua 代码
    luaL_dostring(L, "coro_func = function() for i=1,5 do coroutine.yield(i) end end");

    startCoroutine(L);  // 启动协程

    lua_close(L);  // 关闭 Lua 状态
    return 0;
}
