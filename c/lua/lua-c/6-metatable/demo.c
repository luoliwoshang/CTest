#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

static int l_tostring(lua_State *L) {
    lua_pushstring(L, "Hello from metatable!");
    return 1;
}

void print_stack(lua_State *L, const char *message) {
    int top = lua_gettop(L);
    printf("%s - Stack size: %d\n", message, top);
    for (int i = 1; i <= top; i++) {
        int t = lua_type(L, i);
        switch (t) {
        case LUA_TSTRING:
            printf("  %d: string: %s\n", i, lua_tostring(L, i));
            break;
        case LUA_TBOOLEAN:
            printf("  %d: boolean: %s\n", i, lua_toboolean(L, i) ? "true" : "false");
            break;
        case LUA_TNUMBER:
            printf("  %d: number: %f\n", i, lua_tonumber(L, i));
            break;
        default:
            printf("  %d: %s\n", i, lua_typename(L, t));
            break;
        }
    }
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // 创建主表
    lua_newtable(L);
    print_stack(L, "After creating main table");

    // 创建元表
    lua_newtable(L);
    print_stack(L, "After creating metatable");

    // 设置 __tostring 元方法
    lua_pushcfunction(L, l_tostring);
    print_stack(L, "After push __tostring");
    lua_setfield(L, -2, "__tostring");
    print_stack(L, "After setting __tostring");

    // 将元表设置为主表的元表
    if (lua_setmetatable(L, -2) == 0) {
        printf("Failed to set metatable\n");
    }
    print_stack(L, "After setting metatable");

    // 将主表设置为全局变量 "obj"
    lua_setglobal(L, "obj");
    print_stack(L, "After setting global obj");

    // 执行Lua代码来测试
    const char *testcode = "if obj == nil then\n"
                           "    print('obj is not defined')\n"
                           "else\n"
                           "    local mt = getmetatable(obj)\n"
                           "    if not mt then\n"
                           "        print('Metatable not set')\n"
                           "    elseif not mt.__tostring then\n"
                           "        print('__tostring not set in metatable')\n"
                           "    else\n"
                           "        print(mt.__tostring(obj))\n"
                           "    end\n"
                           "end\n";

    if (luaL_dostring(L, testcode) != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
    }

    lua_close(L);
    return 0;
}