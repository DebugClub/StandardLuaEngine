#pragma once
#include "_SLEDefine.h"
#include <string>
#include <map>

struct lua_State;

#define DECLARE_CALL_FUNC_METHOD(ARG_NUMBER) \
	template <DECLARE_TYPENAME##ARG_NUMBER> \
	_LuaMultiValueHolder& calltablefunc(const char* pszTable, const char* pszFunc, DECLARE_ARG##ARG_NUMBER) \
	{ \
		*m_pIntervalTableFunc = gettablefunc(pszTable, pszFunc); \
		return m_pIntervalTableFunc->invoke(DECLARE_ARG_LIST##ARG_NUMBER); \
	} \
	template <DECLARE_TYPENAME##ARG_NUMBER> \
	_LuaMultiValueHolder& callfunc(const char* pszFunc, DECLARE_ARG##ARG_NUMBER) \
	{ \
		*m_pIntervalFunc = getfunc(pszFunc); \
		return m_pIntervalFunc->invoke(DECLARE_ARG_LIST##ARG_NUMBER); \
	}

namespace sle
{
	class luafunc;
	class luatablefunc;
	class luatable;
	class _LuaMultiValueHolder;

	class SLE_EXPORT_API luaevn
	{
	public:
		luaevn();
		luaevn& operator=(const luaevn& rhl){return *this;};
		virtual ~luaevn();
		luaevn(const luaevn& rhl){};
		luaevn(lua_State* L){};

	public:
		bool create();
		bool attach(lua_State* L);
		void release();

	public:
		bool dofile(const char* pszFilename);
		bool dostring(const char* pszSource);
		bool loadfile(const char* pszFilename);
		bool loadstring(const char* pszSource);
		int loadpath(const char* pszPath);
		lua_State* getLuaState();

		double read_number();
		const char* read_string();

		luafunc getfunc(const char* pszLuaAddrDesc);
		luatablefunc gettablefunc(const char* pszLuaTableAddrDesc, const char* pszFunc);
		luatable gettable(const char* pszLuaAddrDesc);
		luatable newtable(const char* pszTableAddrDesc = NULL);
		luatable newmetatable(const char* pszName);

	public:
		_LuaMultiValueHolder& calltablefunc(const char* pszTable, const char* pszFunc);
		_LuaMultiValueHolder& callfunc(const char* pszFunc);

		DECLARE_CALL_FUNC_METHOD(1)
		DECLARE_CALL_FUNC_METHOD(2)
		DECLARE_CALL_FUNC_METHOD(3)
		DECLARE_CALL_FUNC_METHOD(4)
		DECLARE_CALL_FUNC_METHOD(5)
		DECLARE_CALL_FUNC_METHOD(6)
		DECLARE_CALL_FUNC_METHOD(7)
		DECLARE_CALL_FUNC_METHOD(8)
		DECLARE_CALL_FUNC_METHOD(9)
		DECLARE_CALL_FUNC_METHOD(10)
		DECLARE_CALL_FUNC_METHOD(11)
		DECLARE_CALL_FUNC_METHOD(12)

	protected:
		lua_State* m_pLuaState;
		luatablefunc* m_pIntervalTableFunc;
		luafunc* m_pIntervalFunc;
	};
}
