#pragma once
#include "_SLEDefine.h"
#include "_LuaRefBase.h"
#include "_LuaMultiValueHolder.h"

#define PUSH_VALUE_OPT(i) SLE_Push(m_pLuaState, arg##i);

#define PUSH_VALUE0
#define PUSH_VALUE1 PUSH_VALUE0 PUSH_VALUE_OPT(1)
#define PUSH_VALUE2 PUSH_VALUE1 PUSH_VALUE_OPT(2)
#define PUSH_VALUE3 PUSH_VALUE2 PUSH_VALUE_OPT(3)
#define PUSH_VALUE4 PUSH_VALUE3 PUSH_VALUE_OPT(4)
#define PUSH_VALUE5 PUSH_VALUE4 PUSH_VALUE_OPT(5)
#define PUSH_VALUE6 PUSH_VALUE5 PUSH_VALUE_OPT(6)
#define PUSH_VALUE7 PUSH_VALUE6 PUSH_VALUE_OPT(7)
#define PUSH_VALUE8 PUSH_VALUE7 PUSH_VALUE_OPT(8)
#define PUSH_VALUE9 PUSH_VALUE8 PUSH_VALUE_OPT(9)
#define PUSH_VALUE10 PUSH_VALUE9 PUSH_VALUE_OPT(10)
#define PUSH_VALUE11 PUSH_VALUE10 PUSH_VALUE_OPT(11)
#define PUSH_VALUE12 PUSH_VALUE11 PUSH_VALUE_OPT(12)

#define DECLARE_INVOKE_METHOD(ARG_NUMBER) \
	template <DECLARE_TYPENAME##ARG_NUMBER> \
	_LuaMultiValueHolder& invoke(DECLARE_ARG##ARG_NUMBER) \
	{ \
		_Push(); \
		PUSH_VALUE##ARG_NUMBER \
		return _Call(ARG_NUMBER); \
	} \
	template <DECLARE_TYPENAME##ARG_NUMBER> \
	_LuaMultiValueHolder& operator()(DECLARE_ARG##ARG_NUMBER) \
	{ \
		return invoke(DECLARE_ARG_LIST##ARG_NUMBER); \
	}

namespace sle
{
	class _LuaMultiValueHolder;
	class SLE_EXPORT_API luafunc : public _LuaRefBase
	{
	public:
		luafunc();
		luafunc(lua_State* L);
		luafunc(const luafunc& rhl);
		luafunc(const _LuaRefBase& rhl);
		virtual ~luafunc();
		virtual luafunc& operator=(const luafunc& rhl);

	public:
		//没有参数的函数调用
		_LuaMultiValueHolder& Invoke()
		{
			_Push();
			return _Call(0);
		}
		_LuaMultiValueHolder& operator()()
		{
			return Invoke();
		}
		DECLARE_INVOKE_METHOD(1)
		DECLARE_INVOKE_METHOD(2)
		DECLARE_INVOKE_METHOD(3)
		DECLARE_INVOKE_METHOD(4)
		DECLARE_INVOKE_METHOD(5)
		DECLARE_INVOKE_METHOD(6)
		DECLARE_INVOKE_METHOD(7)
		DECLARE_INVOKE_METHOD(8)
		DECLARE_INVOKE_METHOD(9)
		DECLARE_INVOKE_METHOD(10)
		DECLARE_INVOKE_METHOD(11)
		DECLARE_INVOKE_METHOD(12)

		virtual _LuaMultiValueHolder& _Call(int nArgs);

		int GetLastErrCode();

	protected:
		int m_nErrCode;
		_LuaMultiValueHolder* m_pLuaMultiValueHolder;
	};
}
