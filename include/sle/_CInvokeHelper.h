#pragma once
#include "_SLEDefine.h"
#include "_LuaStack.h"
#include "luatable.h"
#include "luafunc.h"
#include "luaevn.h"
#include "_SLEGlobal.h"


#define DEFINE_SLE_GET_VALUE(_TYPE, _FUNC)										\
	template<> inline _TYPE SLE_TransValue<_TYPE>(lua_State* L, int nIdx)		\
	{																			\
		return _FUNC(L, nIdx);													\
	}

#define DEFINE_SLE_GET_REF_VALUE(_TYPE)											\
	template<> inline _TYPE SLE_TransValue<_TYPE>(lua_State* L, int nIdx)		\
	{																			\
		_TYPE LuaRef(L);														\
		LuaRef._SetRefrence(nIdx);												\
		return LuaRef;															\
	}

#define SLE_METHOD_PARAMS_OPT(_INDEX) \
	(T##_INDEX)SLE_TransValue<T##_INDEX>(L, _INDEX)

#define SLE_METHOD_PARAMS0
#define SLE_METHOD_PARAMS1 SLE_METHOD_PARAMS0  SLE_METHOD_PARAMS_OPT(1)
#define SLE_METHOD_PARAMS2 SLE_METHOD_PARAMS1, SLE_METHOD_PARAMS_OPT(2)
#define SLE_METHOD_PARAMS3 SLE_METHOD_PARAMS2, SLE_METHOD_PARAMS_OPT(3)
#define SLE_METHOD_PARAMS4 SLE_METHOD_PARAMS3, SLE_METHOD_PARAMS_OPT(4)
#define SLE_METHOD_PARAMS5 SLE_METHOD_PARAMS4, SLE_METHOD_PARAMS_OPT(5)
#define SLE_METHOD_PARAMS6 SLE_METHOD_PARAMS5, SLE_METHOD_PARAMS_OPT(6)
#define SLE_METHOD_PARAMS7 SLE_METHOD_PARAMS6, SLE_METHOD_PARAMS_OPT(7)
#define SLE_METHOD_PARAMS8 SLE_METHOD_PARAMS7, SLE_METHOD_PARAMS_OPT(8)
#define SLE_METHOD_PARAMS9 SLE_METHOD_PARAMS8, SLE_METHOD_PARAMS_OPT(9)
#define SLE_METHOD_PARAMS10 SLE_METHOD_PARAMS9, SLE_METHOD_PARAMS_OPT(10)
#define SLE_METHOD_PARAMS11 SLE_METHOD_PARAMS10, SLE_METHOD_PARAMS_OPT(11)
#define SLE_METHOD_PARAMS12 SLE_METHOD_PARAMS11, SLE_METHOD_PARAMS_OPT(12)

// 成员函数的Holder
#define SLE_METHOD_HOLDER_CLASS(N) \
class _MethodHolder<RetT (SLE_API ClassT::*)(DECLARE_TYPE_LIST##N)> : public _IMethodHolder \
{ \
	typedef RetT (SLE_API ClassT::*TYPE_METHOD)(DECLARE_TYPE_LIST##N); \
public: \
	_MethodHolder(TYPE_METHOD FuncAddr) : m_pFuncAddr(FuncAddr) { } \
	virtual bool Invoke(lua_State* L, void* pObj = NULL, bool bRet2Lua = true) \
	{ \
		SLE_ASSERT_RETURN(pObj); \
		RetT RetValue = ((ClassT*)pObj->*m_pFuncAddr)(SLE_METHOD_PARAMS##N); \
		if (bRet2Lua) \
			SLE_Push(L, RetValue); \
		return true; \
	} \
	virtual void Abstract() { }; \
protected: \
	TYPE_METHOD m_pFuncAddr; \
};

// 静态函数的Holder
#define SLE_METHOD_HOLDER_STATIC(N) \
class _MethodHolder<RetT (SLE_API *)(DECLARE_TYPE_LIST##N)> : public _IMethodHolder \
{ \
	typedef RetT (SLE_API *TYPE_METHOD)(DECLARE_TYPE_LIST##N); \
public: \
	_MethodHolder(TYPE_METHOD FuncAddr) : m_pFuncAddr(FuncAddr) { } \
	virtual bool Invoke(lua_State* L, bool bRet2Lua = true) \
	{ \
		RetT RetValue = m_pFuncAddr(SLE_METHOD_PARAMS##N); \
		if (bRet2Lua) \
			SLE_Push(L, RetValue); \
		return true; \
	} \
	virtual void Abstract() { }; \
protected: \
	TYPE_METHOD m_pFuncAddr; \
};

// 成员函数的Holder - 返回值为void
#define SLE_METHOD_HOLDER_CLASS_VOID(N) \
class _MethodHolder<void (SLE_API ClassT::*)(DECLARE_TYPE_LIST##N)> : public _IMethodHolder \
{ \
	typedef void (SLE_API ClassT::*TYPE_METHOD)(DECLARE_TYPE_LIST##N); \
public: \
	_MethodHolder(TYPE_METHOD FuncAddr) : m_pFuncAddr(FuncAddr) { } \
	virtual bool Invoke(lua_State* L, void *pObj = NULL, bool = true) \
	{ \
		((ClassT*)pObj->*m_pFuncAddr)(SLE_METHOD_PARAMS##N); \
		return true; \
	} \
	virtual void Abstract() { }; \
protected: \
	TYPE_METHOD m_pFuncAddr; \
};

// 静态函数的Holder - 返回值为void
#define SLE_METHOD_HOLDER_STATIC_VOID(N) \
class _MethodHolder<void (SLE_API *)(DECLARE_TYPE_LIST##N)> : public _IMethodHolder \
{ \
	typedef void (SLE_API *TYPE_METHOD)(DECLARE_TYPE_LIST##N); \
public: \
	_MethodHolder(TYPE_METHOD FuncAddr) : m_pFuncAddr(FuncAddr) { } \
	virtual bool Invoke(lua_State* L, bool = true) \
	{ \
		m_pFuncAddr(SLE_METHOD_PARAMS##N); \
		return true; \
	} \
	virtual void Abstract() { }; \
protected: \
	TYPE_METHOD m_pFuncAddr; \
};

#define SLE_DECLARE_METHOD_HOLDER_CLASS(N) \
	template <typename ClassT, typename RetT, DECLARE_TYPENAME##N> \
	SLE_METHOD_HOLDER_CLASS(N) \
	template <typename RetT, DECLARE_TYPENAME##N> \
	SLE_METHOD_HOLDER_STATIC(N)

#define SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(N) \
	template <typename ClassT, DECLARE_TYPENAME##N> \
	SLE_METHOD_HOLDER_CLASS_VOID(N) \
	template <DECLARE_TYPENAME##N> \
	SLE_METHOD_HOLDER_STATIC_VOID(N)

namespace sle
{
	template <typename ValueT>
	ValueT SLE_TransValue(lua_State* L, int nIdx)
	{
		typedef typename _SLE_GET_PURE_TYPE<ValueT>::type PURE_TYPE;
		LuaUserDataType* pUD = (LuaUserDataType*)lua_touserdata(L, nIdx);
		return _SLE_Translate<PURE_TYPE, ValueT>::GetValue(pUD->pObj);
	}

	DEFINE_SLE_GET_VALUE(lua_Integer, lua_tointeger);
	DEFINE_SLE_GET_VALUE(int, (int)lua_tointeger);
	DEFINE_SLE_GET_VALUE(double, lua_tonumber);
	DEFINE_SLE_GET_VALUE(bool, !!lua_toboolean);
	DEFINE_SLE_GET_VALUE(const char*, lua_tostring);
	DEFINE_SLE_GET_VALUE(void*, lua_touserdata);

	DEFINE_SLE_GET_REF_VALUE(_LuaRefBase);
	DEFINE_SLE_GET_REF_VALUE(luatable);
	DEFINE_SLE_GET_REF_VALUE(luafunc);

	template<> inline lua_State* SLE_TransValue<lua_State*>(lua_State* L, int nIdx)
	{
		return L;
	}

	// 这个是接口基类，只有虚的invoke方法，任何时候都不应该触发
	class _IMethodHolder
	{
	public:
		virtual bool Invoke(lua_State* /*L*/, void* /*lpObj*/, bool /*bRet2Lua：指示是否需要把函数返回值入Lua栈*/ = true)
		{
			// 如果调用到这里，请检查函数参数的类型是否符合规则，或者通过静态类名调用了成员函数
			// 或者是否在CLASS_DERIVED前，声明了LUA_CONSTRUCTOR
			SLE_ASSERT(false);
			return false;
		}
		virtual bool Invoke(lua_State* /*L*/, bool /*bRet2Lua：指示是否需要把函数返回值入Lua栈*/ = true)
		{
			// 如果调用到这里，请检查函数参数的类型是否符合规则，或者通过对象调用了静态函数
			// 或者是否在CLASS_DERIVED前，声明了LUA_CONSTRUCTOR
			SLE_ASSERT(false);
			return false;
		}
		virtual void Abstract() { };
		virtual void DeleteThis() { delete this; };
	};

	// 这个模板类只是用来给下面的类特化的，本身没有用处，其构造函数任何时候都不应该被调用
	template <typename FuncType>
	class _MethodHolder : public _IMethodHolder
	{
	public:
		_MethodHolder(FuncType /*FuncAddr*/) { SLE_ASSERT(false); }
	};

	template <typename ClassT, typename RetT>
	SLE_METHOD_HOLDER_CLASS(0);
	template <typename RetT>
	SLE_METHOD_HOLDER_STATIC(0);
	SLE_DECLARE_METHOD_HOLDER_CLASS(1);
	SLE_DECLARE_METHOD_HOLDER_CLASS(2);
	SLE_DECLARE_METHOD_HOLDER_CLASS(3);
	SLE_DECLARE_METHOD_HOLDER_CLASS(4);
	SLE_DECLARE_METHOD_HOLDER_CLASS(5);
	SLE_DECLARE_METHOD_HOLDER_CLASS(6);
	SLE_DECLARE_METHOD_HOLDER_CLASS(7);
	SLE_DECLARE_METHOD_HOLDER_CLASS(8);
	SLE_DECLARE_METHOD_HOLDER_CLASS(9);
	SLE_DECLARE_METHOD_HOLDER_CLASS(10);
	SLE_DECLARE_METHOD_HOLDER_CLASS(11);
	SLE_DECLARE_METHOD_HOLDER_CLASS(12);

	template <typename ClassT>
	SLE_METHOD_HOLDER_CLASS_VOID(0);
	template <>
	SLE_METHOD_HOLDER_STATIC_VOID(0);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(1);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(2);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(3);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(4);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(5);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(6);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(7);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(8);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(9);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(10);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(11);
	SLE_DECLARE_METHOD_HOLDER_CLASS_VOID(12);

#define DECLARE_VAR_METHOD(_TYPE) \
	template <> \
	inline _TYPE _SLE_ReadVariable_Method<_TYPE>(void* pAddress) \
	{ \
		void* pVar = (char*)this + (INT_PTR)pAddress; \
		return *(_TYPE*)pVar; \
	} \
	template <> \
	static inline _TYPE _SLE_ReadVariable_Method_Static<_TYPE>(void* pAddress) \
	{ \
		return *(_TYPE*)pAddress; \
	}

	struct _SLE_VAR_METHOD_TOOL
	{
		template <typename ValueT>
		ValueT _SLE_ReadVariable_Method(void* pAddress)
		{
			VAR TYPE ERR
		}

		template <typename ValueT>
		static ValueT _SLE_ReadVariable_Method_Static(void* pAddress)
		{
			VAR TYPE ERR
		}

		DECLARE_VAR_METHOD(int);
		DECLARE_VAR_METHOD(unsigned int);
		DECLARE_VAR_METHOD(double);
		DECLARE_VAR_METHOD(char*);
		DECLARE_VAR_METHOD(bool);

		////////////////////////////////////////////////////////////
		
		template <typename ValueT>
		void _SLE_WriteVariable_Method(void* pAddress, lua_State* L)
		{
			void* pVar = (char*)this + (INT_PTR)pAddress;
			*(ValueT*)pVar = SLE_TransValue<ValueT>(L, -1);
		}

		template <>
		void inline _SLE_WriteVariable_Method<char*>(void* pAddress, lua_State* L)
		{
			void* pVar = (char*)this + (INT_PTR)pAddress;
			strcpy((char*)pVar, SLE_TransValue<const char*>(L, -1));
		}

		template <int N>
		void _SLE_WriteVariable_Method(char (*pAddress)[N], lua_State* L)
		{
			void* pVar = (char*)this + (INT_PTR)pAddress;
			strncpy((char*)pVar, SLE_TransValue<const char*>(L, -1), N);
		}

		/////////////////////////////////////////////////////////////////

		template <typename ValueT>
		static void _SLE_WriteVariable_Method_Static(void* pAddress, lua_State* L)
		{
			*(ValueT*)pAddress = SLE_TransValue<ValueT>(L, -1);
		}

		template <>
		static void inline _SLE_WriteVariable_Method_Static<char*>(void* pAddress, lua_State* L)
		{
			strcpy((char*)pAddress, SLE_TransValue<const char*>(L, -1));
		}

		template <int N>
		static void _SLE_WriteVariable_Method_Static(char (*pAddress)[N], lua_State* L)
		{
			strncpy((char*)pAddress, SLE_TransValue<const char*>(L, -1), N);
		}
	};

}
