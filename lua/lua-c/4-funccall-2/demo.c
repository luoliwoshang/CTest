#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>

int main(void) {
    lua_State *L = luaL_newstate();  // 创建 Lua 状态机
    luaL_openlibs(L);                // 打开 Lua 标准库

    // 定义 Lua 函数
    const char *lua_code =
        "function combineParams(int, num, str) "
        "    return 'Combined Result: ' .. str .. ' ' .. tostring(int) .. ' ' .. tostring(num) "
        "end";
    luaL_loadstring(L, lua_code);  // 加载 Lua 代码字符串
    lua_pcall(L, 0, 0, 0);         // 执行 Lua 代码以定义函数

    // 获取 Lua 中的函数
    lua_getglobal(L, "combineParams");
    printf("Stack size: %d\n", lua_gettop(L));
    // 压入函数的参数
    lua_pushinteger(L, 42);        // 压入一个整数
    printf("Stack size: %d\n", lua_gettop(L));
    lua_pushnumber(L, 3.14159);    // 压入一个浮点数
    printf("Stack size: %d\n", lua_gettop(L));
    lua_pushstring(L, "hello");    // 压入一个字符串
    printf("Stack size: %d\n", lua_gettop(L));

    // 调用函数，3个参数，期待1个返回值
    if (lua_pcall(L, 3, 1, 0) != LUA_OK) {
        fprintf(stderr, "Error calling function: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // 从栈顶读取返回值
    if (lua_isstring(L, -1)) {
        const char *result = lua_tostring(L, -1);
        printf("Result: %s\n", result);
    } else {
        fprintf(stderr, "Expected a string as return value.\n");
    }

    // 清理栈
    lua_pop(L, 1);

    // 关闭 Lua 状态机
    lua_close(L);
    return 0;
}
