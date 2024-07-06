#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

/* `__index` 元方法的实现 */
int index_meta(lua_State *L) {
    lua_pushstring(L, "This is a default value");
    return 1;  
}

int main(void) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // 创建一个新表
    lua_newtable(L);

    // 为表创建一个元表
    lua_newtable(L);

    lua_pushstring(L, "__index");
    lua_pushcfunction(L, index_meta);  
    lua_settable(L, -3); 

    // 将元表设置到之前创建的表上
    lua_setmetatable(L, -2);

    // 模拟访问不存在的键
    lua_pushstring(L, "testKey");
    lua_gettable(L, -2);  // 触发 __index 元方法
    printf("Result of access: %s\n", lua_tostring(L, -1));  // 输出返回的默认值

    lua_close(L);
    return 0;
}
