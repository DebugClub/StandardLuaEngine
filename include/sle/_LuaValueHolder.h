#pragma once
#include "_SLEDefine.h"

namespace sle
{
	class _LuaStack;
	class luafunc;
	class luatable;
	class _LuaRefBase;
	class _LuaValueHolder
	{
	public:
		_LuaValueHolder();
		~_LuaValueHolder();
		
	public:
		void _SetValue(int nValue);
		void _SetValue(double dValue);
		void _SetValue(const char* pszValue);
		void _SetValue(bool bValue);
		void _SetValue(void *pValue);
		void _SetStackValue(lua_State* L, int nIndex);
		virtual void InitValue();
		virtual void ClearValue();
		virtual int GetType();

	public:
		virtual operator int() const;
		virtual operator double() const;
		virtual operator bool() const;
		virtual operator const char*() const;
		virtual operator void*() const;
		virtual operator luafunc() const;
		virtual operator luatable() const;

	protected:
		int m_nType;
		double m_dValue;
		const char* m_pszValue;
		void *m_pValue;
		_LuaRefBase* m_pLuaValueRef;
	};
}
