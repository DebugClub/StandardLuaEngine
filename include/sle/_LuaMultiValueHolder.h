#pragma once
#include "_LuaValueHolder.h"

#define SLE_MAX_LUA_MULTIVALUEHOLDER 36

namespace sle
{
	class _LuaMultiValueHolder
	{
	public:
		_LuaMultiValueHolder();

	public:
		//��lua��ջ�ϻ�ȡN��ֵ��Ȼ�����Щֵ��luaջ���Ƴ�
		virtual void _RefreshFromStack(lua_State* L, int nCount);
	
	public:
		virtual const _LuaValueHolder& operator[](int _idx) const;

	public:
		virtual operator int() const;
		virtual operator double() const;
		virtual operator bool() const;
		virtual operator const char*() const;
		virtual operator void*() const;

	protected:
		int m_nHolderCount;
		_LuaValueHolder m_Holders[SLE_MAX_LUA_MULTIVALUEHOLDER];
	};
}
