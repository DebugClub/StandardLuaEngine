#pragma once
#include "_SLEDefine.h"

// ����һϵ�к����������ڴ������һ�����ڴ���/�洢Lua�����Ļ�����
// ����ֵ����ʾ��������洢��֮��ĵ�ַ,�����򷵻�NULL

struct SLE_EXPORT_API _LuaPaker
{
public:
	_LuaPaker(size_t uBufferSize = 1024 * 64);
	~_LuaPaker();

	bool Save(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize); // Compress

	bool PushNumber(double fValue);
	bool PushBool(bool bValue);
	bool PushString(const char cszValue[]);
	bool PushNill();

	BYTE* NewTable();
	void  SetTable(BYTE* pbyTable);

	bool  PushValue(lua_State* L, int nValueIndex, int nCount = 1);

	// ������push�Ĺ����в���鷵��ֵ,�����ж��Ƿ��������
	bool  IsOverflow() { return m_bIsOverflow; }

	void  Reset();

private:
	bool  PushTable(lua_State* L, int nIndex);
	bool  PushLString(const char* pszValue, size_t uLen);

	BYTE*   m_pbyBuffer;
	size_t  m_uBufferSize;
	BYTE*   m_pbyPos;
	BYTE*   m_pbyEnd;
	bool    m_bIsOverflow;
	int     m_nCallStack;
};

struct SLE_EXPORT_API _LuaUnpaker
{
public:
	_LuaUnpaker(size_t uBufferSize = 1024 * 64);
	~_LuaUnpaker();

	// ����չ��ֵ����
	int  Expand(lua_State* L, BYTE* pbyData, size_t uDataLen);

private:
	BYTE* ExpandValue(lua_State* L, BYTE* pbyData, size_t uDataLen);
	bool  ExpandTable(lua_State* L, BYTE* pbyTable, size_t uTableLen);

	BYTE*           m_pbyBuffer;
	size_t          m_uBufferSize;
};
