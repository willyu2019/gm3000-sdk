/*
[]=========================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	BaseAll.cpp

DESC:
	base class:Find the token and open/close session.
[]=========================================================================[]
*/

#include "BaseAll.h"
CBaseAll::CBaseAll()
{
}
CBaseAll::CBaseAll(char *dll_filepath) :
	m_hDll(NULL),
	pSlotList(NULL_PTR)
{
	memset(m_dll_filepath, 0, MAX_FILEPATH);
	memcpy(m_dll_filepath, dll_filepath, strlen(dll_filepath));
	memset(m_ucPin, 0, 32);
}

CBaseAll::~CBaseAll()
{
	if(pSlotList != NULL_PTR)
	{
		free(pSlotList);
		pSlotList = NULL_PTR;
	}
	if(m_hDll != NULL_PTR)
	{
		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
}
CK_RV CBaseAll::BaseAllStart()
{
	cout <<
	"\n[ENTER: CBaseAll::BaseAllStart:LoadLibrary PKCS#11's dll and initialize it!]"
		 <<
	endl;
	CK_RV rv = CKR_OK;
	m_hDll = LoadLibrary(m_dll_filepath);
	if(m_hDll == NULL)
	{
		cout <<
		"......LoadLibrary [" <<
		m_dll_filepath <<
		"] failed!" <<
		endl;
		return CKR_HOST_MEMORY;
	}

	//Get the list of the PKCS#11 function entries.
	C_GETFUNCTIONLISTPROC pC_GetFunctionList = (C_GETFUNCTIONLISTPROC)
											   GetProcAddress(m_hDll,
															  "C_GetFunctionList");
	if(pC_GetFunctionList == NULL)
	{
		cout << "...Err: (pC_GetFunctionList == NULL)!" << endl;
		return CKR_HOST_MEMORY;
	}

	//Get PKCS#11 function list.
	if(CKR_OK != (rv = pC_GetFunctionList(&m_gToken)))
	{
		cout << "...Get PKCS#11 function list failed!" << endl;
		return rv;
	}
	cout << "Initialize the UsbToken PKCS#11 library..." << endl;
	rv = m_gToken->C_Initialize(NULL_PTR);
	if(CKR_OK != rv)
	{
		cout << "...C_Initialize failed! Return Error Code=" << rv << endl;
		return rv;
	}

	// Get the number of slots with a token attached.
	cout << "Get the slots information..." << endl;
	unsigned long ulCount;
	rv = m_gToken->C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if(CKR_OK != rv)
	{
		cout << "...C_GetSlotList failed! Return Error Code=" << rv << endl;
		return rv;
	}
	if(ulCount <= 0)
	{
		cout << "...No slot with token attached." << endl;
		return CKR_TOKEN_NOT_PRESENT;
	}

	// Allocate memory buffer for the slot list.
	cout << "Allocate memory for slots..." << endl;
	pSlotList = (CK_SLOT_ID_PTR) malloc(ulCount * sizeof(CK_SLOT_ID));
	if(!pSlotList)
	{
		cout << "...No slots!" << endl;
		return CKR_HOST_MEMORY;
	}
	else
	{
		cout << "Get " << ulCount << " slot attached to USB port!" << endl;
	}
	// Okay, we have the buffer allocated. Let's get the slot list.
	cout << "Get the slots information." << endl;
	rv = m_gToken->C_GetSlotList(TRUE, pSlotList, &ulCount);
	if(CKR_OK != rv)
	{
		cout << "........C_GetSLotList failed! Return Error Code=" << rv
			 << endl;
		CK_RV rv1 = m_gToken->C_Finalize(0);
		if(CKR_OK != rv1)
		{
			cout << "..........C_Finalize failed!" << endl;
			return rv1;
		}
		m_gToken = NULL_PTR;
		Sleep(500);	
		if(m_hDll)
		{
			FreeLibrary(m_hDll);
			m_hDll = NULL;
		}
		return rv;
	}
	// Open a session to communicate with the token.
	cout << "Open a session to communicate with the UsbToken..." << endl;
	rv = m_gToken->C_OpenSession(pSlotList[0],
								 CKF_RW_SESSION | CKF_SERIAL_SESSION,
								 NULL_PTR, NULL_PTR, &hSession);
	if(CKR_OK != rv)
	{
		cout << "....C_OpenSession failed! Return Error Code=" << rv << endl;
		BaseAllEnd();
		return rv;
	}

	CK_TOKEN_INFO tokenInfo = {0};
	rv = m_gToken->C_GetTokenInfo(pSlotList[0], &tokenInfo);
	if (CKR_OK != rv)
	{
		cout << "....Can not get token info! Return Error Code=" << rv << endl;
		BaseAllEnd();
		return rv;
	}	
	cout << "Input user-pin to login first:" << endl;
	cin >> m_ucPin;

	rv = m_gToken->C_Login(hSession, CKU_USER, m_ucPin,
							   strlen((char *) m_ucPin)); 
	
	if(rv != CKR_OK)
	{
		cout << "...C_Login failed!" << rv << endl;
		BaseAllEnd();
		return rv;
	} 
	cout << "...C_Login OK!" << endl;
	cout << "\n[EXIT:CBaseAll::BaseAllStart()]" << endl << endl;
	return rv;
}

CK_RV CBaseAll::BaseAllEnd()
{
	cout <<
	"\n[ENTER:CBaseAll::BaseAllEnd:Close session and free the PKCS#11's dll]"
		 <<
	endl;
	CK_RV rv = CKR_OK;
	cout << "Release the session handle..." << endl;
	if(m_gToken)
	{
		rv = m_gToken->C_CloseSession(hSession);
		if(CKR_OK != rv)
		{
			cout << "....C_CloseSession failed!" << endl;
			return rv;
		}
		rv = m_gToken->C_Finalize(0);
		if(CKR_OK != rv)
		{
			cout << "...C_Finalize failed!" << endl;
			return rv;
		}
		m_gToken = NULL_PTR;
	}
	Sleep(500);	
	if(m_hDll != NULL_PTR)
	{
		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
	cout << "\n[EXIT:CBaseAll::BaseAllEnd()]" << endl << endl;
	return CKR_OK;
}

void CBaseAll::CheckRV(char *pFuncName, CK_RV rv)
{
	if(rv == CKR_OK)
	{
		cout << "......";
		cout << pFuncName;
		cout << " OK! \n";
		return ;
	}
	else
	{
		cout << "......";
		cout << pFuncName;
		cout << " failed: [0x" << rv << "] " << endl;
		return ;
	}
}
