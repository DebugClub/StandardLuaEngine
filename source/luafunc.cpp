#include "stdafx.h"
#include "luafunc.h"
#include "lua.hpp"
#include "luaevn.h"
#include "_LuaStack.h"
#include "_LuaMultiValueHolder.h"
#include "_SLEGlobal.h"

using namespace sle;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

luafunc::luafunc()
{
	m_nErrCode = 0;
	m_pLuaMultiValueHolder = new _LuaMultiValueHolder();
}

luafunc::luafunc(lua_State* L) : _LuaRefBase(L)
{
	m_nErrCode = 0;
	m_pLuaMultiValueHolder = new _LuaMultiValueHolder();
}

luafunc::luafunc(const luafunc& rhl) : _LuaRefBase(rhl)
{
	m_nErrCode = 0;
	m_pLuaMultiValueHolder = new _LuaMultiValueHolder();
}

luafunc::luafunc(const _LuaRefBase& rhl) : _LuaRefBase(rhl)
{
	m_nErrCode = 0;
	m_pLuaMultiValueHolder = new _LuaMultiValueHolder();
}

luafunc& luafunc::operator=(const luafunc& rhl)
{
	_LuaRefBase::operator =(rhl);
	m_nErrCode = 0;
	return *this;
}

luafunc::~luafunc()
{
	m_nErrCode = 0;
	DELETE_POINTER(m_pLuaMultiValueHolder);
}

_LuaMultiValueHolder& luafunc::_Call(int nArgs)
{
	int nResult = 0;
	m_nErrCode = SLE_PCall(m_pLuaState, nArgs, LUA_MULTRET, &nResult);
	if (m_nErrCode == 0)
	{
		m_pLuaMultiValueHolder->_RefreshFromStack(m_pLuaState, nResult);
	}
	return *m_pLuaMultiValueHolder;
}

int luafunc::GetLastErrCode()
{
	return m_nErrCode;
}

