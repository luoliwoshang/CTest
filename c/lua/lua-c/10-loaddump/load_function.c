#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <string.h>

static const char *reader(lua_State *L, void *data, size_t *size) {
    static char buffer[4096];
    FILE *file = (FILE *)data;
    *size = fread(buffer, 1, sizeof(buffer), file);
    return (*size > 0) ? buffer : NULL;
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    FILE *file = fopen("greet.luac", "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for reading\n");
        lua_close(L);
        return 1;
    }

    // Load the function
    if (lua_load(L, reader, file, "greet", NULL) != LUA_OK) {
        fprintf(stderr, "Failed to load function: %s\n", lua_tostring(L, -1));
        fclose(file);
        lua_close(L);
        return 1;
    }

    fclose(file);

    // Print stack info before calling
    printf("Stack size before call: %d\n", lua_gettop(L));
    printf("Top element type before call: %s\n", lua_typename(L, lua_type(L, -1)));

    // Call the function
    lua_pushstring(L, "World");
    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        fprintf(stderr, "Failed to call function: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // Print stack info after calling
    printf("Stack size after call: %d\n", lua_gettop(L));
    printf("Top element type after call: %s\n", lua_typename(L, lua_type(L, -1)));

    if (lua_isstring(L, -1)) {
        printf("Result: %s\n", lua_tostring(L, -1));
    } else {
        printf("Unexpected result type: %s\n", lua_typename(L, lua_type(L, -1)));
    }

    lua_close(L);
    return 0;
}