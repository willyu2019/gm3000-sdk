/*
[]=========================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	common.h

DESC:
	Common def file!
[]=========================================================================[]
*/
	
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cassert>
#include <iostream>
using namespace std;

#include <conio.h>
#include <malloc.h>
#include <Windows.h>

#include "include/cryptoki.h"

#define MAX_PIN_LEN	 8	// Max length of user PIN.
#define MODULUS_BIT_LENGTH 1024 	// size of RSA key in bits

#define	countof(a)			(sizeof(a)/ sizeof(CK_ATTRIBUTE))


#define START_OP(szMsg)												\
{																	\
	printf ("\nOP: %s", szMsg);									\
}

#define CHECK_OP(rvRet)												\
{																	\
	if(CKR_OK == (rvRet))											\
	{																\
		printf ("....[OK].");										\
	}																\
	else															\
	{																\
		printf ("....[FAILED]\n\t\t\tErrCode = 0x%08X.", (rvRet));	\
		break;														\
	}																\
}


#define SHOW_ERROR(szMsg)											\
{																	\
	printf ("\n\nSTOP! %s!", (szMsg));								\
	break;															\
}

#define SHOW_INFO(szMsg)											\
{																	\
	printf ("%s", (szMsg));	 										\
}

void ShowTitle(void);

void ShowData(unsigned char *pData, unsigned long ulDataLen);