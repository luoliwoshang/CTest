#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>

int main(void) {
    lua_State *L = luaL_newstate();  // 创建一个新的 Lua 状态机
    luaL_openlibs(L);                // 打开 Lua 标准库

    // 定义 Lua 代码：打印一条消息
    const char *code = "print('Hello from Lua!')";

    // 加载并编译 Lua 代码
    if (luaL_loadstring(L, code) != LUA_OK) {
        fprintf(stderr, "Error loading Lua string: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // 执行编译后的 Lua 代码
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        fprintf(stderr, "Error executing Lua code: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // 关闭 Lua 状态机
    lua_close(L);
    return 0;
}
