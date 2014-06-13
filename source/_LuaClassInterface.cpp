#include "stdafx.h"
#include "_LuaClassInterface.h"
#include "luaevn.h"
#include "luatable.h"
#include "_LuaClassHelper.h"
#include "_SLEHelper.h"

using namespace sle;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region ErrorProcess

static void ClassKeyError(lua_State* L, const char* pszClassName, const char* pszField)
{
	char szInfo[SLE_BUFFER_LEN];
	const char* lpInfoFmt = "Lua key error: invalid key [%s] of class [%s].";
	sprintf(szInfo, lpInfoFmt, pszField, pszClassName);
	_SLE_Throw_Error(L, szInfo);
}

static const char* GetFuncParamDesc(const SLE_RegisterFunction* pFuncInfo)
{
	static char szParamTypeDesc[SLE_BUFFER_LEN] = { 0 };
	memset(szParamTypeDesc, 0, sizeof(szParamTypeDesc));
	for (int i = 0; i < pFuncInfo->nMinParamsCount; ++i)
	{
		szParamTypeDesc[i] = _SLE_LuaType2Desc(pFuncInfo->ParamsType[i]);
	}
	if (pFuncInfo->nMaxParamsCount > pFuncInfo->nMinParamsCount)
		szParamTypeDesc[pFuncInfo->nMinParamsCount] = '[';
	for (int i = pFuncInfo->nMinParamsCount; i < pFuncInfo->nMaxParamsCount; ++i)
	{
		szParamTypeDesc[i + 1] = _SLE_LuaType2Desc(pFuncInfo->ParamsType[i]);
	}
	if (pFuncInfo->nMaxParamsCount > pFuncInfo->nMinParamsCount)
		szParamTypeDesc[pFuncInfo->nMaxParamsCount] = ']';
	return szParamTypeDesc;
}

static void ParamsTypeError(lua_State* L, const SLE_RegisterFunction* pFuncInfo, int nIndex, int nGot)
{
	char szInfo[SLE_BUFFER_LEN];

	const char* lpInfoFmt = "Lua type error: [%s] expected, got [%s], at param:%d.\nSee declaration of function %s::%s(%s)";
	sprintf(szInfo, lpInfoFmt,
		_SLE_LuaType2Name(pFuncInfo->ParamsType[nIndex]),
		_SLE_LuaType2Name(nGot),
		nIndex + 1,
		pFuncInfo->szClassName,
		pFuncInfo->szName,
		GetFuncParamDesc(pFuncInfo)
		);
	_SLE_Throw_Error(L, szInfo);
}

static void ParamsCountError(lua_State* L, const SLE_RegisterFunction* pFuncInfo, int nExpected, int nGot)
{
	char szInfo[SLE_BUFFER_LEN];
	const char* lpInfoFmt = "Lua params count error: [%d] params expected at least, got [%d].\nSee declaration of function %s::%s(%s).";
	sprintf(szInfo, lpInfoFmt,
		nExpected,
		nGot,
		pFuncInfo->szClassName,
		pFuncInfo->szName,
		GetFuncParamDesc(pFuncInfo)
		);
	_SLE_Throw_Error(L, szInfo);
}

static bool CheckParams(lua_State* L, const SLE_RegisterFunction* pFuncInfo)
{
	int nArgs = lua_gettop(L);
	int nType = -1;
	int nArgIdx = 0;

	if (nArgs < pFuncInfo->nMinParamsCount)
	{
		ParamsCountError(L, pFuncInfo, pFuncInfo->nMinParamsCount, nArgs);
		return false;
	}

	for (nArgIdx = 0; nArgIdx < nArgs; ++nArgIdx)
	{
		if (pFuncInfo->ParamsType[nArgIdx] == LUA_TVARIABLE) // '?'��ʾʲô���Ͷ����ԣ�����nil
			continue;
		nType = lua_type(L, nArgIdx + 1);
		if(nType == LUA_TNIL) // ������Ͳ���'?'��������nil
			break;
		else if (pFuncInfo->ParamsType[nArgIdx] == nType)
			continue;
		else if ((pFuncInfo->ParamsType[nArgIdx] == LUA_TOBJECT) && (nType == LUA_TUSERDATA || nType == LUA_TLIGHTUSERDATA))
			continue;
		else
			break;
	}

	if (nArgIdx != nArgs)
	{
		ParamsTypeError(L, pFuncInfo, nArgIdx, nType);
		return false;
	}
	return true;
}

#pragma endregion

static const char* GetClassNameFromTable(lua_State* L, int nTableIdx)
{
	LuaStackHolder StackSafe(L);
	lua_getmetatable(L, nTableIdx);
	lua_getfield(L, -1, SZ_CLASS_NAME_FIELD);
	const char* pszClassName = lua_tostring(L, -1);
	return pszClassName;
}

static void FillDefaultParams2LuaStack(lua_State* L, const SLE_RegisterFunction* pFuncInfo)
{
	int nLuaArgc = lua_gettop(L);
	// ��ȫ����ΪĬ��ֵ
	for (int i = nLuaArgc; i < pFuncInfo->nMaxParamsCount; ++i)
	{
		lua_pushnil(L);
	}
}

static int CB_Override_Function(lua_State *L)
{
	int nArgs = lua_gettop(L);
	
	lua_pushvalue(L, lua_upvalueindex(2)); // self��ֵ����һ������
	lua_insert(L, 1);

	lua_pushvalue(L, lua_upvalueindex(1)); // ����
	lua_insert(L, 1);

	SLE_PCall(L, nArgs + 1, LUA_MULTRET);

	return lua_gettop(L);
}

static bool CallOverride(lua_State* L, const char* pszClassName, const char* pszField, void* pObj)
{
	const char* pszOverrideTable = pObj ? SZ_MemberOverride : SZ_StaticOverride;
	lua_getglobal(L, pszClassName);
	lua_getfield(L, -1, pszOverrideTable);
	lua_getfield(L, -1, pszField);
	if (lua_isnil(L, -1))
	{
		lua_pop(L, 3);
		return false;
	}

	if (pObj)
		lua_pushlightuserdata(L, pObj);
	else
		lua_getglobal(L, pszClassName);

	lua_pushcclosure(L, CB_Override_Function, 2);
	return true;
}

static int InvokeCMethod(lua_State* L, _IMethodHolder* pMethod, bool bStatic, void* pObj)
{
	bool bCalled = false;
	int nLuaArgc = lua_gettop(L);

	_SLE_Push_State(L);
	if (bStatic && !pObj)
	{
		bCalled = true;
		pMethod->Invoke(L);
	}
	else if (!bStatic && pObj)
	{
		bCalled = true;
		pMethod->Invoke(L, pObj);
	}
	else
	{
		assert(false);
	}
	_SLE_Pop_State();

	int nTopIndex = lua_gettop(L);
	int nRets = bCalled ? nTopIndex - nLuaArgc : 0;
	assert(nRets >= 0);
	return nRets;
}

static int OnAccessClassField(lua_State* L, const char* pszClassName, const char* pszField, void* pObj)
{
	if (CallOverride(L, pszClassName, pszField, pObj))
		return 1;

	SLE_RegisterElement* pRegInfo = SLE_QueryLuaElement(L, pszClassName, pszField, !pObj);
	if (!pRegInfo)
	{
		ClassKeyError(L, pszClassName, pszField);
		return 0;
	}
	else if (pRegInfo->eType == emSLE_RegisterFunction)
	{
		lua_pushlightuserdata(L, pRegInfo);			// ��������Ϣ
		lua_pushlightuserdata(L, pObj);				// thisָ��
		lua_pushcclosure(L, _CB_Function, 2);		// ���ش�����upvalue��Closure��
		return 1;
	}
	else if (pRegInfo->eType == emSLE_RegisterVariable)
	{
		SLE_RegisterVariable* pVarInfo = (SLE_RegisterVariable*)pRegInfo;
		lua_pushlightuserdata(L, pVarInfo->pAddress);
		lua_insert(L, 1);
		return InvokeCMethod(L, pVarInfo->pReadMethod, pVarInfo->bStatic, pObj);
	}
	else
	{
		assert(false);
		return 0;
	}
}

// һ����2��������userdata��field
int sle::_CB_Operator_Index(lua_State* L)
{
	SLE_ASSERT_RETURN(lua_type(L, 1) == LUA_TUSERDATA);		// ��1������������userdata
	SLE_ASSERT_RETURN(lua_type(L, 2) == LUA_TSTRING);		// ��2������������string
	LuaUserDataType* pUserData = (LuaUserDataType*)lua_touserdata(L, 1);
	const char* pszField = lua_tostring(L, 2);
	return OnAccessClassField(L, pUserData->pszClassName, pszField, pUserData->pObj);
}

// һ����3��������userdata��field��value
int sle::_CB_Operator_NewIndex(lua_State *L)
{
	SLE_ASSERT_RETURN(lua_type(L, 1) == LUA_TUSERDATA);		// ��1������������userdata
	SLE_ASSERT_RETURN(lua_type(L, 2) == LUA_TSTRING);		// ��2������������string
	LuaUserDataType* pUserData = (LuaUserDataType*)lua_touserdata(L, 1);
	
	const char* pszClassName = pUserData->pszClassName;
	void* pObj = pUserData->pObj;
	const char* pszField = lua_tostring(L, 2);

	SLE_RegisterElement* pRegInfo = SLE_QueryLuaElement(L, pszClassName, pszField, !pObj);
	if (!pRegInfo || pRegInfo->eType != emSLE_RegisterVariable)
	{
		ClassKeyError(L, pszClassName, pszField);
		return 0;
	}

	SLE_RegisterVariable* pVarInfo = (SLE_RegisterVariable*)pRegInfo;
	lua_pushlightuserdata(L, pVarInfo->pAddress);
	lua_insert(L, 1);
	return InvokeCMethod(L, pVarInfo->pWriteMethod, pVarInfo->bStatic, pObj);
}

// һ����2��������table��field
int sle::_CB_Static_Operator_Index(lua_State *L)
{
	SLE_ASSERT_RETURN(lua_type(L, 1) == LUA_TTABLE);		// ��1������������table
	SLE_ASSERT_RETURN(lua_type(L, 2) == LUA_TSTRING);		// ��2������������string
	const char* pszField = lua_tostring(L, 2);
	const char* pszClassName = GetClassNameFromTable(L, 1);
	return OnAccessClassField(L, pszClassName, pszField, NULL);
}

// һ����3��������table��field��value
int sle::_CB_Static_Operator_NewIndex(lua_State *L)
{
	/*luaevn* pLuaEvn = _GetLuaEvn(L, 1);
	SLE_FAIL_RETURN(lpLuaEvn);
	SLE_ASSERT_RETURN(lua_type(L, 1) == LUA_TTABLE);		// ��1������������table
	SLE_ASSERT_RETURN(lua_type(L, 2) == LUA_TSTRING);		// ��2������������string
	const char* pszField =lua_tostring(L, 2);
	const char* pszClassName = _GetClassName(L, 1);
	return _Operator_NewIndex(*pLuaEvn, pszClassName, pszField, NULL);*/
	return 0;
}

int sle::_CB_Function(lua_State *L)
{
	SLE_RegisterFunction* pFuncInfo = (SLE_RegisterFunction*)(lua_touserdata(L, lua_upvalueindex(1)));
	void* pObj = lua_touserdata(L, lua_upvalueindex(2));

	if (!CheckParams(L, pFuncInfo))
		return 0;

	FillDefaultParams2LuaStack(L, pFuncInfo);

	return InvokeCMethod(L, pFuncInfo->pMethod, pFuncInfo->bStatic, pObj);
}

//ͨ���ű���������Ļص�������һ��userdata����lua���������а���һ������ָ��T*����userdata��gcʱ��T*��c++delete
int sle::_CB_Operator_New(lua_State *L)
{
	const char* pszClassName = GetClassNameFromTable(L, 1);
	lua_remove(L, 1); // ��һ��������table��ջ
	// ��ʱջ��ֻ�е���C++�����Ĳ�����

	SLE_RegisterFunction* pFuncInfo = (SLE_RegisterFunction*)SLE_QueryLuaElement(L, pszClassName, LUA_CONSTRUCT_METHOD, true);

	if (!CheckParams(L, pFuncInfo))
		return 0;

	// ������Ĭ�Ϲ��캯������ʱ����ֵ����userdata�Ѿ��ڶ�ջ��
	pFuncInfo->pMethod->Invoke(L);
	LuaUserDataType* pUD = (LuaUserDataType*)lua_touserdata(L, -1);
	pUD->bAutoRelease = true;
	return 1;
}

//����������GC�Ļص�
int sle::_CB_Operator_Delete(lua_State *L)
{
	LuaUserDataType* pUD = (LuaUserDataType*)(lua_touserdata(L, 1));
	if (pUD->bAutoRelease)
	{
		SLE_RegisterFunction* pFuncInfo = (SLE_RegisterFunction*)SLE_QueryLuaElement(L, pUD->pszClassName, LUA_DESTRUCT_METHOD, true);
		pFuncInfo->pMethod->Invoke(L);
	}
	return 0;
}

int sle::_CB_Operator_ToString(lua_State *L)
{
	char szBuff[SLE_BUFFER_LEN] = { 0 };
	LuaUserDataType *pUD = (LuaUserDataType*)(lua_touserdata(L, 1));
	sprintf(szBuff, "Ptr:0x%p ClassName:%s", pUD->pObj, pUD->pszClassName);
	lua_pushstring(L, szBuff);
	return 1;
}

int sle::_CB_Operator_Equal(lua_State *L)
{
	LuaUserDataType *pUD1 = (LuaUserDataType*)(lua_touserdata(L, 1));
	LuaUserDataType *pUD2 = (LuaUserDataType*)(lua_touserdata(L, 2));
	lua_pushboolean(L, pUD1->pObj == pUD2->pObj);
	return 1;
}
