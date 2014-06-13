#pragma once
#include "luafunc.h"
#include "luatable.h"

namespace sle
{
	class SLE_EXPORT_API luatablefunc: public luafunc
	{
	public:
		luatablefunc();
		luatablefunc(lua_State* L);
		luatablefunc(const luatablefunc& rhl);
		virtual ~luatablefunc();
		virtual luatablefunc& operator=(const luatablefunc& rhl);

	public:
		void _SetTableRefrence(const char* pszLuaTableAddrDesc);
		virtual _LuaMultiValueHolder& _Call(int nArgs);

	protected:
		luatable m_LuaTable;
	};
}
