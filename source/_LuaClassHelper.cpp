#include "stdafx.h"
#include "_LuaClassHelper.h"
using namespace sle;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int s_nRegisterElementTable = 0;

static int SLE_GCLuaElement(lua_State* L)
{
	SLE_RegisterElement* pElem = (SLE_RegisterElement*)(lua_touserdata(L, 1));
	switch (pElem->eType)
	{
	case emSLE_RegisterFunction:
		{
			SLE_RegisterFunction* pRegFunc = (SLE_RegisterFunction*)pElem;
			pRegFunc->pMethod->DeleteThis();
		}
		break;

	case emSLE_RegisterVariable:
		{
			SLE_RegisterVariable* pRegVar = (SLE_RegisterVariable*)pElem;
			pRegVar->pReadMethod->DeleteThis();
			pRegVar->pWriteMethod->DeleteThis();
		}
		break;

	default:
		assert(false);
		return NULL;
	}

	return 0;
}

SLE_RegisterElement* sle::SLE_RegisterLuaElement(lua_State* L, SLE_RegisterElement* pElem)
{
	LuaStackHolder StackSafe(L);
	if (s_nRegisterElementTable == 0)
	{
		lua_newtable(L);
		s_nRegisterElementTable = luaL_ref(L, LUA_REGISTRYINDEX);
	}
	lua_rawgeti(L, LUA_REGISTRYINDEX, s_nRegisterElementTable);

	lua_getfield(L, -1, pElem->szClassName);
	if (lua_type(L, -1) != LUA_TTABLE)
	{
		lua_pop(L, 1);
		lua_newtable(L);
		lua_setfield(L, -2, pElem->szClassName);
		lua_getfield(L, -1, pElem->szClassName);
	}

	size_t uSize = 0;
	switch (pElem->eType)
	{
	case emSLE_RegisterFunction:
		uSize = sizeof(SLE_RegisterFunction);
		break;

	case emSLE_RegisterVariable:
		uSize = sizeof(SLE_RegisterVariable);
		break;

	default:
		assert(false);
		return NULL;
	}

	void* pUD = lua_newuserdata(L, uSize); // TODO 应该重载__gc，写一个回收的方法
	
	// 重载__gc
	{
		lua_newtable(L);
		lua_pushcfunction(L, SLE_GCLuaElement);
		lua_setfield(L, -2, "__gc");
		lua_setmetatable(L, -2);
	}

	memcpy(pUD, pElem, uSize);
	lua_setfield(L, -2, pElem->szName);	
	return (SLE_RegisterElement*)pUD;
}

SLE_RegisterElement* sle::SLE_QueryLuaElement(lua_State* L, const char* pszClassName, const char* pszField, bool bStatic)
{
	LuaStackHolder StackSafe(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, s_nRegisterElementTable);
	lua_getfield(L, -1, pszClassName);
	if (lua_type(L, -1) != LUA_TTABLE)
		return NULL;

	lua_getfield(L, -1, pszField);
	SLE_RegisterElement* pElem = (SLE_RegisterElement*)lua_touserdata(L, -1);
	
	if (pElem && pElem->bStatic != bStatic)
		pElem = NULL;

	return pElem;
}
