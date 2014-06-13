#include "stdafx.h"
#include "_SLEGlobal.h"
#include "lua.hpp"
#include "luatable.h"
#include "luafunc.h"
#include "luatablefunc.h"
#include "_LuaError.h"
#include "_LuaClassHelper.h"
#include "_SLEHelper.h"
#include "_LuaClassInterface.h"

using namespace sle;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const char *const luaT_typedesc = "ibudstfuhrvpo?";

sle::LUA_ERROR_PROC	g_pErrFunc = NULL;

extern "C"
{
	extern _CRTIMP void __cdecl _assert(_In_z_ const char * _Message, _In_z_ const char *_File, _In_ unsigned _Line);
}

void sle::_SLE_ASSERT(const char * _Message, const char *_File, unsigned _Line)
{
	_assert(_Message, _File, _Line);
}

void sle::SLE_SetErrorHandle(LUA_ERROR_PROC pFunc)
{
	g_pErrFunc = pFunc;
}

char sle::_SLE_LuaType2Desc(int nLuaType)
{
	return luaT_typedesc[nLuaType];
}

const char* sle::_SLE_LuaType2Name(int nLuaType)
{
	if (nLuaType < LUA_TYPE_COUNT)
		return lua_typename(NULL, nLuaType);
	switch(nLuaType)
	{
	case LUA_TOBJECT:
		return "CObject";
	case LUA_TSTATE:
		return "lua_State*";
	default:
		assert(false);
		return NULL;
	}
}

bool sle::SLE_DoFile(lua_State* L, const char* pszFilename)
{
	return SLE_LoadFile(L, pszFilename) ? 
		SLE_PCall(L, 0, LUA_MULTRET) == 0 : false;
}

bool sle::SLE_DoString(lua_State* L, const char* pszSource)
{
	return SLE_LoadString(L, pszSource) ?
		SLE_PCall(L, 0, LUA_MULTRET) == 0 : false;
}

bool sle::SLE_LoadFile(lua_State* L, const char* pszFilename)
{
	return luaL_loadfile(L, pszFilename) == 0 ?
		true :_SLE_OnError(L);
}

bool sle::SLE_LoadString(lua_State* L, const char* pszSource)
{
	return luaL_loadstring(L, pszSource) == 0 ?
		true : _SLE_OnError(L);
}

luafunc sle::SLE_GetFunc(lua_State* L, const char* pszLuaAddrDesc)
{
	luafunc f(L);
	f._SetRefrence(pszLuaAddrDesc);
	return f;
}

luatablefunc sle::SLE_GetTableFunc(lua_State* L, const char* pszLuaTableAddrDesc, const char* pszFunc)
{
	luatablefunc f(L);
	char szFuncDesc[SLE_MAX_NAME_LEN];
	sprintf(szFuncDesc, "%s.%s", pszLuaTableAddrDesc, pszFunc);
	f._SetRefrence(szFuncDesc);
	f._SetTableRefrence(pszLuaTableAddrDesc);
	return f;
}

luatable sle::SLE_GetTable(lua_State* L, const char* pszLuaAddrDesc)
{
	luatable t(L);
	t._SetRefrence(pszLuaAddrDesc);
	return t;
}

luatable sle::SLE_NewTable(lua_State* L, const char* pszTableAddrDesc)
{
	LuaStackHolder StackHolder(L);
	luatable t(L);
	if (!pszTableAddrDesc)
	{
		lua_newtable(L);
		t._SetRefrence(-1);
	}
	else
	{
		char szCmd[512] = { 0 };
		sprintf(szCmd, "%s={}", pszTableAddrDesc);
		if (SLE_DoString(L, szCmd))
			t._SetRefrence(pszTableAddrDesc);
		else
			assert(false);
	}

	return t;
}

luatable sle::SLE_NewMetaTable(lua_State* L, const char* pszName)
{
	LuaStackHolder StackHolder(L);
	luatable t(L);
	luaL_newmetatable(L, pszName);
	t._SetRefrence(-1);
	return t;
}

int sle::SLE_PCall(lua_State* L, int nArgs, int nResults, int* pnResult)
{
	int nTopIndex	= lua_gettop(L);
	int nFuncIdx	= nTopIndex - nArgs;

	lua_pushcfunction(L, _SLE_pcall_error);
	lua_insert(L, nFuncIdx); // 把错误处理函数插到被调用函数之前
	int nRetCode = lua_pcall(L, nArgs, nResults, 1);
	lua_remove(L, nFuncIdx);

	if (nRetCode)
	{
		_SLE_OnError(L);
		lua_settop(L, nTopIndex);
	}
	else if (pnResult)
	{
		nTopIndex = lua_gettop(L);
		*pnResult = nTopIndex - nFuncIdx + 1;
	}

	return nRetCode;
}

void sle::RegisterLuaClass(lua_State* L, const char* pszClassName)
{
	luatable tbClass = SLE_NewTable(L, pszClassName);

	tbClass[SZ_StaticOverride] = SLE_NewTable(L);
	tbClass[SZ_MemberOverride] = SLE_NewTable(L);

	{
		luatable tbMetatable = SLE_NewTable(L);
		tbMetatable["__call"] = _CB_Operator_New;
		tbMetatable["__index"] = _CB_Static_Operator_Index;
		tbMetatable["__newindex"] = _CB_Static_Operator_NewIndex;
		tbMetatable[SZ_CLASS_NAME_FIELD] = pszClassName;
		tbClass.setmetatable(tbMetatable);
	}

	// 新建一个metatable，并命名为C++的类名
	// 注意：这个metatable的名字和上述表A的名字是一样的，
	// 但是metatable的命名空间和global的命名空间是不一样的，所以重名无所谓
	luatable tbClassMetatable = SLE_NewMetaTable(L, pszClassName);


	// 隐藏lua对象的metatable，防止在lua中通过getmetatable/setmetatable访问
/*	lpPrase->pushvalue("__metatable");
	lpPrase->pushcopy(methods);
	lpPrase->_SetTable(metatable);
*/

	// 重载对象的__index，访问成员
	tbClassMetatable["__index"] = _CB_Operator_Index;

	// 重载对象的__newindex，变量赋值
	tbClassMetatable["__newindex"] = _CB_Operator_NewIndex;

	// 重载对象的__tostring，print对象会用到
	tbClassMetatable["__tostring"] = _CB_Operator_ToString;

	// 重载对象的__gc，释放
	tbClassMetatable["__gc"] = _CB_Operator_Delete;
}

