#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>

static int trigger_lua_error(lua_State *L) {
    lua_pushstring(L, "This is an error triggered by lua_error");
    return lua_error(L);
}

static int trigger_luaL_error(lua_State *L) {
    return luaL_error(L, "This is an error triggered by luaL_error: %d", 42);
}

static int panic(lua_State *L) {
    const char *msg = lua_tostring(L, -1);
    fprintf(stderr, "PANIC: %s\n", msg);
    exit(EXIT_FAILURE);
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_atpanic(L, panic);

    lua_pushcfunction(L, trigger_lua_error);
    lua_setglobal(L, "trigger_lua_error");
    lua_pushcfunction(L, trigger_luaL_error);
    lua_setglobal(L, "trigger_luaL_error");

    printf("1. lua_error (protected):\n");
    lua_getglobal(L, "trigger_lua_error");
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    printf("\n2. luaL_error (protected):\n");
    lua_getglobal(L, "trigger_luaL_error");
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    printf("\n3. Unprotected call (panic):\n");
    lua_getglobal(L, "trigger_lua_error");
    lua_call(L, 0, 0);

    lua_close(L);
    return 0;
}