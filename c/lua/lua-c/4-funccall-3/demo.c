#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
#include <stdarg.h>

void push_formatted_string(lua_State *L, const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    lua_pushvfstring(L, fmt, argp);
    va_end(argp);
}

int main(void) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // 定义 Lua 函数
    luaL_dostring(L,
        "function processStrings(a, b, c) "
        "    print('Received string a: ' .. a) "
        "    print('Received string b (binary data): ', b) "
        "    print('Received string c (formatted): ' .. c) "
        "    return a .. b .. c "
        "end");

    // 获取 Lua 中的函数
    lua_getglobal(L, "processStrings");

    // 压入函数的参数
    lua_pushstring(L, "Hello, World!");        // 常规字符串
    lua_pushlstring(L, "Hello Lua In LLGO", 17);  // 包含 null 的字符串
    push_formatted_string(L, "Hello %s In %d", "LLGO",2024);  // 格式化字符串

    // 调用函数，3个参数，期待1个返回值
    if (lua_pcall(L, 3, 1, 0) != LUA_OK) {
        fprintf(stderr, "Error calling function: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // 从栈顶读取返回值
    const char *result = lua_tostring(L, -1);
    printf("Combined result: %s\n", result);  // 打印组合结果

    // 清理栈
    lua_pop(L, 1);

    // 关闭 Lua 状态机
    lua_close(L);
    return 0;
}
