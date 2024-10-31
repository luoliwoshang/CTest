#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>

static void *alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
    const char *udContent = *(const char **)ud;
    printf("ud: %s; *ptr: %p; osize: %zu; nsize: %zu\n", udContent, ptr, osize, nsize);
    if (nsize == 0) {
        free(ptr);
        return NULL;
    } else
        return realloc(ptr, nsize);
}

static void *newalloc(void *ud, void *ptr, size_t osize, size_t nsize) {
    const char *udContent = *(const char **)ud;
    printf("新的分配函数 ud: %s; *ptr: %p; osize: %zu; nsize: %zu\n", udContent, ptr, osize, nsize);
    if (nsize == 0) {
        free(ptr);
        return NULL;
    } else
        return realloc(ptr, nsize);
}

void allocationFunction() {
    /* 常规的创建 lua_State */
    /* lua_State *L = luaL_newstate(); */
    printf("------------- 自定义内存分配函数 -------------\n");
    /* 自定义内存分配数据 */
    const char *ud = "ud test message.";
    lua_State *L = lua_newstate(alloc, &ud);

    printf("------------- 获取 Lua 当前的内存分配函数 -------------\n");
    /* lua_getallocf 第二个参数会返回之前设置的 自定义内存分配数据 */
    void *userData = NULL;
    lua_Alloc allocFunc = lua_getallocf(L, &userData);
    const char *udContent = *(const char **)userData;
    printf("%p %s\n", (void *)allocFunc, udContent);

    printf("------------- 设置 Lua 的内存分配函数 -------------\n");
    const char *nud = "new ud test message.";
    lua_setallocf(L, newalloc, &nud);

    printf("关闭 lua_State\n");
    lua_close(L);
}

int main() {
    allocationFunction();
    return 0;
}