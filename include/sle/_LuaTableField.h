#pragma once
#include "_SLEDefine.h"
#include "_LuaStack.h"
#include "_LuaRefBase.h"

#define TRANS_TYPE(_TYPE, _FUNC)						\
	operator _TYPE() const								\
	{													\
		lua_State* L = m_pLuaTable->GetLuaState();		\
		m_pLuaTable->_GetTable(m_Key);					\
		_TYPE _Value = _FUNC(L, -1);					\
		lua_pop(L, 1);									\
		return _Value;									\
	}

namespace sle
{
	class luatable;

	template <typename KeyT>
	class _LuaTableField
	{
	public:
		_LuaTableField()
		{
		}

		_LuaTableField(luatable* pLuaTable, KeyT _Key)
			: m_pLuaTable(pLuaTable), m_Key(_Key)
		{
		}

		~_LuaTableField()
		{
		}

		_LuaTableField<KeyT>& operator=(const _LuaTableField<KeyT>& rhl)
		{
			m_pLuaTable = rhl.m_pLuaTable;
			m_Key = rhl.m_Key;
			return *this;
		}

		template <typename ValueT>
		void operator=(const ValueT& _Value)
		{
			m_pLuaTable->setfield(m_Key, _Value);
		}

	public:
		TRANS_TYPE(lua_Integer, lua_tointeger);
		TRANS_TYPE(int, (int)lua_tointeger);
		TRANS_TYPE(const char*, lua_tostring);
		TRANS_TYPE(double, lua_tonumber);
		TRANS_TYPE(bool, !!lua_toboolean);
		TRANS_TYPE(void*, lua_touserdata);

		operator _LuaRefBase&() const 
		{
			LuaStackHolder StackHolder(m_pLuaTable->GetLuaState());
			m_pLuaTable->_GetTable(m_Key);
			m_LuaRefValue._SetRefrence(-1);
			return m_LuaRefValue;
		}

	protected:
		luatable* m_pLuaTable;
		KeyT m_Key;
		_LuaRefBase m_LuaRefValue;
	};
}
