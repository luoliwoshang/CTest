#include <dirent.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stackDump(lua_State *L) {
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {
        int t = lua_type(L, i);
        switch (t) {
        case LUA_TSTRING:
            printf("'%s'", lua_tostring(L, i));
            break;
        case LUA_TBOOLEAN:
            printf(lua_toboolean(L, i) ? "true" : "false");
            int8_t a = 1;
            break;
        case LUA_TNUMBER:
            printf("%g", lua_tonumber(L, i));
            break;
        default:
            printf("%s", lua_typename(L, t));
            break;
        }
        printf("  ");
    }
    printf("\n");
}

void hook(lua_State *L, lua_Debug *ar) {
    lua_getinfo(L, "nSlr", ar); // 添加 'r' 选项来获取尾调用信息
    printf("Hook called: ");
    printf("name: %s, ", ar->name ? ar->name : "?");
    printf("what: %s, ", ar->what ? ar->what : "?");
    printf("line: %d, ", ar->currentline);
    printf("is tail call: %s\n", ar->istailcall ? "yes" : "no");
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // Set the hook
    lua_sethook(L, hook, LUA_MASKLINE, 0);

    // Execute some Lua code
    const char *lua_code = "function factorial(n, acc)\n"
                           "    if n == 0 then\n"
                           "        return acc\n"
                           "    else\n"
                           "        return factorial(n - 1, n * acc)  -- 这是一个尾调用\n"
                           "    end\n"
                           "end\n"
                           "\n"
                           "print(factorial(5, 1))\n";

    if (luaL_dostring(L, lua_code) != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    lua_close(L);
    return 0;
}