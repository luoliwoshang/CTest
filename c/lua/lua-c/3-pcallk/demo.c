#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

// 函数声明
static int counter_k(lua_State *L, int status, lua_KContext ctx);
static int counter(lua_State *L);
static int create_counter(lua_State *L);

// 计数器函数
static int counter(lua_State *L) {
    int current = lua_tointeger(L, lua_upvalueindex(1));
    int max = lua_tointeger(L, lua_upvalueindex(2));

    printf("Counter called: current = %d, max = %d\n", current, max);

    if (current < max) {
        lua_pushinteger(L, current + 1);
        lua_pushvalue(L, -1);
        lua_replace(L, lua_upvalueindex(1));

        printf("Yielding: %d\n", current + 1);
        return lua_yieldk(L, 1, (lua_KContext)max, &counter_k);
    }

    printf("Counter finished\n");
    return 0;
}

// Continuation 函数
static int counter_k(lua_State *L, int status, lua_KContext ctx) {
    printf("Counter_k called: status = %d, ctx = %ld\n", status, (long)ctx);

    if (status != LUA_YIELD) {
        return luaL_error(L, "unexpected status");
    }

    int max = (int)ctx;
    int current = lua_tointeger(L, lua_upvalueindex(1));

    printf("Counter_k: current = %d, max = %d\n", current, max);

    if (current < max) {
        lua_pushinteger(L, current + 1);
        lua_pushvalue(L, -1);
        lua_replace(L, lua_upvalueindex(1));

        printf("Yielding from counter_k: %d\n", current + 1);
        return lua_yieldk(L, 1, (lua_KContext)max, &counter_k);
    }

    printf("Counter_k finished\n");
    return 0;
}

// 创建计数器的函数
static int create_counter(lua_State *L) {
    int max = luaL_checkinteger(L, 1);
    printf("Creating counter with max = %d\n", max);
    lua_pushinteger(L, 0);
    lua_pushinteger(L, max);
    lua_pushcclosure(L, &counter, 2);
    return 1;
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushcfunction(L, create_counter);
    lua_setglobal(L, "create_counter");

    printf("Running Lua code...\n");
    if (luaL_dostring(L, "print('Lua code started')\n"
                         "local co = coroutine.create(function()\n"
                         "    print('Coroutine started')\n"
                         "    local counter = create_counter(5)\n"
                         "    print('Counter created')\n"
                         "    while true do\n"
                         "        print('Calling counter')\n"
                         "        local success, value = coroutine.resume(coroutine.create(counter))\n"
                         "        print('Call result:', success, value)\n"
                         "        if not success or value == nil then break end\n"
                         "    end\n"
                         "    print('Coroutine finished')\n"
                         "end)\n"
                         "print('Resuming main coroutine')\n"
                         "local success, msg = coroutine.resume(co)\n"
                         "if not success then\n"
                         "    print('Error in coroutine:', msg)\n"
                         "end\n"
                         "print('Lua code finished')\n") != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
    }

    lua_close(L);
    return 0;
}