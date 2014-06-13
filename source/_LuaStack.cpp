#include "stdafx.h"
#include "_LuaStack.h"
#include "_SLEGlobal.h"
#include "_LuaRefBase.h"
#include "_LuaError.h"
#include "luaevn.h"
#include "_SLEHelper.h"
#include <memory.h>

using namespace sle;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static char s_szAddrList[12][64] = { 0 };

LuaStackHolder::LuaStackHolder(lua_State* L)
{
	m_pLuaState = L;
	m_nTopIndex = lua_gettop(L);
}

LuaStackHolder::LuaStackHolder(luaevn* E)
{
	m_pLuaState = E->getLuaState();
	m_nTopIndex = lua_gettop(m_pLuaState);
}

LuaStackHolder::~LuaStackHolder()
{
	int nTopIndex = lua_gettop(m_pLuaState);
	assert(nTopIndex >= m_nTopIndex);
	lua_settop(m_pLuaState, m_nTopIndex);
}

LuaStackPusher::LuaStackPusher()
{
	m_pLuaState = _SLE_Get_State();
}

LuaStackPusher::~LuaStackPusher()
{
	m_pLuaState = NULL;
}

void sle::SLE_Push(lua_State* L, int nValue)
{
	lua_pushinteger(L, nValue);
}

void sle::SLE_Push(lua_State* L, float fValue)
{
	lua_pushnumber(L, fValue);
}

void sle::SLE_Push(lua_State* L, double dValue)
{
	lua_pushnumber(L, dValue);
}

void sle::SLE_Push(lua_State* L, bool bValue)
{
	lua_pushboolean(L, bValue);
}

void sle::SLE_Push(lua_State* L, const char* pszValue)
{
	lua_pushstring(L, pszValue);
}

void sle::SLE_Push(lua_State* L, lua_CFunction pfnValue)
{
	lua_pushcfunction(L, pfnValue);
}

void sle::SLE_Push(lua_State* L, void* pPointer)
{
	lua_pushlightuserdata(L, pPointer);
}

void sle::SLE_Push(lua_State* L, const _LuaRefBase* pLuaRefValue)
{
	assert(L == pLuaRefValue->GetLuaState());
	pLuaRefValue->_Push();
}

void sle::SLE_Push(lua_State* L, const _LuaRefBase& rLuaRefValue)
{
	assert(L == rLuaRefValue.GetLuaState());
	rLuaRefValue._Push();
}

void sle::SLE_Push(lua_State* L, LuaUserDataType* pUserData)
{
	LuaUserDataType* pNewUserData = (LuaUserDataType*)lua_newuserdata(L, sizeof(LuaUserDataType));
	memcpy(pNewUserData, pUserData, sizeof(LuaUserDataType));
	// 就算之前的autorelease是true，这里也要设置成false，因为一个对象只能有一个autorelease，否则会出现重复delete
	pNewUserData->bAutoRelease = false;
	luaL_getmetatable(L, pUserData->pszClassName);
	lua_setmetatable(L, -2);
}

void sle::_SLE_PrintStack(lua_State* L)
{
	int nTop = lua_gettop(L);
	printf("TopIndex=%d\n", nTop);
	for (int i = 1; i <= nTop; ++i)
	{
		int nType = lua_type(L, i);
		const char* pszType = lua_typename(L, nType);
		switch (nType)
		{
		case LUA_TNUMBER:
			printf("%d: Type=%s, Value=%f\n", i, pszType, lua_tonumber(L, i));
			break;
		case LUA_TFUNCTION:
		case LUA_TTABLE:
		case LUA_TUSERDATA:
		case LUA_TLIGHTUSERDATA:
			printf("%d: Type=%s, Value=0x%p\n", i, pszType, lua_topointer(L, i));
			break;
		case LUA_TBOOLEAN:
			printf("%d: Type=%s, Value=%b\n", i, pszType, lua_toboolean(L, i));
			break;
		default:
			printf("%d: Type=%s, Value=%s\n", i, pszType, lua_tostring(L, i));
		}
	}
}

bool sle::GetLuaValue(lua_State* L, const char* pszLuaAddrDesc)
{
	char		szAddr[256];
	int			nSecCount		= 0;

	strcpy(szAddr, pszLuaAddrDesc);
	lua_getglobal(L, "_G");
	for (char* pszSection = strtok(szAddr, "."); pszSection; pszSection = strtok(NULL, "."))
	{
		lua_getfield(L, -1, pszSection);
		lua_remove(L, -2);
		
		if (lua_type(L, -1) == LUA_TNIL)
		{
			lua_remove(L, -1);
			lua_pushfstring(L, "attempt to index %s, a nil value, in %s", pszSection, pszLuaAddrDesc);
			_SLE_pcall_error(L);
			return _SLE_OnError(L);
		}
	}

	return true;
}
