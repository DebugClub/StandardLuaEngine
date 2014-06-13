#pragma once
#include "sle.h"

class TestClass1
{
	DECLARE_LUA_CLASS(TestClass1);
public:
	TestClass1()
	{
		m_nValue = 898;
	}
	int TestVoid(void*p)
 	{
		return 0;
	};
	static int TestStaticVoid(void* p)
	{
		return 0;
	};
	int TestAllParam(int n, const char* s, double d, sle::luatable t, sle::luafunc f, TestClass1* p, lua_State* L)
	{
		int a = t["nValue"];
		f("aaaaaaaaaaaaaa");
		return 999;
	}
	int m_nValue;
};
