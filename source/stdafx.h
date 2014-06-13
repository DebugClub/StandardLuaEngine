#pragma once

#ifdef _WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#if defined(DEBUG) | defined(_DEBUG)
		#ifndef DEBUG_NEW
			void* __cdecl operator new(size_t nSize, const char* lpszFileName, int nLine);
			void* __cdecl operator new[](size_t nSize, int nType, const char* lpszFileName, int nLine);
			#ifndef _NORMAL_BLOCK
			#define _NORMAL_BLOCK 1
			#endif
			#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
		#endif
	#endif
#endif

#define _HAS_EXCEPTIONS 0

#include <stdio.h>
#include <assert.h>
