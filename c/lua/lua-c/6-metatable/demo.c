#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

// 简单的元方法：打印消息
static int l_tostring(lua_State *L) {
    lua_pushstring(L, "Hello from metatable!");
    return 1;
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // 创建主表
    lua_newtable(L);

    // 创建元表
    lua_newtable(L);

    // 设置 __tostring 元方法
    lua_pushcfunction(L, l_tostring);
    lua_setfield(L, -2, "__tostring");

    // 将元表设置为主表的元表
    lua_setmetatable(L, -2);

    // 将主表设置为全局变量 "obj"
    lua_setglobal(L, "obj");

    // 执行Lua代码来测试
    const char *testcode = "local mt = getmetatable(obj)\n"
                           "if mt and mt.__tostring then\n"
                           "    print(mt.__tostring(obj))\n"
                           "else\n"
                           "    print('Metatable or __tostring not set correctly')\n"
                           "end\n";

    if (luaL_dostring(L, testcode) != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
    }

    lua_close(L);
    return 0;
}