#include "stdafx.h"
#include <string.h>
#include "_SLEHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int					s_nLuaStatePos	= 0;
static lua_State*			s_L[255]		= { NULL };
extern sle::LUA_ERROR_PROC	g_pErrFunc;

// pcall出错的调用，附加本次报错的堆栈信息
int sle::_SLE_pcall_error(lua_State* L)
{
	const char* pszErr = lua_tostring(L, -1);
	if (pszErr[strlen(pszErr) - 1] != '\n')
	{
		lua_pushstring(L, "\n");
		lua_concat(L, 2);
	}
	lua_getglobal(L, "debug");
	lua_getfield(L, -1, "traceback");
	lua_remove(L, -2);
	int nRet = lua_pcall(L, 0, 1, 0);
	if (nRet != 0)
		return 1;
	lua_concat(L, 2);
	return 1;
}

bool sle::_SLE_OnError(lua_State* L)
{
	bool bShow = true;
	if (g_pErrFunc)
		bShow = !g_pErrFunc(L);

	if (bShow)
	{
		const char* pszErr = lua_tostring(L, -1);
		printf("%s\n", pszErr);
		lua_pop(L, 1);
	}

	return false;
}

void sle::_SLE_Throw_Error(lua_State* L, const char* pszError)
{
	lua_Debug ar;
	bool bSafe = false;
	memset(&ar, 0, sizeof(ar));
	for (int i = 0; lua_getstack(L, i, &ar); ++i)
	{
		lua_getinfo(L, "nSlu", &ar);
		if (ar.what == NULL)
			continue;
		if (strcmp(ar.what, "Lua") == 0 || strcmp(ar.what, "main") == 0)
		{
			bSafe = true;
			break;
		}
	}
	if (bSafe)
	{
		luaL_error(L, pszError);
	}
	else
	{
		lua_pushstring(L, pszError);
		_SLE_pcall_error(L);
		_SLE_OnError(L);
		assert(false);
	}
}

void sle::_SLE_Push_State(lua_State* L)
{
	s_L[++s_nLuaStatePos] = L;
}

void sle::_SLE_Pop_State()
{
	s_L[s_nLuaStatePos--] = NULL;
}

lua_State* sle::_SLE_Get_State()
{
	assert(s_nLuaStatePos > 0);
	return s_L[s_nLuaStatePos];
}
