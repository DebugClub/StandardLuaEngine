// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						
#pragma warning(disable:4251)

#include <stdio.h>
#include <tchar.h>

#include <assert.h>
#define ASSERT(arg) assert(arg)

/*
#define CHECK_POINTER(p)	if (!p) ASSERT(false);

#define DELETE_POINTER(p) { if (p) delete p; p = NULL; }
*/
//#define RETURN_FALSE_ON_FAIL(condiction) { if (!(condiction)) { ASSERT(false); return false; } }


// TODO: reference additional headers your program requires here
