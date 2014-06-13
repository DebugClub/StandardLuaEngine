#include "stdafx.h"
#include "luaevn.h"
#include "lua.hpp"
#include "_SLEGlobal.h"
#include "_LuaStack.h"
#include "luafunc.h"
#include "luatablefunc.h"
#include "luatable.h"
#include "_LuaError.h"
#include "_SLEHelper.h"
#include <io.h>
#include <set>
#include <string>

using namespace sle;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

luaevn::luaevn()
{
	m_pLuaState = NULL;
}

luaevn::~luaevn()
{
	release();
}

bool luaevn::create()
{
	if (m_pLuaState != NULL)
		return false;

	lua_State* L = luaL_newstate();

	luaopen_base(L);
	luaL_openlibs(L);
	lua_settop(L, 0);

	return attach(L);
}

bool luaevn::attach(lua_State* L)
{
	if (m_pLuaState != NULL)
		return false;

	m_pLuaState = L;
	m_pIntervalTableFunc = new luatablefunc(m_pLuaState);
	m_pIntervalFunc = new luafunc(m_pLuaState);

	return true;
}

void luaevn::release()
{
	if (!m_pLuaState)
		return;
	DELETE_POINTER(m_pIntervalFunc);
	DELETE_POINTER(m_pIntervalTableFunc);
	lua_close(m_pLuaState);
	m_pLuaState = NULL;
}

bool luaevn::dofile(const char* pszFilename)
{
	LuaStackHolder StackHolder(m_pLuaState);
	if (luaL_loadfile(m_pLuaState, pszFilename) != 0)
	{
		return _SLE_OnError(m_pLuaState);
	}
	int nRet = SLE_PCall(m_pLuaState, 0, LUA_MULTRET);
	return nRet == 0;
}

bool luaevn::dostring(const char* pszSource)
{
	LuaStackHolder StackHolder(m_pLuaState);
	if (luaL_loadstring(m_pLuaState, pszSource) != 0)
	{
		return _SLE_OnError(m_pLuaState);
	}
	int nRet = SLE_PCall(m_pLuaState, 0, LUA_MULTRET);
	return nRet == 0;
}

bool luaevn::loadfile(const char* pszFilename)
{
	if (luaL_loadfile(m_pLuaState, pszFilename) != 0)
	{
		return _SLE_OnError(m_pLuaState);
	}
	return true;
}

int luaevn::loadpath(const char* pszPath)
{
	int nFileCount = 0;
	char szPathMask[_MAX_FNAME];
	long handle = 0;
	_finddata_t fileInfo;
	std::set<std::string> FileNameSet;

	sprintf(szPathMask, "%s*.lua", pszPath);
	handle = _findfirst(szPathMask, &fileInfo);
	SLE_FAIL_JUMP(handle != -1);

	do 
	{
		nFileCount++;
		fileInfo.name;
	} while (_findnext(handle, &fileInfo) == 0);

Exit0:
	return nFileCount;
}

bool luaevn::loadstring(const char* pszSource)
{
	if (luaL_loadstring(m_pLuaState, pszSource) != 0)
	{
		return _SLE_OnError(m_pLuaState);
	}
	return true;
}

lua_State* luaevn::getLuaState()
{
	return m_pLuaState;
}

double luaevn::read_number()
{
	return 0.0F;
}

const char* luaevn::read_string()
{
	return NULL;
}

luafunc luaevn::getfunc(const char* pszLuaAddrDesc)
{
	luafunc f(m_pLuaState);
	f._SetRefrence(pszLuaAddrDesc);
	return f;
}

luatablefunc luaevn::gettablefunc(const char* pszLuaTableAddrDesc, const char* pszFunc)
{
	luatablefunc f(m_pLuaState);
	char szFuncDesc[SLE_MAX_NAME_LEN];
	sprintf(szFuncDesc, "%s.%s", pszLuaTableAddrDesc, pszFunc);
	f._SetRefrence(szFuncDesc);
	f._SetTableRefrence(pszLuaTableAddrDesc);
	return f;
}

luatable luaevn::gettable(const char* pszLuaAddrDesc)
{
	luatable t(m_pLuaState);
	t._SetRefrence(pszLuaAddrDesc);
	return t;
}

luatable luaevn::newtable(const char* pszTableAddrDesc)
{
	LuaStackHolder StackHolder(m_pLuaState);
	luatable t(m_pLuaState);
	if (!pszTableAddrDesc)
	{
		lua_newtable(m_pLuaState);
		t._SetRefrence(-1);
	}
	else
	{
		char szCmd[512] = { 0 };
		sprintf(szCmd, "%s={}", pszTableAddrDesc);
		if (dostring(szCmd))
			t._SetRefrence(pszTableAddrDesc);
		else
			assert(false);
	}

	return t;
}

luatable luaevn::newmetatable(const char* pszName)
{
	LuaStackHolder StackHolder(m_pLuaState);
	luatable t(m_pLuaState);
	luaL_newmetatable(m_pLuaState, pszName);
	t._SetRefrence(-1);
	return t;
}

_LuaMultiValueHolder& luaevn::calltablefunc(const char* pszTable, const char* pszFunc)
{
	*m_pIntervalTableFunc = gettablefunc(pszTable, pszFunc);
	return m_pIntervalTableFunc->Invoke();
}

_LuaMultiValueHolder& luaevn::callfunc(const char* pszFunc)
{
	*m_pIntervalFunc = getfunc(pszFunc);
	return m_pIntervalFunc->Invoke();
}
