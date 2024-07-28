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
                           "end\n"
                           "\n"
                           "-- 使用Lua的debug.getmetatable()函数检查元表\n"
                           "local has_metatable = debug.getmetatable(obj) ~= nil\n"
                           "print('Has metatable (Lua):', has_metatable)\n";

    if (luaL_dostring(L, testcode) != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
    }

    // 使用 lua_getmetatable 来验证元表
    lua_getglobal(L, "obj");
    if (lua_getmetatable(L, -1)) {
        printf("Metatable found using lua_getmetatable\n");
        // 检查 __tostring 是否存在
        lua_getfield(L, -1, "__tostring");
        if (lua_isfunction(L, -1)) {
            printf("__tostring function found in metatable\n");

            // 使用 lua_iscfunction 检查是否是 C 函数
            if (lua_iscfunction(L, -1)) {
                printf("__tostring is a C function\n");

                // 使用 lua_tocfunction 获取函数指针
                lua_CFunction cfunc = lua_tocfunction(L, -1);
                if (cfunc != NULL) {
                    printf("Successfully retrieved __tostring C function pointer\n");

                    // 调用获取到的 C 函数
                    lua_pushcfunction(L, cfunc);
                    if (lua_pcall(L, 0, 1, 0) == LUA_OK) {
                        const char *result = lua_tostring(L, -1);
                        printf("Result of calling __tostring: %s\n", result);
                    } else {
                        printf("Error calling __tostring function\n");
                    }
                    lua_pop(L, 1); // 弹出函数调用的结果
                } else {
                    printf("Failed to retrieve __tostring C function pointer\n");
                }
            } else {
                printf("__tostring is a Lua function\n");
            }
        } else {
            printf("__tostring function not found in metatable\n");
        }
        lua_pop(L, 1); // 弹出 __tostring
    } else {
        printf("No metatable found using lua_getmetatable\n");
    }
    lua_pop(L, 2); // 弹出元表和 obj

    lua_close(L);
    return 0;
}