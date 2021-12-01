
	
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cassert>
#include <string.h>
#include <dlfcn.h>
#include "../../include/cryptoki.h"
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

#define CHECK_RV(funName, rv)\
{								\
	if(CKR_OK == rv )					\
	{							\
		printf("......%s [OK]\n",funName);		\
	}							\
	else							\
	{							\
		printf("......%s Failed: [0x%d",funName,rv);	\
	}							\
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
