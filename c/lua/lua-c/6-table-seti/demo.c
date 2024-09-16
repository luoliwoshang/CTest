#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // Create a table and populate it
    lua_newtable(L);
    lua_pushstring(L, "Hello");
    lua_setfield(L, -2, "greeting");
    lua_pushinteger(L, 42);
    lua_seti(L, -2, 1);
    lua_pushstring(L, "World");
    lua_rawseti(L, -2, 2);

    // Demonstration of lua_geti
    printf("lua_geti demo:\n");
    lua_geti(L, -1, 1); // Get value at index 1
    printf("Value at index 1: %lld\n", lua_tointeger(L, -1));
    lua_pop(L, 1);

    // Demonstration of lua_rawget
    printf("\nlua_rawget demo:\n");
    lua_pushstring(L, "greeting");
    lua_rawget(L, -2);
    printf("Value of 'greeting': %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);

    // Demonstration of lua_rawgeti
    printf("\nlua_rawgeti demo:\n");
    lua_rawgeti(L, -1, 2); // Get value at index 2
    printf("Value at index 2: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);

    // Demonstration of lua_rawgetp
    printf("\nlua_rawgetp demo:\n");
    static const char *key = "pointer_key";
    lua_pushstring(L, "Pointer value");
    lua_rawsetp(L, -2, key);

    lua_rawgetp(L, -1, key);
    printf("Value associated with pointer: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);

    lua_close(L);
    return 0;
}