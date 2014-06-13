#pragma once
#include "_SLEDefine.h"

namespace sle
{
	// 重载Lua的Metatable相关操作
	int _CB_Operator_Index(lua_State* L);
	int _CB_Operator_NewIndex(lua_State* L);
	int _CB_Operator_New(lua_State* L);
	int _CB_Operator_Delete(lua_State* L);
	int _CB_Operator_ToString (lua_State* L);
	int _CB_Operator_Equal (lua_State* L);

	int _CB_Static_Operator_Index(lua_State* L);
	int _CB_Static_Operator_NewIndex(lua_State* L);

	int _CB_Static_Override(lua_State* L);
	int _CB_Member_Override(lua_State* L);
	
	int _CB_Function(lua_State *L);
	int _CB_Override_Function(lua_State *L);
}
