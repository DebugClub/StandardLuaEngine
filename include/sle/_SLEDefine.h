#pragma once

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#ifdef _WIN64
typedef __int64             INT_PTR;
#else
typedef _W64 int            INT_PTR;
#endif // WIN64

#define SLE_API			// Lua和C都可以调用的接口
#define SLE_API_ONLY	// 仅供Lua调用的接口

#define LUA_TYPE_COUNT		11		// lua类型数量
#define LUA_TSTATE			11		// lua_State*
#define LUA_TOBJECT			12		// 所有从luaclass派生出的类，即脚本对象
#define LUA_TVARIABLE		13		// 任意lua类型

#ifdef _MSC_VER
#pragma warning(disable:4996) // sprintf之类的警告
//#pragma warning(disable:4800) 
#endif

#ifdef _USRDLL
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#else
#define DLL_EXPORT
#define DLL_IMPORT
#endif

#ifdef SLE_EXPORTS
#define SLE_EXPORT_API DLL_EXPORT
#else
#define SLE_EXPORT_API DLL_IMPORT
#endif

namespace sle
{
	SLE_EXPORT_API void _SLE_ASSERT(const char * _Message, const char *_File, unsigned _Line);
}

#define SLE_ASSERT(_Expression) (void)( (!!(_Expression)) || (sle::_SLE_ASSERT(#_Expression, __FILE__, __LINE__), 0) )

// 各种名字的最大程度
#define SLE_MAX_NAME_LEN 255

// 一般使用的缓冲区大小
#define SLE_BUFFER_LEN 1024

// 这个宏最好不要改，改了还要改代码，因为参数处理过程是写死的
#define SLE_MAX_PARAMS_COUNT 12

#define DECLARE_TYPENAME0 
#define DECLARE_TYPENAME1 DECLARE_TYPENAME0  typename T1
#define DECLARE_TYPENAME2 DECLARE_TYPENAME1, typename T2
#define DECLARE_TYPENAME3 DECLARE_TYPENAME2, typename T3
#define DECLARE_TYPENAME4 DECLARE_TYPENAME3, typename T4
#define DECLARE_TYPENAME5 DECLARE_TYPENAME4, typename T5
#define DECLARE_TYPENAME6 DECLARE_TYPENAME5, typename T6
#define DECLARE_TYPENAME7 DECLARE_TYPENAME6, typename T7
#define DECLARE_TYPENAME8 DECLARE_TYPENAME7, typename T8
#define DECLARE_TYPENAME9 DECLARE_TYPENAME8, typename T9
#define DECLARE_TYPENAME10 DECLARE_TYPENAME9, typename T10
#define DECLARE_TYPENAME11 DECLARE_TYPENAME10, typename T11
#define DECLARE_TYPENAME12 DECLARE_TYPENAME11, typename T12

#define DECLARE_REF_ARG0 
#define DECLARE_REF_ARG1 DECLARE_REF_ARG0  T1 &arg1
#define DECLARE_REF_ARG2 DECLARE_REF_ARG1, T2 &arg2
#define DECLARE_REF_ARG3 DECLARE_REF_ARG2, T3 &arg3
#define DECLARE_REF_ARG4 DECLARE_REF_ARG3, T4 &arg4
#define DECLARE_REF_ARG5 DECLARE_REF_ARG4, T5 &arg5
#define DECLARE_REF_ARG6 DECLARE_REF_ARG5, T6 &arg6
#define DECLARE_REF_ARG7 DECLARE_REF_ARG6, T7 &arg7
#define DECLARE_REF_ARG8 DECLARE_REF_ARG7, T8 &arg8
#define DECLARE_REF_ARG9 DECLARE_REF_ARG8, T9 &arg9
#define DECLARE_REF_ARG10 DECLARE_REF_ARG9, T10 &arg10
#define DECLARE_REF_ARG11 DECLARE_REF_ARG10, T11 &arg11
#define DECLARE_REF_ARG12 DECLARE_REF_ARG11, T12 &arg12

#define DECLARE_ARG0 
#define DECLARE_ARG1 DECLARE_ARG0  T1 arg1
#define DECLARE_ARG2 DECLARE_ARG1, T2 arg2
#define DECLARE_ARG3 DECLARE_ARG2, T3 arg3
#define DECLARE_ARG4 DECLARE_ARG3, T4 arg4
#define DECLARE_ARG5 DECLARE_ARG4, T5 arg5
#define DECLARE_ARG6 DECLARE_ARG5, T6 arg6
#define DECLARE_ARG7 DECLARE_ARG6, T7 arg7
#define DECLARE_ARG8 DECLARE_ARG7, T8 arg8
#define DECLARE_ARG9 DECLARE_ARG8, T9 arg9
#define DECLARE_ARG10 DECLARE_ARG9, T10 arg10
#define DECLARE_ARG11 DECLARE_ARG10, T11 arg11
#define DECLARE_ARG12 DECLARE_ARG11, T12 arg12

#define DECLARE_ARG_LIST0
#define DECLARE_ARG_LIST1 DECLARE_ARG_LIST0 arg1
#define DECLARE_ARG_LIST2 DECLARE_ARG_LIST1, arg2
#define DECLARE_ARG_LIST3 DECLARE_ARG_LIST2, arg3
#define DECLARE_ARG_LIST4 DECLARE_ARG_LIST3, arg4
#define DECLARE_ARG_LIST5 DECLARE_ARG_LIST4, arg5
#define DECLARE_ARG_LIST6 DECLARE_ARG_LIST5, arg6
#define DECLARE_ARG_LIST7 DECLARE_ARG_LIST6, arg7
#define DECLARE_ARG_LIST8 DECLARE_ARG_LIST7, arg8
#define DECLARE_ARG_LIST9 DECLARE_ARG_LIST8, arg9
#define DECLARE_ARG_LIST10 DECLARE_ARG_LIST9, arg10
#define DECLARE_ARG_LIST11 DECLARE_ARG_LIST10, arg11
#define DECLARE_ARG_LIST12 DECLARE_ARG_LIST11, arg12

#define DECLARE_TYPE_LIST0
#define DECLARE_TYPE_LIST1 DECLARE_TYPE_LIST0 T1
#define DECLARE_TYPE_LIST2 DECLARE_TYPE_LIST1, T2
#define DECLARE_TYPE_LIST3 DECLARE_TYPE_LIST2, T3
#define DECLARE_TYPE_LIST4 DECLARE_TYPE_LIST3, T4
#define DECLARE_TYPE_LIST5 DECLARE_TYPE_LIST4, T5
#define DECLARE_TYPE_LIST6 DECLARE_TYPE_LIST5, T6
#define DECLARE_TYPE_LIST7 DECLARE_TYPE_LIST6, T7
#define DECLARE_TYPE_LIST8 DECLARE_TYPE_LIST7, T8
#define DECLARE_TYPE_LIST9 DECLARE_TYPE_LIST8, T9
#define DECLARE_TYPE_LIST10 DECLARE_TYPE_LIST9, T10
#define DECLARE_TYPE_LIST11 DECLARE_TYPE_LIST10, T11
#define DECLARE_TYPE_LIST12 DECLARE_TYPE_LIST11, T12

// 在导出类的table中用于保存table名，即ClassName的字段名称
#define SZ_CLASS_NAME_FIELD		"__SZ_CLASS_NAME__"
#define SZ_StaticOverride		"__StaticOverride"
#define SZ_MemberOverride		"__MemberOverride"

namespace sle
{
	enum SLE_RUNTIME_ERROR_TYPE
	{
		emSLE_BeginError = -10000,
		emSLE_TypeError,				// 类型错误
		emSLE_ParamNumError,			// 参数个数错误
		emSLE_BufferSizeError,			// 字符串长度超出数组大小
		emSLE_IndexError,				// 寻址错误，即元素不能push
		emSLE_KeyError,					// 无效的key
		emSLE_NilError,					// 无效的nil值
		emSLE_ReadonlyError,			// 写只读变量
		emSLE_AccessMemberFieldError,	// 通过类名访问了成员函数、或成员变量
		emSLE_EndError
	};

	enum SLE_REGISTER_TYPE
	{
		emSLE_RegisterFunction,
		emSLE_RegisterVariable,
	};

	class _IMethodHolder;

	struct SLE_RegisterElement
	{
		SLE_REGISTER_TYPE eType;
		const char* szClassName;			// 成员所属类的名字
		const char* szName;					// 成员的名字
		bool bStatic;						// 是否是静态成员
	};

	typedef struct _RegisterFunction : SLE_RegisterElement
	{
		int ParamsType[SLE_MAX_PARAMS_COUNT];	// 转换成Lua类型后的参数描述
		int nMinParamsCount;					// 最小参数个数
		int nMaxParamsCount;					// 最大参数个数
		_IMethodHolder *pMethod;				// 目标函数的Holder
	} SLE_RegisterFunction;

	typedef struct _RegisterVariable : SLE_RegisterElement
	{
		// 如果是成员变量，则表示变量在对象中的位置偏移
		// 如果是静态变量，则表示变量在的实际地址
		void* pAddress;
		int nSize;							// 如果是char*，则表示长度
		_IMethodHolder *pReadMethod;		// 读变量的回调函数的Holder
		_IMethodHolder *pWriteMethod;		// 写变量的回调函数的Holder
		bool bReadonly;						// 只读
	} SLE_RegisterVariable;

	typedef struct _LuaUserDataType
	{
		void *pObj;					// 具体类的指针
		bool bAutoRelease;			// 指示GC时是否delete对象
		const char* pszClassName;	// 类的名字
	} LuaUserDataType;

	typedef bool (__stdcall *LUA_ERROR_PROC)(lua_State* L);

#pragma region Type Translate
	// remove_const
	template<typename T>
	struct _SLE_REMOVE_CONST 
	{
		typedef T type;
	};

	template<typename T>
	struct _SLE_REMOVE_CONST<const T> 
	{
		typedef T type;
	};

	// remove_reference
	template<typename T>
	struct _SLE_REMOVE_REFERENCE 
	{
		typedef T type;
	};

	template<typename T>
	struct _SLE_REMOVE_REFERENCE<T&> 
	{
		typedef T type;
	};

	// remove_pointer
	template<typename T>
	struct _SLE_REMOVE_POINTER
	{
		typedef T type;
	};

	template<typename T>
	struct _SLE_REMOVE_POINTER<T*> 
	{
		typedef T type;
	};

	// remove_const_referance_pointer
	template<typename T>
	struct _SLE_GET_PURE_TYPE
	{
		typedef typename _SLE_REMOVE_POINTER<typename _SLE_REMOVE_REFERENCE<typename _SLE_REMOVE_CONST<T>::type>::type>::type type;
	};

	template <typename T1, typename T2>
	struct _SLE_Translate
	{
		static T2 GetValue(void* pObj)	{ err; }
	};

	template <typename T1>
	struct _SLE_Translate<T1, T1*>
	{
		static T1* GetValue(void* pObj)
		{
			return (T1*)pObj;
		}
	};

	template <typename T1>
	struct _SLE_Translate<T1, T1&>
	{
		static T1& GetValue(void* pObj)
		{
			return *(T1*)pObj;
		}
	};

#pragma endregion

}

#define DELETE_POINTER(p)	do { if (p) delete p; p = NULL; } while(false);
#define DELETE_ARRAY(p)		do { if (p) delete[] p; p = NULL; } while(false);
#define SLE_FAIL_RETURN(p)	do { if (!(p)) return 0; } while(false);
#define SLE_FAIL_JUMP(p)	do { if (!(p)) goto Exit0; } while(false);
#define SLE_ASSERT_RETURN(p)	do { if (!(p)) { SLE_ASSERT(false); return 0;} } while(false);
#define SLE_ASSERT_JUMP(p)	do { if (!(p)) { SLE_ASSERT(false); goto Exit0;} } while(false);
