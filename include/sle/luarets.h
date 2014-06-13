#pragma once
#include "_SLEDefine.h"

namespace sle
{
	class _LuaValueHolder;
	class _LuaMultiValueHolder;

	class SLE_EXPORT_API _luarets
	{
	public:
		_luarets(const _LuaMultiValueHolder& rcMulValueHolder);
		virtual ~_luarets(void);
		//这个operator=是特殊的，用来处理导出函数返回值
		void operator=(const _LuaMultiValueHolder& rcMulValueHolder);

	private:
		//以下两个函数不能调用，因为无意义，防止误调用，直接private
		_luarets(const _luarets&) {};
		virtual void operator=(const _luarets&) {}

	public:
		virtual const _LuaValueHolder& operator[](int nIdx) const;

	protected:
		const _LuaMultiValueHolder *m_lpLuaMultiValueHolder;
	};

	typedef const _luarets& luarets;
}
