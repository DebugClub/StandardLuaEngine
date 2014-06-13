#include "stdafx.h"
#include "_LuaRefBase.h"
#include "lua.hpp"
#include "_LuaStack.h"
#include "_SLEHelper.h"

using namespace sle;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int s_LuaRefCount[8192] = { 0 };

_LuaRefBase::_LuaRefBase()
{
	m_pLuaState = NULL;
	m_nRefIndex = 0;
	m_nStackIndex = 0;
}

_LuaRefBase::_LuaRefBase(lua_State* L)
{
	m_pLuaState = L;
	m_nRefIndex = 0;
	m_nStackIndex = 0;
}

_LuaRefBase::_LuaRefBase(const _LuaRefBase& rhl)
{
	m_pLuaState = rhl.m_pLuaState;
	m_nRefIndex = rhl.m_nRefIndex;
	m_nStackIndex = 0;
	if (m_nRefIndex)
	{
		s_LuaRefCount[m_nRefIndex]++;
	}
}

_LuaRefBase& _LuaRefBase::operator=(const _LuaRefBase& rhl)
{
	_ResetRefrence();
	m_pLuaState = rhl.m_pLuaState;
	m_nRefIndex = rhl.m_nRefIndex;
	m_nStackIndex = 0;
	if (m_nRefIndex)
	{
		s_LuaRefCount[m_nRefIndex]++;
	}
	return *this;
}

_LuaRefBase::~_LuaRefBase()
{
	_ResetRefrence();
}

bool _LuaRefBase::valid() const
{
	return m_nRefIndex != 0;
}

void _LuaRefBase::_ResetRefrence()
{
	if (m_nRefIndex)
	{
		s_LuaRefCount[m_nRefIndex]--;
		if (s_LuaRefCount[m_nRefIndex] == 0)
		{
			luaL_unref(m_pLuaState, LUA_REGISTRYINDEX, m_nRefIndex);
		}
		m_nRefIndex = 0;
	}
}

bool _LuaRefBase::_SetRefrence(const char* pszLuaAddrDesc)
{
	_ResetRefrence();
	if (!GetLuaValue(m_pLuaState, pszLuaAddrDesc))
		return false;

	m_nRefIndex = luaL_ref(m_pLuaState, LUA_REGISTRYINDEX);
	// value is nil
	if (m_nRefIndex < 0)
	{
		lua_pushfstring(m_pLuaState, "%s is a nil value", pszLuaAddrDesc);
		_SLE_OnError(m_pLuaState);
		return false;
	}
	s_LuaRefCount[m_nRefIndex]++;
	return true;
}

bool _LuaRefBase::_SetRefrence(int nIndex)
{
	_ResetRefrence();
	lua_pushvalue(m_pLuaState, nIndex);
	m_nRefIndex = luaL_ref(m_pLuaState, LUA_REGISTRYINDEX);
	s_LuaRefCount[m_nRefIndex]++;
	return true;
}

void _LuaRefBase::_Push() const
{
	assert(m_nStackIndex == 0);
	lua_rawgeti(m_pLuaState, LUA_REGISTRYINDEX, m_nRefIndex);
	((_LuaRefBase*)this)->m_nStackIndex = lua_gettop(m_pLuaState);
}

void _LuaRefBase::_Pop() const
{
	assert(m_nStackIndex != 0);
	lua_remove(m_pLuaState, m_nStackIndex);
	((_LuaRefBase*)this)->m_nStackIndex = 0;
}

lua_State* _LuaRefBase::GetLuaState() const
{
	return m_pLuaState;
}
