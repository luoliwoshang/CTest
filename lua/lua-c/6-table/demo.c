#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>

// 打印表中的内容
void print_table(lua_State *L) {
    lua_pushnil(L);  // 初始键 nil
    while (lua_next(L, -2) != 0) {
        printf("%s - %s\n", lua_tostring(L, -2), lua_tostring(L, -1));
        lua_pop(L, 1);  // 弹出值，留下键用于下一次迭代
    }
}

int main(void) {
    lua_State *L = luaL_newstate();  // 创建一个新的 Lua 状态
    luaL_openlibs(L);  // 打开 Lua 标准库

    lua_newtable(L);  // 在栈顶创建一个新表

    // 向表中插入数据
    lua_pushstring(L, "name");
    lua_pushstring(L, "John");
    lua_settable(L, -3);

    lua_pushstring(L, "age");
    lua_pushinteger(L, 30);
    lua_settable(L, -3);

    // 直接使用 lua_setfield 来简化
    lua_pushstring(L, "John Doe");
    lua_setfield(L, -2, "fullname");

    // 使用 lua_getfield 获取表中的数据
    lua_getfield(L, -1, "name");
    printf("Name: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);  // 弹出 'name' 的值

    // 使用 lua_gettable 获取表中的数据
    lua_pushstring(L, "age");
    lua_gettable(L, -2);
    printf("Age: %d\n", (int)lua_tointeger(L, -1));
    lua_pop(L, 1);  // 弹出 'age' 的值

    // 遍历表中的所有键值对
    printf("All entries in the table:\n");
    print_table(L);

    // 清理栈
    lua_pop(L, 1);  // 弹出表

    lua_close(L);  // 关闭 Lua 状态机
    return 0;
}
