#include "stdafx.h"
#include "luarets.h"
#include "_LuaMultiValueHolder.h"

using namespace sle;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

_luarets::_luarets(const _LuaMultiValueHolder& rcMulValueHolder)
{
	m_lpLuaMultiValueHolder = &rcMulValueHolder;
}

_luarets::~_luarets(void)
{
}

void _luarets::operator=(const _LuaMultiValueHolder& rcMulValueHolder)
{
	m_lpLuaMultiValueHolder = &rcMulValueHolder;
}

const _LuaValueHolder& _luarets::operator[](int nIdx) const
{
	const _LuaValueHolder& rcHolder = m_lpLuaMultiValueHolder->operator[](nIdx);
	return rcHolder;
}
