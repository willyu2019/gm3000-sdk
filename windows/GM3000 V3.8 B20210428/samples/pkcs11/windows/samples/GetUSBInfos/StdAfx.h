// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2F6FAC04_EA2A_4ED3_AD5D_ABBB5918A60C__INCLUDED_)
#define AFX_STDAFX_H__2F6FAC04_EA2A_4ED3_AD5D_ABBB5918A60C__INCLUDED_

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#undef WIN32
#define WIN32
#elif defined(linux) || defined(__linux) || defined(__linux__)
#undef linux
#define linux
#define FreeLibrary			dlclose
#define GetProcAddress		dlsym
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#undef macintosh
#define macintosh
#define FreeLibrary			dlclose
#define GetProcAddress		dlsym
#endif

#if defined(WIN32)
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "include/cryptoki.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "GetInfos.h"  

// TODO: reference additional headers your program requires here


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif

#endif // !defined(AFX_STDAFX_H__2F6FAC04_EA2A_4ED3_AD5D_ABBB5918A60C__INCLUDED_)
