#include "stdafx.h"
#include "_LuaMultiValueHolder.h"
#include "_SLEGlobal.h"
#include "lua.hpp"

using namespace sle;

_LuaMultiValueHolder::_LuaMultiValueHolder()
{
	m_nHolderCount = 0;
}

void _LuaMultiValueHolder::_RefreshFromStack(lua_State* L, int nCount)
{
	if (nCount == LUA_MULTRET)
		nCount = lua_gettop(L);

	m_nHolderCount = 0;

	for (int i = -nCount; i < 0; ++i)
	{
		_LuaValueHolder &holder = m_Holders[m_nHolderCount++];
		holder._SetStackValue(L, i);
		lua_remove(L, i);
	}
}

const _LuaValueHolder& _LuaMultiValueHolder::operator[](int nIdx) const
{
	assert(nIdx < SLE_MAX_LUA_MULTIVALUEHOLDER);
	assert(nIdx < m_nHolderCount);
	const _LuaValueHolder& holder = m_Holders[nIdx];
	return holder;
}

_LuaMultiValueHolder::operator int() const
{
	return (*this)[0];
}

_LuaMultiValueHolder::operator double() const
{
	return (*this)[0];
}

_LuaMultiValueHolder::operator bool() const
{
	return (*this)[0];
}

_LuaMultiValueHolder::operator const char*() const
{
	return (*this)[0];
}

_LuaMultiValueHolder::operator void*() const
{
	return (*this)[0];
}
