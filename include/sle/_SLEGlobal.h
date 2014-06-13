#pragma once
#include "_SLEDefine.h"

namespace sle
{
	class luatable;
	class luafunc;
	class luatablefunc;

	SLE_EXPORT_API void SLE_SetErrorHandle(LUA_ERROR_PROC pFunc);

	SLE_EXPORT_API bool SLE_DoFile(lua_State* L, const char* pszFilename);
	SLE_EXPORT_API bool SLE_DoString(lua_State* L, const char* pszSource);
	SLE_EXPORT_API bool SLE_LoadFile(lua_State* L, const char* pszFilename);
	SLE_EXPORT_API bool SLE_LoadString(lua_State* L, const char* pszSource);

	SLE_EXPORT_API luafunc SLE_GetFunc(lua_State* L, const char* pszLuaAddrDesc);
	SLE_EXPORT_API luatablefunc SLE_GetTableFunc(lua_State* L, const char* pszLuaTableAddrDesc, const char* pszFunc);
	SLE_EXPORT_API luatable SLE_GetTable(lua_State* L, const char* pszLuaAddrDesc);
	SLE_EXPORT_API luatable SLE_NewTable(lua_State* L, const char* pszTableAddrDesc = NULL);
	SLE_EXPORT_API luatable SLE_NewMetaTable(lua_State* L, const char* pszName);

	SLE_EXPORT_API void RegisterLuaClass(lua_State* L, const char* pszClassName);
	SLE_EXPORT_API int SLE_PCall(lua_State* L, int nArgs, int nResults, int* pnResult = NULL);

	// 把Lua类型转换成类型描述符号，比如'd'
	SLE_EXPORT_API char _SLE_LuaType2Desc(int nLuaType);

	// 把lua的类型转换成描述字符串
	SLE_EXPORT_API const char* _SLE_LuaType2Name(int nLuaType);

	// 把c++的类型转换成lua描述
	template <typename T>
	char _SLE_CType2Lua()
	{
		// compile error, please check the type of function param
		typedef typename _SLE_GET_PURE_TYPE<T>::type PURE_TYPE;
		PURE_TYPE::SLE_S_GetClassName();
		return LUA_TOBJECT;
	}
	template <>	char inline _SLE_CType2Lua<int>()			{ return LUA_TNUMBER;		}
	template <>	char inline _SLE_CType2Lua<unsigned int>()	{ return LUA_TNUMBER;		}
	template <>	char inline _SLE_CType2Lua<double>()		{ return LUA_TNUMBER;		}
	template <>	char inline _SLE_CType2Lua<bool>()			{ return LUA_TBOOLEAN;		}
	template <>	char inline _SLE_CType2Lua<const char*>()	{ return LUA_TSTRING;		}
	template <>	char inline _SLE_CType2Lua<char*>()			{ return LUA_TSTRING;		}
	template <>	char inline _SLE_CType2Lua<void*>()			{ return LUA_TUSERDATA;		}
	template <>	char inline _SLE_CType2Lua<luatable>()		{ return LUA_TTABLE;		}
	template <>	char inline _SLE_CType2Lua<luafunc>()		{ return LUA_TFUNCTION;		}
	template <>	char inline _SLE_CType2Lua<lua_State*>()	{ return LUA_TSTATE;		}
}
