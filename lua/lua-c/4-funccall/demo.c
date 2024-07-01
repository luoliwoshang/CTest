#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>

int main(void) {
    lua_State *L = luaL_newstate();  // 创建 Lua 状态机
    luaL_openlibs(L);                // 打开 Lua 标准库

    // 定义一个 Lua 函数 doubleNumber
    const char *code = "function doubleNumber(x) return x * 2 end";
    
    // 加载 Lua 代码字符串
    if (luaL_loadstring(L, code) != LUA_OK) {
        fprintf(stderr, "Error loading Lua string: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }
    
    // 执行 Lua 代码
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        fprintf(stderr, "Error executing Lua code: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // 从全局环境中获取 doubleNumber 函数并压入栈顶
    lua_getglobal(L, "doubleNumber");
    
    // 压入函数的参数
    lua_pushnumber(L, 10);  // 压入数字 10 作为参数

    // 调用函数，1个参数，期待1个返回值
    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        fprintf(stderr, "Error calling function: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }
    // 从栈顶读取返回值
    if (lua_isnumber(L, -1)) {
        int result = lua_tointeger(L, -1);
        printf("Result: %d\n", result);
    } else {
        fprintf(stderr, "Expected a number as return value.\n");
    }

    // 清理栈
    lua_pop(L, 1);

    // 关闭 Lua 状态机
    lua_close(L);
    return 0;
}
