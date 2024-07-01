#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

void test_lua_types(lua_State *L) {
    // Push various types onto the Lua stack
    lua_pushnil(L);
    lua_pushboolean(L, 1);
    lua_pushnumber(L, 42.0);
    lua_pushstring(L, "Hello, Lua!");
    lua_newtable(L);
    lua_pushcfunction(L, NULL);
    lua_pushlightuserdata(L, NULL);
    lua_newthread(L);

    // Test each type
    printf("lua_isfunction (index 6): %d\n", lua_isfunction(L, 6)); // true
    printf("lua_istable (index 5): %d\n", lua_istable(L, 5)); // true
    printf("lua_islightuserdata (index 7): %d\n", lua_islightuserdata(L, 7)); // true
    printf("lua_isnil (index 1): %d\n", lua_isnil(L, 1)); // true
    printf("lua_isboolean (index 2): %d\n", lua_isboolean(L, 2)); // true
    printf("lua_isthread (index 8): %d\n", lua_isthread(L, 8)); // true
    printf("lua_isnone (index -1): %d\n", lua_isnone(L, -1)); // true
    printf("lua_isnoneornil (index 1): %d\n", lua_isnoneornil(L, 1)); // true
    printf("lua_isnoneornil (index -1): %d\n", lua_isnoneornil(L, -9)); // true
}

int main(void) {
    // Create a new Lua state
    lua_State *L = luaL_newstate();

    // Open standard libraries
    luaL_openlibs(L);

    // Run the tests
    test_lua_types(L);

    // Close the Lua state
    lua_close(L);
    return 0;
}
