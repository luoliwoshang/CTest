#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

// Set an integer value in the extra space
void set_extra_int(lua_State *L, int value) {
    int *extra = (int *)lua_getextraspace(L);
    *extra = value;

    printf("Setting value:\n");
    // printf("lua_State address: %p\n", (void *)L);
    // printf("Extra space address: %p\n", (void *)extra);
    printf("Address difference (bytes): %td\n", (char *)L - (char *)extra);
    printf("LUA_EXTRASPACE value: %zu\n", LUA_EXTRASPACE);
    printf("Set value: %d\n\n", value);
}

// Get the integer value from the extra space
int get_extra_int(lua_State *L) {
    int *extra = (int *)lua_getextraspace(L);
    return *extra;
}

// Lua C function: print the stored integer
static int l_print_extra_int(lua_State *L) {
    int value = get_extra_int(L);
    lua_pushfstring(L, "Stored integer is: %d", value);
    return 1;
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // Set an integer in the extra space
    set_extra_int(L, 42);

    // Register Lua function
    lua_pushcfunction(L, l_print_extra_int);
    lua_setglobal(L, "print_extra_int");

    // Execute Lua code
    if (luaL_dostring(L, "print(print_extra_int())") != LUA_OK) {
        fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
    }

    lua_close(L);
    return 0;
}