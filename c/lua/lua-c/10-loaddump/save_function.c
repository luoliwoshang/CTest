#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <string.h>

static const char *luaCode = "function greet(name)\n"
                             "    return 'Hello, ' .. name .. '!'\n"
                             "end\n"
                             "return greet"; // 注意这里的改变

static int writer(lua_State *L, const void *p, size_t sz, void *ud) {
    FILE *file = (FILE *)ud;
    return (fwrite(p, sz, 1, file) != 1) && (sz != 0);
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // Load the Lua code
    if (luaL_loadstring(L, luaCode) != LUA_OK) {
        fprintf(stderr, "Failed to load Lua code: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // Execute the chunk to get the function
    if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
        fprintf(stderr, "Failed to execute Lua code: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // Check if we got a function
    if (!lua_isfunction(L, -1)) {
        fprintf(stderr, "Expected a function, but got %s\n", lua_typename(L, lua_type(L, -1)));
        lua_close(L);
        return 1;
    }

    // Dump the function
    FILE *file = fopen("greet.luac", "wb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for writing\n");
        lua_close(L);
        return 1;
    }

    if (lua_dump(L, writer, file, 0) != 0) {
        fprintf(stderr, "Failed to dump Lua function\n");
        fclose(file);
        lua_close(L);
        return 1;
    }

    fclose(file);
    printf("Function saved to greet.luac\n");

    lua_close(L);
    return 0;
}