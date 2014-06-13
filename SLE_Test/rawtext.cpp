#include "stdafx.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

void rawtest()
{
	lua_State* L = luaL_newstate();
	int n = 0;
	lua_pushinteger(L, 10);
	lua_setglobal(L, "a");
	lua_getglobal(L, "a");

	n = lua_gettop(L);
	int idx = luaL_ref(L, LUA_REGISTRYINDEX);
	n = lua_gettop(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, idx);
	idx = luaL_ref(L, LUA_REGISTRYINDEX);
}
