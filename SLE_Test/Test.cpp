#include "stdafx.h"
#include <iostream>
#include "sle.h"
#include "_LuaMultiValueHolder.h"
//#include "lua.hpp"
#include "time.h"
#include "defclass.h"
#include <string>
#include <Windows.h>
using namespace sle;
using namespace std;

class XPerfTestBase
{
public:
	XPerfTestBase(int nTestCount)
	{
		m_nTestCount = nTestCount;
	}
	virtual void Test_Raw() = 0;
	virtual void Test_SLE() = 0;
	virtual const char* TestName() = 0;
public:
	lua_State* L;
	luaevn* E;
	int m_nTestCount;
};

class XPerfTest01 : public XPerfTestBase
{
public:
	XPerfTest01(int nTestCount) : XPerfTestBase(nTestCount)
	{

	}

	virtual const char* TestName()
	{
		return "Table Access & Assign";
	}

	virtual void Test_Raw()
	{
		char szField[64];
		int nTopIndex = lua_gettop(L);
		lua_newtable(L);
		lua_setglobal(L, "XPerfTest01RawTable");
		lua_getglobal(L, "XPerfTest01RawTable");
		int nTable = lua_gettop(L);
		for (int i = 0; i < m_nTestCount; ++i)
		{
			sprintf(szField, "FieldRaw%d", i);
			lua_pushnumber(L, i);
			lua_setfield(L, -2, szField);
		}
		lua_pop(L, 1);
		assert(nTopIndex == lua_gettop(L));
	}

	virtual void Test_SLE()
	{
		char szField[64];
		luatable tb = E->newtable("TestTable");
		for (int i = 0; i < m_nTestCount; ++i)
		{
			sprintf(szField, "FieldSLE%d", i);
			tb[szField] = i;
			assert((int)tb[szField] == i);
		}
	}
};

class XPerfTest02 : public XPerfTestBase
{
public:
	XPerfTest02(int nTestCount) : XPerfTestBase(nTestCount)
	{

	}

	virtual const char* TestName()
	{
		return "Invoke Lua Func";
	}

	virtual void Test_Raw()
	{
		for (int i = 0; i < m_nTestCount; ++i)
		{
			lua_getglobal(L, "TestGlobalFunc");
			lua_pushinteger(L, 9999);
			lua_pushstring(L, "TestString");
			lua_call(L, 2, 1);
			int nResult = (int)lua_tointeger(L, -1);
			lua_pop(L, 1);
			assert(nResult == 1);
		}
	}

	virtual void Test_SLE()
	{
		luafunc lf(L);
		for (int i = 0; i < m_nTestCount; ++i)
		{
			lf._SetRefrence("TestGlobalFunc");
			int nResult = lf(9999, "TestString");
			assert(nResult == 1);
		}
	}
};

XPerfTestBase* TestProc[] =
{
	new XPerfTest01(100000),
 	new XPerfTest02(100000),
	NULL,
};

INT _tmain(INT argc, _TCHAR* argv[])
{
	luaevn e;
	XPerfTestBase** pTestAddr = TestProc;
	XPerfTestBase* pTest = *pTestAddr;
	clock_t timeRaw, timeSLE;
	bool bResult = false;

	e.create();
	bResult = e.dofile("test.lua");
	assert(bResult);

	while (pTest)
	{
		pTest->E = &e;
		pTest->L = e.getLuaState();
		luaL_dostring(e.getLuaState(), "FFFFFFFF={}");

		lua_gc(e.getLuaState(), LUA_GCCOLLECT, 0);

		timeRaw = clock();
		pTest->Test_Raw();
		timeRaw = clock() - timeRaw;

		lua_gc(e.getLuaState(), LUA_GCCOLLECT, 0);
		
		timeSLE = clock();
		pTest->Test_SLE();
		timeSLE = clock() - timeSLE;
		
		printf("%s, Raw/SLE = %d/%d => %.2f\n", pTest->TestName(), timeRaw, timeSLE, (float)timeSLE / timeRaw);

		pTest = *(++pTestAddr);
	}

	/*TestClass1 c;
	REGISTER_TO_LUA(e.getLuaState(), TestClass1);
	e.dofile("test.lua");*/
	system("pause");
	return 0;
}
