#pragma once
#include "_SLEDefine.h"

// 下面一系列函数用于在内存中填充一个用于传输/存储Lua变量的缓冲区
// 返回值均表示打包参数存储区之后的地址,出错则返回NULL

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

	// 可以在push的过程中不检查返回值,最后才判断是否发生了溢出
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

	// 返回展开值个数
	int  Expand(lua_State* L, BYTE* pbyData, size_t uDataLen);

private:
	BYTE* ExpandValue(lua_State* L, BYTE* pbyData, size_t uDataLen);
	bool  ExpandTable(lua_State* L, BYTE* pbyTable, size_t uTableLen);

	BYTE*           m_pbyBuffer;
	size_t          m_uBufferSize;
};
