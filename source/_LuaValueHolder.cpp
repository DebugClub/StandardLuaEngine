#include "stdafx.h"
#include "_LuaValueHolder.h"
#include "lua.hpp"
#include "string.h"
#include "_LuaStack.h"
#include "_LuaRefBase.h"
#include "luafunc.h"
#include "luatable.h"

using namespace sle;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

_LuaValueHolder::_LuaValueHolder()
{
	InitValue();
}

_LuaValueHolder::~_LuaValueHolder()
{
	ClearValue();
}

void _LuaValueHolder::InitValue()
{
	m_nType = LUA_TNONE;
	m_dValue = 0.0F;
	m_pszValue = NULL;
	m_pValue = NULL;
	m_pLuaValueRef = NULL;
}

void _LuaValueHolder::ClearValue()
{
	DELETE_POINTER(m_pLuaValueRef);
	InitValue();
}

int _LuaValueHolder::GetType()
{
	return m_nType;
}

_LuaValueHolder::operator int() const
{
	assert(m_nType == LUA_TNUMBER);
	return (int)(m_dValue);
}

_LuaValueHolder::operator double() const
{
	assert(m_nType == LUA_TNUMBER);
	return m_dValue;
}

_LuaValueHolder::operator bool() const
{
	assert(m_nType == LUA_TBOOLEAN || m_nType == LUA_TNUMBER);
	return (int)m_dValue != 0;
}

_LuaValueHolder::operator const char*() const
{
	assert(m_nType == LUA_TSTRING);
	return m_pszValue;
}

_LuaValueHolder::operator void*() const
{
	assert(m_nType == LUA_TUSERDATA || m_nType == LUA_TLIGHTUSERDATA);
	return (void*)m_pValue;
}

_LuaValueHolder::operator luafunc() const
{
	assert(m_nType == LUA_TFUNCTION);
	return luafunc(*m_pLuaValueRef);
}

_LuaValueHolder::operator luatable() const
{
	assert(m_nType == LUA_TTABLE);
	return luatable(*m_pLuaValueRef);
}

void _LuaValueHolder::_SetValue(int nValue)
{
	ClearValue();
	m_nType = LUA_TNUMBER;
	m_dValue = nValue;
}

void _LuaValueHolder::_SetValue(double dValue)
{
	ClearValue();
	m_nType = LUA_TNUMBER;
	m_dValue = dValue;
}

void _LuaValueHolder::_SetValue(const char* pszValue)
{
	ClearValue();
	m_nType = LUA_TSTRING;
	m_pszValue = pszValue;
}

void _LuaValueHolder::_SetValue(void *pValue)
{
	ClearValue();
	m_nType = LUA_TUSERDATA;
	m_pValue = pValue;
}

void _LuaValueHolder::_SetValue(bool bValue)
{
	ClearValue();
	m_nType = LUA_TBOOLEAN;
	m_dValue = (double)(int)bValue;
}

void _LuaValueHolder::_SetStackValue(lua_State* L, int nIndex)
{
	int nType = lua_type(L, nIndex);
	switch(nType)
	{
	case LUA_TNUMBER:
		_SetValue(lua_tonumber(L, nIndex));
		break;
	case LUA_TSTRING:
		_SetValue(lua_tostring(L, nIndex));
		break;
	case LUA_TBOOLEAN:
		_SetValue(lua_toboolean(L, nIndex) != 0);
		break;
	case LUA_TUSERDATA:
	case LUA_TLIGHTUSERDATA:
		_SetValue(lua_touserdata(L, nIndex));
		break;
	case LUA_TFUNCTION:
	case LUA_TTABLE:
		if (!m_pLuaValueRef)
			m_pLuaValueRef = new _LuaRefBase(L);
		m_pLuaValueRef->_SetRefrence(nIndex);
	case LUA_TNIL:
		ClearValue();
		m_nType = LUA_TNIL;
		break;
	default:
		assert(false);
	};
}
