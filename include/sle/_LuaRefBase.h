#pragma once
#include "_SLEDefine.h"

namespace sle
{
	class _LuaStack;
	class SLE_EXPORT_API _LuaRefBase
	{
	public:
		_LuaRefBase();
		_LuaRefBase(lua_State* L);
		_LuaRefBase(const _LuaRefBase& rhl);
		virtual _LuaRefBase& operator=(const _LuaRefBase& rhl);
		virtual ~_LuaRefBase();

	public:
		virtual bool valid() const;
		virtual void _ResetRefrence();
		virtual bool _SetRefrence(const char* pszLuaAddrDesc);
		virtual bool _SetRefrence(int nIndex);
		virtual void _Push() const;
		virtual void _Pop() const;
		lua_State* GetLuaState() const;

	protected:
		lua_State* m_pLuaState;
		int m_nRefIndex;
		int m_nStackIndex;
	};
}
