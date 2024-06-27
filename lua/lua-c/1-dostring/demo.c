#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>

int main(void) {
    lua_State *L;

    // 创建一个新的 Lua 状态机
    L = luaL_newstate();

    // 打开所有 Lua 标准库
    luaL_openlibs(L);

    // 运行一段 Lua 代码
    if (luaL_dostring(L, "print('Hello, Lua!')")) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);  // 从栈中弹出错误信息
    }
    // 关闭 Lua 状态机
    lua_close(L);

    return 0;
}
