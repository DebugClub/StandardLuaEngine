#include "stdafx.h"
typedef unsigned char BYTE;
typedef unsigned long DWORD;
#include "LuaPacker.h"
#include "minilzo.h"
#include <string.h>

//  warning C4800: “int”: 将值强制为布尔值“true”或“false”(性能警告)
// #pragma warning(disable:4800)
#define LZO_COMPRESS_BUFFER_SIZE(InputSize) (InputSize + (InputSize / 16) + 64 + 3)

enum KLuaValueDef
{
	eLuaPackNumber,
	eLuaPackBoolean,
	eLuaPackString,
	eLuaPackNill,
	eLuaPackTable,
};

_LuaPaker::_LuaPaker(size_t uBufferSize)
{
	m_pbyBuffer     = new BYTE[uBufferSize];
	m_uBufferSize   = uBufferSize;
	m_pbyPos        = m_pbyBuffer;
	m_pbyEnd        = m_pbyBuffer + uBufferSize;
	m_bIsOverflow   = false;
	m_nCallStack    = 0;
}

_LuaPaker::~_LuaPaker()
{
	DELETE_ARRAY(m_pbyBuffer);
}

bool _LuaPaker::Save(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize)
{
	bool     bResult            = false;
	int      nRetCode           = 0;
	BYTE*    pbyLzoWorkMem      = NULL;
	lzo_uint uOutSize           = uBufferSize;
	DWORD    uSrcBufferSize     = (DWORD)(m_pbyPos - m_pbyBuffer);

	SLE_ASSERT_JUMP(!m_bIsOverflow);
	SLE_ASSERT_JUMP(uBufferSize >= LZO_COMPRESS_BUFFER_SIZE(uSrcBufferSize));

	pbyLzoWorkMem = new BYTE[LZO1X_1_MEM_COMPRESS];

	nRetCode = lzo1x_1_compress(
		m_pbyBuffer, uSrcBufferSize, 
		pbyBuffer, &uOutSize, pbyLzoWorkMem
		);
	SLE_ASSERT_JUMP(nRetCode == LZO_E_OK);

	*puUsedSize = uOutSize;

	bResult = true;
Exit0:
	DELETE_ARRAY(pbyLzoWorkMem);
	return bResult;
}

bool _LuaPaker::PushNumber(double fValue)
{
	bool bResult = false;

	SLE_FAIL_JUMP((size_t)(m_pbyEnd - m_pbyPos) >= sizeof(BYTE));
	*m_pbyPos++ = eLuaPackNumber;

	SLE_FAIL_JUMP((size_t)(m_pbyEnd - m_pbyPos) >= sizeof(double));
	*(double*)m_pbyPos = fValue;
	m_pbyPos   += sizeof(double);

	bResult = true;
Exit0:
	if (!bResult)
	{
		m_bIsOverflow = true;
	}
	return bResult;
}

bool _LuaPaker::PushBool(bool bValue)
{
	bool bResult = false;

	SLE_FAIL_JUMP((size_t)(m_pbyEnd - m_pbyPos) >= sizeof(BYTE));
	*m_pbyPos++ = eLuaPackBoolean;

	SLE_FAIL_JUMP((size_t)(m_pbyEnd - m_pbyPos) >= sizeof(bool));
	*(bool*)m_pbyPos = (bool)bValue;
	m_pbyPos   += sizeof(bool);

	bResult = true;
Exit0:
	if (!bResult)
	{
		m_bIsOverflow = true;
	}
	return bResult;
}

bool _LuaPaker::PushString(const char cszValue[])
{
	return PushLString(cszValue, strlen(cszValue));
}

bool _LuaPaker::PushNill()
{
	bool bResult = false;

	SLE_FAIL_JUMP((size_t)(m_pbyEnd - m_pbyPos) >= sizeof(BYTE));
	*m_pbyPos++ = eLuaPackNill;

	bResult = true;
Exit0:
	if (!bResult)
	{
		m_bIsOverflow = true;
	}
	return bResult;
}

BYTE* _LuaPaker::NewTable()
{
	BYTE* pbyResult = NULL;

	SLE_FAIL_JUMP((size_t)(m_pbyEnd - m_pbyPos) >= sizeof(BYTE));
	*m_pbyPos++ = eLuaPackTable;

	SLE_FAIL_JUMP((size_t)(m_pbyEnd - m_pbyPos) >= sizeof(DWORD));
	*(DWORD*)m_pbyPos = 0;
	m_pbyPos   += sizeof(DWORD);

	pbyResult = m_pbyPos;
Exit0:
	if (!pbyResult)
	{
		m_bIsOverflow = true;
	}
	return pbyResult;
}

void _LuaPaker::SetTable(BYTE* pbyTable)
{
	bool  bResult   = false;
	BYTE* pbyType   = pbyTable - sizeof(DWORD) - sizeof(BYTE);
	BYTE* pbyLength = pbyTable - sizeof(DWORD);

	if (!m_bIsOverflow)
	{
		assert(*pbyType == eLuaPackTable);
		*(DWORD*)pbyLength = (DWORD)(m_pbyPos - pbyTable);
	}
}

bool _LuaPaker::PushValue(lua_State* L, int nValueIndex, int nCount)
{
	bool    bResult     = false;
	bool    bRetCode    = false;

	assert(L);

	m_nCallStack++; // 检测递归深度        
	SLE_ASSERT_JUMP(m_nCallStack < 16);

	for (int i = 0; i < nCount; i++)
	{
		int     nIndex      = nValueIndex + i;
		int     nType       = lua_type(L, nIndex);

		switch (nType)
		{
		case LUA_TNUMBER:
			{
				double fValue = lua_tonumber(L, nIndex);
				bRetCode = PushNumber(fValue);
			}
			break;

		case LUA_TBOOLEAN:
			{
				bool bValue = !!lua_toboolean(L, nIndex);
				bRetCode = PushBool(bValue);
			}
			break;

		case LUA_TSTRING:
			{
				size_t uLen = 0;
				const char* pszValue = lua_tolstring(L, nIndex, &uLen);
				SLE_FAIL_JUMP(pszValue);
				bRetCode = PushLString(pszValue, uLen);
			}
			break;

		case LUA_TNIL:
			{
				bRetCode = PushNill();
			}
			break;

		case LUA_TTABLE:
			bRetCode = PushTable(L, nIndex);
			break;

		default:
			bRetCode = true;
			break;
		}

		SLE_FAIL_JUMP(bRetCode);

	}

	bResult = true;
Exit0:
	if (!bResult)
	{
		m_bIsOverflow = true;
	}
	m_nCallStack--;
	return bResult;
}

void  _LuaPaker::Reset()
{
	m_pbyPos        = m_pbyBuffer;
	m_pbyEnd        = m_pbyBuffer + m_uBufferSize;
	m_bIsOverflow   = false;
	m_nCallStack    = 0;
}

bool _LuaPaker::PushTable(lua_State* L, int nIndex)
{
	bool    bResult    = false;
	bool    bRetCode   = false;
	BYTE*   pbyTable   = NULL;

	assert(lua_istable(L, nIndex));

	pbyTable = NewTable();
	SLE_FAIL_JUMP(pbyTable);

	lua_pushnil(L);

	if (nIndex < 0) // pushnil会改变相对索引
		nIndex--;

	while (lua_next(L, nIndex))
	{
		int nTopIndex = lua_gettop(L);

		bRetCode = PushValue(L, nTopIndex - 1);
		SLE_FAIL_JUMP(bRetCode);

		bRetCode = PushValue(L, nTopIndex);
		SLE_FAIL_JUMP(bRetCode);

		SetTable(pbyTable);

		lua_pop(L, 1);
	}

	bResult = true;
Exit0:
	if (!bResult)
	{
		m_bIsOverflow = true;
	}
	return bResult;
}

bool _LuaPaker::PushLString(const char* pszValue, size_t uLen)
{
	bool bResult = false;

	assert(pszValue);

	SLE_FAIL_JUMP((size_t)(m_pbyEnd - m_pbyPos) >= sizeof(BYTE));
	*m_pbyPos++ = eLuaPackString;

	SLE_FAIL_JUMP((size_t)(m_pbyEnd - m_pbyPos) >= sizeof(DWORD));
	*(DWORD*)m_pbyPos = (DWORD)uLen;
	m_pbyPos += sizeof(DWORD);

	SLE_FAIL_JUMP((size_t)(m_pbyEnd - m_pbyPos) >= uLen);
	memcpy(m_pbyPos, pszValue, uLen);
	m_pbyPos += uLen;

	bResult = true;
Exit0:
	if (!bResult)
	{
		m_bIsOverflow = true;
	}
	return bResult;
}

_LuaUnpaker::_LuaUnpaker(size_t uBufferSize)
{
	m_pbyBuffer     = new BYTE[uBufferSize];
	m_uBufferSize   = uBufferSize;
}

_LuaUnpaker::~_LuaUnpaker()
{
	DELETE_ARRAY(m_pbyBuffer);
}

int _LuaUnpaker::Expand(lua_State* L, BYTE* pbyData, size_t uDataLen)
{
	int         nCount     = 0;
	int         nRetCode   = 0;
	lzo_uint    uOutSize   = m_uBufferSize;
	BYTE*       pbyPos     = m_pbyBuffer;
	BYTE*       pbyEnd     = 0;

	nRetCode = lzo1x_decompress_safe(
		pbyData, uDataLen,
		m_pbyBuffer, &uOutSize, NULL
		);
	SLE_ASSERT_JUMP(nRetCode == LZO_E_OK);

	pbyEnd = m_pbyBuffer + uOutSize; 

	while (pbyPos < pbyEnd)
	{
		pbyPos = ExpandValue(L, pbyPos, size_t(pbyEnd - pbyPos));
		SLE_ASSERT_JUMP(pbyPos);
		nCount++;
	}

Exit0:
	return nCount;
}

BYTE* _LuaUnpaker::ExpandValue(lua_State* L, BYTE* pbyData, size_t uDataLen)
{
	BYTE*   pbyResult   = NULL;
	bool    bRetCode    = false;
	BYTE*   pbyPos      = pbyData;
	BYTE*   pbyEnd      = pbyData + uDataLen;
	int     nType       = 0;

	SLE_FAIL_JUMP((size_t)(pbyEnd - pbyPos) >= sizeof(BYTE));
	nType = *pbyPos++;

	switch (nType)
	{
	case eLuaPackNumber:
		{
			double fValue = 0.0f;
			SLE_FAIL_JUMP((size_t)(pbyEnd - pbyPos) >= sizeof(double));
			fValue = *(double*)pbyPos;
			pbyPos += sizeof(double);
			lua_pushnumber(L, fValue);
		}
		break;

	case eLuaPackBoolean:
		{
			bool bValue = false;
			SLE_FAIL_JUMP((size_t)(pbyEnd - pbyPos) >= sizeof(bool));
			bValue = *(bool*)pbyPos;
			pbyPos += sizeof(bool);
			lua_pushboolean(L, bValue);
		}
		break;

	case eLuaPackString:
		{
			size_t      uLen        = 0;
			const char* pszValue    = NULL;(char*)pbyPos;

			SLE_FAIL_JUMP((size_t)(pbyEnd - pbyPos) >= sizeof(DWORD));
			uLen = (size_t)*(DWORD*)pbyPos;
			pbyPos += sizeof(DWORD);

			SLE_FAIL_JUMP((size_t)(pbyEnd - pbyPos) >= uLen);
			lua_pushlstring(L, (const char*)pbyPos, uLen);
			pbyPos += uLen;
		}
		break;

	case eLuaPackNill:
		lua_pushnil(L);
		break;

	case eLuaPackTable:
		{
			DWORD           dwSize      = 0;

			SLE_FAIL_JUMP((size_t)(pbyEnd - pbyPos) >= sizeof(DWORD));
			dwSize    = *(DWORD*)pbyPos;
			pbyPos   += sizeof(DWORD);

			SLE_FAIL_JUMP((size_t)(pbyEnd - pbyPos) >= dwSize);
			bRetCode = ExpandTable(L, pbyPos, dwSize);
			SLE_FAIL_JUMP(bRetCode);
			pbyPos += dwSize;
		}
		break;

	default:        
		goto Exit0;
	}

	pbyResult = pbyPos;
Exit0:
	return pbyResult;
}

bool _LuaUnpaker::ExpandTable(lua_State* L, BYTE* pbyTable, size_t uTableLen)
{
	bool    bResult   = false;
	bool    bRetCode  = false;
	BYTE*   pbyOffset = pbyTable;
	BYTE*   pbyTabEnd = pbyTable + uTableLen;

	lua_newtable(L);

	while (pbyOffset < pbyTabEnd)
	{
		pbyOffset = ExpandValue(L, pbyOffset, (size_t)(pbyTabEnd - pbyOffset));
		SLE_FAIL_JUMP(pbyOffset);

		pbyOffset = ExpandValue(L, pbyOffset, (size_t)(pbyTabEnd - pbyOffset));
		SLE_FAIL_JUMP(pbyOffset);

		lua_settable(L, -3);
	}

	bResult = true;
Exit0:
	return bResult;
}

