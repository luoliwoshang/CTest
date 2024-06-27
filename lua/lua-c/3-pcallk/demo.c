#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>

int dummy_continuation(lua_State *L, int status, lua_KContext ctx) {
    printf("continuation");
    return 0; 
}

int main(void) {
    lua_State *L = luaL_newstate();  
    luaL_openlibs(L);            

    const char *code = "print('Hello from Lua!')";

    if (luaL_loadstring(L, code) != LUA_OK) {
        fprintf(stderr, "Failed to load Lua code: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    if (lua_pcallk(L, 0, 0, 0, 0, dummy_continuation) != LUA_OK) {
        fprintf(stderr, "Failed to run Lua code: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    lua_close(L);
    return 0;
}
