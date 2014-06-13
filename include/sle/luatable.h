#pragma once
#include "_LuaRefBase.h"
#include "_LuaStack.h"
#include "_LuaTableField.h"

namespace sle
{
	class _LuaValueHolder;
	template <typename KeyT>
	class _LuaTableField;

	class SLE_EXPORT_API luatable : public _LuaRefBase
	{
	public:
		luatable();
		luatable(lua_State* L);
		luatable(const luatable& rhl);
		luatable(const _LuaRefBase& rhl);
		virtual ~luatable();
		virtual luatable& operator=(const luatable& rhl);

	public:
		template <typename T_K, typename T_V>
		void setfield(T_K _Key, T_V _Value)
		{
			_Push();
			SLE_Push(m_pLuaState, _Key); 
			SLE_Push(m_pLuaState, _Value);
			lua_settable(m_pLuaState, -3);
			_Pop();
		}

		template <typename T_K, typename T_V>
		void rawset(T_K _Key, T_V _Value)
		{
			_Push();
			SLE_Push(m_pLuaState, _Key); 
			SLE_Push(m_pLuaState, _Value);
			lua_rawset(m_pLuaState, -3);
			_Pop();
		}

		template <typename T>
		int insert(T _Value)
		{
			int pos = _GetFirstEmptyPos();
			setfield(pos, _Value);
			return pos;
		}

		template <typename KeyT>
		_LuaTableField<KeyT> operator[](KeyT _Key)
		{
			return _LuaTableField<KeyT>(this, _Key);
		}

		_LuaValueHolder& getfield(int nIdx);
		_LuaValueHolder& getfield(const char* pszKey);

		void setmetatable(luatable& tbMetatable);

	public:
		template <typename KeyT>
		void _GetTable(KeyT _Key)
		{
			_Push();
			SLE_Push(m_pLuaState, _Key);
			lua_gettable(m_pLuaState, -2);
			_Pop();
		}
		lua_Integer _GetFirstEmptyPos(); // ”√”⁄À≥–Ú≤Â»Î£¨º¥table.insert

	protected:
		_LuaValueHolder* m_pLuaValueHolder;
	};
}
