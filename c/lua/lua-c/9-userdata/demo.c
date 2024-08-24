#include <lauxlib.h>
#include <lua.h>
#include <stdio.h>

typedef struct {
    int value;
} SimpleStruct;

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // Full userdata
    int *full_data = (int *)lua_newuserdata(L, sizeof(int));
    *full_data = 42;
    lua_setglobal(L, "full_data");

    // Light userdata
    SimpleStruct data = {43};
    lua_pushlightuserdata(L, &data);
    lua_setglobal(L, "light_data");

    // Run Lua code to access userdata
    const char *luaCode = "print('Full userdata:', full_data)\n"
                          "print('Light userdata:', light_data)\n";

    if (luaL_dostring(L, luaCode) != LUA_OK) {
        fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    // Access full userdata from C
    lua_getglobal(L, "full_data");
    if (lua_isuserdata(L, -1)) {
        int *ptr = (int *)lua_touserdata(L, -1);
        printf("Full userdata value accessed from C: %d\n", *ptr);
    }

    // Access light userdata from C
    lua_getglobal(L, "light_data");
    if (lua_islightuserdata(L, -1)) {
        SimpleStruct *ptr = (SimpleStruct *)lua_touserdata(L, -1);
        printf("Light userdata value accessed from C: %d\n", ptr->value);
    }

    lua_close(L);
    return 0;
}