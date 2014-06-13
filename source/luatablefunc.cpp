#include "stdafx.h"
#include "luatablefunc.h"
#include "_LuaStack.h"
#include "luaevn.h"

using namespace sle;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

luatablefunc::luatablefunc() : luafunc()
{
}

luatablefunc::luatablefunc(lua_State* L) : luafunc(L), m_LuaTable(L)
{
}

luatablefunc::luatablefunc(const luatablefunc& rhl) : luafunc(rhl), m_LuaTable(rhl.m_LuaTable)
{
}

luatablefunc::~luatablefunc()
{
}

luatablefunc& luatablefunc::operator=(const luatablefunc& rhl)
{
	luafunc::operator=(rhl);
	m_LuaTable = rhl.m_LuaTable;
	return *this;
}

void luatablefunc::_SetTableRefrence(const char* pszLuaTableAddrDesc)
{
	m_LuaTable._SetRefrence(pszLuaTableAddrDesc);
}

_LuaMultiValueHolder& luatablefunc::_Call(int nArgs)
{
	m_LuaTable._Push();
	lua_insert(m_pLuaState, -nArgs - 1); // 把table参数插入所有参数之前
	return luafunc::_Call(nArgs + 1);
}
