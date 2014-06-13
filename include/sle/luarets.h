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
		//���operator=������ģ�������������������ֵ
		void operator=(const _LuaMultiValueHolder& rcMulValueHolder);

	private:
		//���������������ܵ��ã���Ϊ�����壬��ֹ����ã�ֱ��private
		_luarets(const _luarets&) {};
		virtual void operator=(const _luarets&) {}

	public:
		virtual const _LuaValueHolder& operator[](int nIdx) const;

	protected:
		const _LuaMultiValueHolder *m_lpLuaMultiValueHolder;
	};

	typedef const _luarets& luarets;
}
