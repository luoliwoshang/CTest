#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>

int main(void) {
    lua_State *L = luaL_newstate();  // 创建一个新的 Lua 环境
    luaL_openlibs(L);  // 打开 Lua 库

    // 加载并运行 Lua 脚本
    if (luaL_dofile(L, "double.lua") != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // 将函数压入栈顶
    lua_getglobal(L, "double_number");

    // 压入函数的参数
    lua_pushnumber(L, 10);  // 假设我们要加倍的数字是10

    // 调用函数，1个参数，期待1个返回值
    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        fprintf(stderr, "Error running function: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // 获取返回值
    int result = (int)lua_tonumber(L, -1);
    printf("The result is: %d\n", result);

    // 清理栈
    lua_pop(L, 1);

    // 关闭 Lua 环境
    lua_close(L);
    return 0;
}
