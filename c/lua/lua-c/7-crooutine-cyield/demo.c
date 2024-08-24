#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdio.h>

static int countdown_k(lua_State *L, int status, lua_KContext ctx);

// 交给协程的函数（最终在栈顶返回给Lua的）
static int countdown(lua_State *L) {
    int arg = lua_tointeger(L, lua_upvalueindex(1));
    printf("C function called with: %d\n", arg);

    if (arg > 0) {
        lua_pushinteger(L, arg - 1);

        // 这个函数用于替换栈中指定索引处的值
        // 将栈顶的值弹出（此时是刚刚push的新的计算好的值），并用这个值替换指定索引处的值。
        lua_replace(L, lua_upvalueindex(1));
        lua_pushinteger(L, arg); // 将当前值推入栈中
        return lua_yieldk(L, 1, 0, countdown_k); // 暂停协程，将控制权返回给Lua，同时将指定数量的值，作为yield的结果返回
        // countdown_k 是  continuation 函数，当协程恢复时会被调用
    }

    return 0;
}

// 当协程恢复时，会调用这个 continuation 函数
// L: Lua 状态机指针
// status: 协程的状态（例如 LUA_YIELD 表示正常 yield）
// ctx: 上下文信息，是之前调用 lua_yieldk 时传入的值

// 当一个通过 lua_yieldk yield 的 C 函数被恢复时，Lua 不会直接从 yield 点继续执行。
// 相反，它调用这个 continuation 函数（countdown_k）。

// resume的时候，会传递lua状态机和yield时候保留的lua_KContext上下文，继续使用，然后只想countdown，继续之前被保留的updateValue的数据
static int countdown_k(lua_State *L, int status, lua_KContext ctx) { return countdown(L); }

// 当 Lua 代码调用 create_countdown(5) 时，实际上执行的是 C 函数 create_countdown：
static int create_countdown(lua_State *L) {
    int max = luaL_checkinteger(L, 1); // 获取传入的参数（这里是 5）。
    lua_pushinteger(L, max);           // 将这个值（5）压入 Lua 栈。

    // 在调用 lua_pushcclosure 之前，你需要将 upvalues 按顺序压入 Lua 栈。
    // lua_pushcclosure 会从栈顶开始，取指定数量的值作为 upvalues，这里传递了 1 个 upvalue，即 max。
    // 推送到栈顶，lua中调用该create_countdown(时，这个栈顶指定返回数量的值，即作为返回值，
    lua_pushcclosure(L, countdown, 1);
    return 1; // 返回 1，表示有一个返回值。
}

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushcfunction(L, create_countdown);
    lua_setglobal(L, "create_countdown"); // 包装为Lua函数，可以通过Lua代码调用 create_countdown

    // 运行Lua代码
    const char *lua_code = "local countdown = create_countdown(5)\n"
                           "local co = coroutine.create(countdown)\n"
                           "while true do\n"
                           "    local success, value = coroutine.resume(co)\n"
                           "    if not success then\n"
                           "        print('Error:', value)\n"
                           "        break\n"
                           "    end\n"
                           "    if value == nil then break end\n"
                           "    print('Lua received:', value)\n"
                           "end\n"
                           "print('Countdown finished')\n";

    if (luaL_dostring(L, lua_code) != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
    }

    lua_close(L);
    return 0;
}