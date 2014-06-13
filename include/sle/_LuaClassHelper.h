#pragma once
#include "_CInvokeHelper.h"
#include "luaevn.h"
#include "_SLEGlobal.h"

#define REGISTER_TO_LUA(L, CLASSNAME)										\
	do {																	\
		CLASSNAME::SLE_S_Init(L);											\
		sle::RegisterLuaClass(L, #CLASSNAME);								\
	}																		\
	while (false);

#define SLE_RETURN LuaStackPusher()

#define BEGIN_DECLARE_SCRIPT(CLASSNAME)										\
	void CLASSNAME::SLE_S_Init(lua_State* L)								\

#define END_DECLARE_SCRIPT()

#define LUA_CONSTRUCT_METHOD "__LUA_CONSTRUCTOR__"
#define LUA_DESTRUCT_METHOD "__LUA_DESTRUCTOR__"

#define DECLARE_LUA_CLASS(CLASSNAME)										\
	public:																	\
		typedef CLASSNAME TYPE_THIS_CLASS;									\
		const char* SLE_GetClassName() const								\
		{																	\
			return #CLASSNAME;												\
		}																	\
		static void SLE_S_DeleteObject(TYPE_THIS_CLASS *pObj)				\
		{																	\
			delete pObj;													\
		}																	\
		static TYPE_THIS_CLASS* SLE_S_NewObject()							\
		{																	\
			return new TYPE_THIS_CLASS;										\
		}																	\
		static const char* SLE_S_GetClassName()								\
		{																	\
			return #CLASSNAME;												\
		}																	\
		static void SLE_S_Init(lua_State* L);								\
	private:																\

#define LUA_FUNC(NAME, MIN_PARAM)											\
	sle::SLE_RegisterMemberFunc(L, SLE_S_GetClassName(), #NAME, &TYPE_THIS_CLASS::NAME, MIN_PARAM);

#define LUA_STATIC_FUNC(NAME, MIN_PARAM)									\
	sle::SLE_RegisterStaticFunc(L, SLE_S_GetClassName(), #NAME, &NAME, MIN_PARAM);

#define LUA_CONSTROCTOR(NAME, MIN_PARAM)									\
	sle::SLE_RegisterStaticFunc(L, SLE_S_GetClassName(), LUA_CONSTRUCT_METHOD, &NAME, MIN_PARAM); \
	sle::SLE_RegisterStaticFunc(L, SLE_S_GetClassName(), LUA_DESTRUCT_METHOD, &SLE_S_DeleteObject, -1);

#define LUA_DEFAULT_CONSTRUCTOR()											\
	sle::SLE_RegisterStaticFunc(L, SLE_S_GetClassName(), LUA_CONSTRUCT_METHOD, &SLE_S_NewObject, -1); \
	sle::SLE_RegisterStaticFunc(L, SLE_S_GetClassName(), LUA_DESTRUCT_METHOD, &SLE_S_DeleteObject, -1);

#define LUA_VAR(NAME) \
	sle::SLE_RegisterMemberVar(L, SLE_S_GetClassName(), #NAME, &(((TYPE_THIS_CLASS*)0)->NAME), false, false);
#define LUA_VAR_READONLY(NAME)	\
	sle::SLE_RegisterMemberVar(L, SLE_S_GetClassName(), #NAME, &(((ClassT*)0)->NAME), true, false);

#define LUA_STATIC_VAR(NAME) \
	sle::SLE_RegisterMemberVar(L, SLE_S_GetClassName(), #NAME, &(NAME), false, true);
#define LUA_STATIC_VAR_READONLY(NAME) \
	sle::SLE_RegisterMemberVar(L, SLE_S_GetClassName(), #NAME, &(NAME), true, true);

#define LUA_VAR_METHOD(NAME, READ, WRITE) \
	sle::SLE_RegisterMemberVarMethod(L, SLE_S_GetClassName(), #NAME, &ClassT::READ, &ClassT::WRITE);
#define LUA_VAR_METHOD_READONLY(NAME, READ) \
	sle::SLE_RegisterMemberVarMethod(L, SLE_S_GetClassName(), #NAME, &ClassT::READ);

#define LUA_STATIC_VAR_METHOD(NAME, READ, WRITE) \
	sle::SLE_RegisterMemberVarMethod(L, SLE_S_GetClassName(), #NAME, &READ, &WRITE);
#define LUA_STATIC_VAR_METHOD_READONLY(NAME, READ) \
	sle::SLE_RegisterMemberVarMethod(L, SLE_S_GetClassName(), #NAME, &READ);










#define SLE_SET_PARAM_OPT(i)												\
	do																		\
	{																		\
		int nType = sle::_SLE_CType2Lua<T##i>();							\
		SLE_ASSERT(i == nArgCount || nType != LUA_TSTATE);						\
		pFuncInfo->ParamsType[i - 1] = nType;								\
	} while(false);

#define SET_PARAM0
#define SET_PARAM1 SET_PARAM0 SLE_SET_PARAM_OPT(1)
#define SET_PARAM2 SET_PARAM1 SLE_SET_PARAM_OPT(2)
#define SET_PARAM3 SET_PARAM2 SLE_SET_PARAM_OPT(3)
#define SET_PARAM4 SET_PARAM3 SLE_SET_PARAM_OPT(4)
#define SET_PARAM5 SET_PARAM4 SLE_SET_PARAM_OPT(5)
#define SET_PARAM6 SET_PARAM5 SLE_SET_PARAM_OPT(6)
#define SET_PARAM7 SET_PARAM6 SLE_SET_PARAM_OPT(7)
#define SET_PARAM8 SET_PARAM7 SLE_SET_PARAM_OPT(8)
#define SET_PARAM9 SET_PARAM8 SLE_SET_PARAM_OPT(9)
#define SET_PARAM10 SET_PARAM9 SLE_SET_PARAM_OPT(10)
#define SET_PARAM11 SET_PARAM10 SLE_SET_PARAM_OPT(11)
#define SET_PARAM12 SET_PARAM11 SLE_SET_PARAM_OPT(12)

#define DEFAULT_PARAMS -1

// 添加成员函数的模板
#define DECLARE_ADD_MEMBER_FUNC(ARG_NUMBER) \
	template <typename RET, typename CLASS, DECLARE_TYPENAME##ARG_NUMBER> \
	void SLE_RegisterMemberFunc(lua_State* L, const char* pszClassName, const char* pszName, RET (SLE_API CLASS::*pFunc)(DECLARE_ARG##ARG_NUMBER), int nMinParam) \
	{ \
		int nArgCount = ARG_NUMBER; \
		SLE_RegisterFunction* pFuncInfo = SLE_RegisterFuncFinal(L, pszClassName, pszName, false, pFunc, ARG_NUMBER, nMinParam == -1 ? ARG_NUMBER : nMinParam); \
		SET_PARAM##ARG_NUMBER \
		if (pFuncInfo->ParamsType[ARG_NUMBER - 1] == LUA_TSTATE) \
		{ \
			pFuncInfo->nMinParamsCount--; \
			pFuncInfo->nMaxParamsCount--; \
		} \
	}

//添加静态函数的模板
#define DECLARE_ADD_STATIC_FUNC(ARG_NUMBER) \
	template <typename RET, DECLARE_TYPENAME##ARG_NUMBER> \
	void SLE_RegisterStaticFunc(lua_State* L, const char* pszClassName, const char* pszName, RET (SLE_API *pFunc)(DECLARE_ARG##ARG_NUMBER), int nMinParam) \
	{ \
		int nArgCount = ARG_NUMBER; \
		SLE_RegisterFunction* pFuncInfo = SLE_RegisterFuncFinal(L, pszClassName, pszName, true, pFunc, ARG_NUMBER, nMinParam == -1 ? ARG_NUMBER : nMinParam); \
		SET_PARAM##ARG_NUMBER \
		if (pFuncInfo->ParamsType[ARG_NUMBER - 1] == LUA_TSTATE) \
		{ \
			pFuncInfo->nMinParamsCount--; \
			pFuncInfo->nMaxParamsCount--; \
		} \
	}

namespace sle
{
	SLE_EXPORT_API SLE_RegisterElement* SLE_RegisterLuaElement(lua_State* L, SLE_RegisterElement* pElem);
	SLE_EXPORT_API SLE_RegisterElement* SLE_QueryLuaElement(lua_State* L, const char* pszClassName, const char* pszField, bool bStatic);

	template <typename ValueT>
	void SLE_RegisterMemberVar(lua_State* L, const char* pszClassName, const char* pszName, ValueT* pVariable, bool bReadonly, bool bStatic)
	{
		pszName = pszName[0] == 'm' && pszName[1] == '_' ? pszName + 2 : pszName;
		if (bStatic)
			SLE_RegisterVarFinal(L, pszClassName, pszName, pVariable, bReadonly, bStatic, 0, _SLE_VAR_METHOD_TOOL::_SLE_ReadVariable_Method_Static<ValueT>, _SLE_VAR_METHOD_TOOL::_SLE_WriteVariable_Method_Static<ValueT>);
		else
			SLE_RegisterVarFinal(L, pszClassName, pszName, pVariable, bReadonly, bStatic, 0, &_SLE_VAR_METHOD_TOOL::_SLE_ReadVariable_Method<ValueT>, &_SLE_VAR_METHOD_TOOL::_SLE_WriteVariable_Method<ValueT>);
	}

	template <int N>
	void SLE_RegisterMemberVar(lua_State* L, const char* pszClassName, const char* pszName, char (*pVar)[N], bool bReadonly, bool bStatic)
	{
		pszName = pszName[0] == 'm' && pszName[1] == '_' ? pszName + 2 : pszName;
		if (bStatic)
			SLE_RegisterVarFinal(L, pszClassName, pszName, (char**)pVar, bReadonly, true, N, _SLE_VAR_METHOD_TOOL::_SLE_ReadVariable_Method_Static<char*>, _SLE_VAR_METHOD_TOOL::_SLE_WriteVariable_Method_Static<N>);
		else
			SLE_RegisterVarFinal(L, pszClassName, pszName, (char**)pVar, bReadonly, true, N, &_SLE_VAR_METHOD_TOOL::_SLE_ReadVariable_Method<char*>, &_SLE_VAR_METHOD_TOOL::_SLE_WriteVariable_Method<N>);
	}

	template <typename RT, typename RCLASS, typename RetWT, typename WT, typename WCLASS>
	void SLE_RegisterMemberVarMethod(lua_State* L, const char* pszClassName, const char* pszName, RT (SLE_API RCLASS::*pReadFunc)(), RetWT (SLE_API WCLASS::*pWriteFunc)(WT))
	{
		SLE_RegisterVarFinal(L, pszClassName, pszName, NULL, false, false, 0, pReadFunc, pWriteFunc);
	}

	template <typename RT, typename RCLASS>
	void SLE_RegisterMemberVarMethod(lua_State* L, const char* pszClassName, const char* pzName, RT (SLE_API RCLASS::*pReadFunc)())
	{
		SLE_RegisterVarFinal(L, pszClassName, pzName, NULL, true, false, 0, pReadFunc, NULL);
	}

	template <typename RT, typename RetWT, typename WT>
	void SLE_RegisterMemberVarMethod(lua_State* L, const char* pszClassName, const char* pszName, RT (*pReadFunc)(), RetWT (*pWriteFunc)(WT))
	{
		SLE_RegisterVarFinal(L, pszClassName, pszName, NULL, false, true, 0, pReadFunc, pWriteFunc);
	}

	template <typename RT>
	void SLE_RegisterMemberVarMethod(lua_State* L, const char* pszClassName, const char* pzName, RT (*pReadFunc)())
	{
		SLE_RegisterVarFinal(L, pszClassName, pzName, NULL, true, true, 0, pReadFunc, NULL);
	}


	// 添加成员函数：0个参数的特化
	template <typename RET, typename CLASS>
	void SLE_RegisterMemberFunc(lua_State* L, const char* pszClassName, const char* pszName, RET (SLE_API CLASS::*pFunc)(), int /*nMinParam*/)
	{
		SLE_RegisterFuncFinal(L, pszClassName, pszName, false, pFunc, 0, 0);
	}

	// 添加静态函数：0个参数的特化
	template <typename RET>
	void SLE_RegisterStaticFunc(lua_State* L, const char* pszClassName, const char* pszName, RET (SLE_API *pFunc)(), int /*nMinParam*/)
	{
		SLE_RegisterFuncFinal(L, pszClassName, pszName, true, pFunc, 0, 0);
	}

	DECLARE_ADD_MEMBER_FUNC(1)
	DECLARE_ADD_MEMBER_FUNC(2)
	DECLARE_ADD_MEMBER_FUNC(3)
	DECLARE_ADD_MEMBER_FUNC(4)
	DECLARE_ADD_MEMBER_FUNC(5)
	DECLARE_ADD_MEMBER_FUNC(6)
	DECLARE_ADD_MEMBER_FUNC(7)
	DECLARE_ADD_MEMBER_FUNC(8)
	DECLARE_ADD_MEMBER_FUNC(9)
	DECLARE_ADD_MEMBER_FUNC(10)
	DECLARE_ADD_MEMBER_FUNC(11)
	DECLARE_ADD_MEMBER_FUNC(12)

	DECLARE_ADD_STATIC_FUNC(1)
	DECLARE_ADD_STATIC_FUNC(2)
	DECLARE_ADD_STATIC_FUNC(3)
	DECLARE_ADD_STATIC_FUNC(4)
	DECLARE_ADD_STATIC_FUNC(5)
	DECLARE_ADD_STATIC_FUNC(6)
	DECLARE_ADD_STATIC_FUNC(7)
	DECLARE_ADD_STATIC_FUNC(8)
	DECLARE_ADD_STATIC_FUNC(9)
	DECLARE_ADD_STATIC_FUNC(10)
	DECLARE_ADD_STATIC_FUNC(11)
	DECLARE_ADD_STATIC_FUNC(12)

	// 添加函数的最终接口
	template <typename FuncType>
	SLE_RegisterFunction* SLE_RegisterFuncFinal(lua_State* L, const char* pszClassName, const char* pszName, bool bStatic, FuncType pFunc, int nMaxParams, int nMinParams)
	{
		SLE_RegisterFunction FuncInfo;
		memset(&FuncInfo, 0, sizeof(FuncInfo));
		FuncInfo.eType = emSLE_RegisterFunction;
		FuncInfo.szClassName = pszClassName;
		FuncInfo.szName = pszName;
		memset(FuncInfo.ParamsType, 0, SLE_MAX_PARAMS_COUNT * sizeof(int));
		FuncInfo.nMaxParamsCount = nMaxParams;
		FuncInfo.nMinParamsCount = nMinParams;
		FuncInfo.bStatic = bStatic;
		FuncInfo.pMethod = new _MethodHolder<FuncType>(pFunc);
		return (SLE_RegisterFunction*)SLE_RegisterLuaElement(L, &FuncInfo);
	}

	template <typename ReadFuncT, typename WriteFuncT>
	void SLE_RegisterVarFinal(lua_State* L, const char* pszClassName, const char* pszName, void* pdest, bool bReadonly, bool bStatic, int size, ReadFuncT pRead, WriteFuncT pWrite)
	{
		/*if (pdest && type != SLE_CTYPE_BOOLEAN && type != SLE_CTYPE_INTEGER && type != SLE_CTYPE_DOUBLE && type != SLE_CTYPE_LPCSTR)
		{
			// TODO: 不支持的类型，不能通过地址直接访问
			SLE_ASSERT(false);
			return;
		}*/
		SLE_RegisterVariable VarInfo;
		memset(&VarInfo, 0, sizeof(VarInfo));
		VarInfo.eType = emSLE_RegisterVariable;
		VarInfo.szClassName = pszClassName;
		VarInfo.szName = pszName;
		VarInfo.pAddress = pdest;
		VarInfo.nSize = size;
		VarInfo.bReadonly = bReadonly;
		VarInfo.bStatic = bStatic;

		if (pRead)
			VarInfo.pReadMethod = new _MethodHolder<ReadFuncT>(pRead);

		if (pWrite)
			VarInfo.pWriteMethod = new _MethodHolder<WriteFuncT>(pWrite);

		SLE_RegisterLuaElement(L, &VarInfo);
	}
}
