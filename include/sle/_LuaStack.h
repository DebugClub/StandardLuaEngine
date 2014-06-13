#pragma once
#include "_SLEDefine.h"

#define DECLARE_SLE_PUSH_REF(_BASETYPE, _TYPE) \
	void inline SLE_Push(lua_State* L, _TYPE v) \
	{ \
		SLE_Push(L, (_BASETYPE)v); \
	}

namespace sle
{
	class _LuaRefBase;
	class luatable;
	class luafunc;
	class luaevn;

	class SLE_EXPORT_API LuaStackHolder
	{
	public:
		LuaStackHolder(luaevn* E);
		LuaStackHolder(lua_State* L);
		~LuaStackHolder();
	protected:
		lua_State* m_pLuaState;
		int m_nTopIndex;
	};

	class SLE_EXPORT_API LuaStackPusher
	{
	public:
		LuaStackPusher();
		~LuaStackPusher();
	public:
		template <typename ValueT>
		LuaStackPusher& operator<<(ValueT tValue)
		{
			SLE_ASSERT(m_pLuaState);
			if (m_pLuaState)
			{
				SLE_Push(m_pLuaState, tValue);
			}
			return *this;
		}
	protected:
		lua_State* m_pLuaState;
	};

	bool SLE_EXPORT_API GetLuaValue(lua_State* L, const char* pszLuaAddrDesc);

	void SLE_EXPORT_API SLE_Push(lua_State* L, int nValue);
	void SLE_EXPORT_API SLE_Push(lua_State* L, float fValue);
	void SLE_EXPORT_API SLE_Push(lua_State* L, double dValue);
	void SLE_EXPORT_API SLE_Push(lua_State* L, bool bValue);
	void SLE_EXPORT_API SLE_Push(lua_State* L, const char* nValue);
	void SLE_EXPORT_API SLE_Push(lua_State* L, lua_CFunction pfnValue);
	void SLE_EXPORT_API SLE_Push(lua_State* L, void* pPointer);

	void SLE_EXPORT_API SLE_Push(lua_State* L, LuaUserDataType* pUserData);

	void SLE_EXPORT_API SLE_Push(lua_State* L, const _LuaRefBase* pLuaRefValue);
	void SLE_EXPORT_API SLE_Push(lua_State* L, const _LuaRefBase& rLuaRefValue);
	void SLE_EXPORT_API _SLE_PrintStack(lua_State* L);

	DECLARE_SLE_PUSH_REF(const _LuaRefBase*, _LuaRefBase*);
	DECLARE_SLE_PUSH_REF(const _LuaRefBase&, _LuaRefBase&);

	DECLARE_SLE_PUSH_REF(const _LuaRefBase*, luatable*);
	DECLARE_SLE_PUSH_REF(const _LuaRefBase*, const luatable*);
	DECLARE_SLE_PUSH_REF(const _LuaRefBase&, luatable&);
	DECLARE_SLE_PUSH_REF(const _LuaRefBase&, const luatable&);

	DECLARE_SLE_PUSH_REF(const _LuaRefBase*, luafunc*);
	DECLARE_SLE_PUSH_REF(const _LuaRefBase*, const luafunc*);
	DECLARE_SLE_PUSH_REF(const _LuaRefBase&, luafunc&);
	DECLARE_SLE_PUSH_REF(const _LuaRefBase&, const luafunc&);

	template <typename ClassT>
	inline void SLE_Push(lua_State* L, ClassT* pObj)
	{
#ifdef _DEBUG
		luaL_getmetatable(L, pObj->SLE_GetClassName());
		int nTypeCheck = lua_type(L, -1);
		lua_pop(L, 1);
		if (nTypeCheck != LUA_TTABLE)
		{
			SLE_ASSERT(false);
			return;
		}
#endif
		LuaUserDataType UD;
		UD.bAutoRelease = false;
		UD.pObj = pObj;
		UD.pszClassName = pObj->SLE_GetClassName();
		SLE_Push(L, &UD);
	}

	template <typename ClassT>
	inline void SLE_Push(lua_State* L, ClassT& rObj)
	{
		SLE_Push(L, &rObj);
	}

	template<>
	inline void SLE_Push<char>(lua_State* L, char szStr[])
	{
		lua_pushstring(L, szStr);
	}

}
