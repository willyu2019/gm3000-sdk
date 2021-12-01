/*
[]=========================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	BaseAll.h

DESC:
[]=========================================================================[]
*/
#ifndef _BASSALL_H_
#define  _BASSALL_H_
#include <windows.h>
#include <iostream>
using namespace std;
#include "include/cryptoki.h"


#define  MAX_FILEPATH 256

class CBaseAll
{
public:
	CBaseAll(char *dll_filepath);
	CBaseAll();
	virtual ~CBaseAll();

	CK_RV BaseAllEnd(void);
	CK_RV BaseAllStart(void);

protected:
	void CheckRV(char *pFuncName, CK_RV rv);
	CK_SESSION_HANDLE hSession;
	CK_FUNCTION_LIST_PTR m_gToken;
	unsigned char m_ucPin[32];

private:
	CK_SLOT_ID_PTR pSlotList;
	HMODULE m_hDll;
	char m_dll_filepath[MAX_FILEPATH];
	typedef CK_RV (* C_GETFUNCTIONLISTPROC)(CK_FUNCTION_LIST_PTR_PTR);
};

#endif 
