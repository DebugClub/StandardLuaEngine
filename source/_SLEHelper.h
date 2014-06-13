#pragma once
// 内部全局功能函数
#include "_SLEDefine.h"

struct lua_State;

namespace sle
{
	int _SLE_pcall_error(lua_State* L);
	bool _SLE_OnError(lua_State* L);
	void _SLE_Throw_Error(lua_State* L, const char* pszError);
	void _SLE_Push_State(lua_State* L);
	void _SLE_Pop_State();
	lua_State* _SLE_Get_State();
}
