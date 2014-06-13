#include "stdafx.h"
#include "luatable.h"
#include "lua.hpp"
#include "_LuaStack.h"
#include "_LuaValueHolder.h"

#define aux_getn(L,n)	(luaL_checktype(L, n, LUA_TTABLE), luaL_len(L, n))
using namespace sle;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

luatable::luatable()
{
	m_pLuaValueHolder = new _LuaValueHolder();
}

luatable::luatable(lua_State* L) : _LuaRefBase(L)
{
	m_pLuaValueHolder = new _LuaValueHolder();
}

luatable::luatable(const luatable& rhl) : _LuaRefBase(rhl)
{
	m_pLuaValueHolder = new _LuaValueHolder();
}

luatable::luatable(const _LuaRefBase& rhl) : _LuaRefBase(rhl)
{
	m_pLuaValueHolder = new _LuaValueHolder();
}

luatable& luatable::operator=(const luatable& rhl)
{
	_LuaRefBase::operator =(rhl);
	return *this;
}

luatable::~luatable()
{
	DELETE_POINTER(m_pLuaValueHolder);
}


_LuaValueHolder& luatable::getfield(int nIdx)
{
	_Push();
	lua_pushinteger(m_pLuaState, nIdx);
	lua_gettable(m_pLuaState, -2);
	m_pLuaValueHolder->_SetStackValue(m_pLuaState, -1);
	_Pop();
	return *m_pLuaValueHolder;
}

_LuaValueHolder& luatable::getfield(const char* pszKey)
{
	_Push();
	lua_pushstring(m_pLuaState, pszKey);
	lua_gettable(m_pLuaState, -2);
	m_pLuaValueHolder->_SetStackValue(m_pLuaState, -1);
	_Pop();
	return *m_pLuaValueHolder;
}

void luatable::setmetatable(luatable& tbMetatable)
{
	_Push();
	tbMetatable._Push();
	lua_setmetatable(m_pLuaState, -2);
	_Pop();
}

lua_Integer luatable::_GetFirstEmptyPos()
{
	_Push();
	lua_Integer pos = aux_getn(m_pLuaState, -1) + 1;
	_Pop();
	return pos;
}
