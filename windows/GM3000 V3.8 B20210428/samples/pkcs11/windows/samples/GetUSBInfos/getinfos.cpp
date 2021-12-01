/*
[]=========================================================================[]

FILE:
	getinfos.cpp

DESC:
	
[]=========================================================================[]
*/

//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "getinfos.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define PKCS_LIB_NAME			"gm3000_pkcs11.dll"

//Initialize the token Cryptoki library:
CGetInfos::CGetInfos():m_hDll(NULL_PTR)
{
	m_path = PKCS_LIB_NAME;
}

//Free the token Cryptoki library:Finish.
CGetInfos::~CGetInfos()
{
	//if load library failed ,but m_pToke is NULL, so call C_Finalize will be wrong
	if(m_pToken != NULL)
	{
		m_pToken->C_Finalize(NULL_PTR);
		m_pToken = NULL_PTR;
	}
	
	if(m_hDll)
	{
		FreeLibrary(m_hDll);
	}
}

unsigned long CGetInfos::GetSlotInfos(CK_SLOT_INFO_PTR pSlotInfo)
{
	CK_RV rv = CKR_OK;
	CK_ULONG ulCount = 0;
	CK_SLOT_ID_PTR pSlotList = NULL_PTR;
	rv = m_pToken->C_GetSlotList(FALSE, NULL_PTR, &ulCount);
	if((rv != CKR_OK) || (ulCount <= 0))
		return CKR_DEVICE_ERROR;
	
	pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID [ulCount];
	if (pSlotList == NULL_PTR)
	{
		return CKR_HOST_MEMORY;
	}

	rv = m_pToken->C_GetSlotList(FALSE, pSlotList, &ulCount);
	if((rv != CKR_OK) || (ulCount <= 0))
	{
		delete [] pSlotList;
		pSlotList = NULL_PTR;
		return CKR_SLOT_ID_INVALID;
	}
	/*Get slot information for the first slot*/
	for (unsigned int i = 0; i < ulCount; ++i)
	{
		rv = m_pToken->C_GetSlotInfo(pSlotList[i], pSlotInfo);
		if(rv != CKR_OK)
		{
			delete [] pSlotList;
			pSlotList = NULL_PTR;
			return CKR_FUNCTION_FAILED;
		}
		//ShowSlotInfo(pSlotInfo);
	}

	delete [] pSlotList;
	pSlotList = NULL_PTR;
	return CKR_OK;
}

unsigned long CGetInfos::GetTokenInfos(CK_TOKEN_INFO_PTR pTokenInfo)
{
	CK_RV rv = CKR_OK;
	CK_ULONG ulCount = 0;
	CK_SLOT_ID_PTR pSlotList = NULL_PTR;

	rv = m_pToken->C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if((rv != CKR_OK) || (ulCount <= 0))
		return CKR_DEVICE_ERROR;
	pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID [ulCount];
	rv = m_pToken->C_GetSlotList(TRUE, pSlotList, &ulCount);
	if((rv != CKR_OK) || (ulCount <= 0))
	{
		delete [] pSlotList;
		pSlotList = NULL_PTR;
		return CKR_TOKEN_NOT_PRESENT;
	}
	/*Get slot information for the first token*/
	for (unsigned int i = 0; i < ulCount; ++i)
	{
		rv = m_pToken->C_GetTokenInfo(pSlotList[i], pTokenInfo);
		if(rv != CKR_OK)
		{
			delete [] pSlotList;
			pSlotList = NULL_PTR;
			return CKR_FUNCTION_FAILED;
		}
		//ShowTokenInfo(pTokenInfo);
	}
	delete [] pSlotList;
	pSlotList = NULL_PTR;
	return CKR_OK;
}

bool CGetInfos::CheckRV(const char* szInfo, unsigned long rv)
{
	printf(szInfo);
	if(CKR_OK == rv)
	{
		//printf(" ... OK.\n");
		return true;
	}

	printf(" ... FAILED. ");

	switch(rv)
	{
	case CKR_SLOT_ID_INVALID:
		printf("[CKR_SLOT_ID_INVALID]");
		break;
	case CKR_TOKEN_NOT_PRESENT:
		printf("[CKR_TOKEN_NOT_PRESENT]");
		break;
	case CKR_FUNCTION_FAILED:
		printf("[CKR_FUNCTION_FAILED]");
		break;
	case CKR_DEVICE_ERROR:
		printf("[CKR_DEVICE_ERROR]");
		break;
	case CKR_HOST_MEMORY:
		printf("[CKR_HOST_MEMORY]");
		break;
	default:
		printf("[Unknown ERROR: 0x%08X]", rv);
	}

	printf("\n");
	return false;
}

void CGetInfos::ShowSlotInfo(CK_SLOT_INFO_PTR slotinfo)
{
	printf("\nSlot information:\n");

	char DescBuffer[65] = {0};
	memcpy(DescBuffer,slotinfo->slotDescription,64);
	DescBuffer[64] = 0; 
	int i = 0;
	for(i = 63; i > 0; --i)
		if(' ' == DescBuffer[i])
			DescBuffer[i] = '\0';
		else
			break;

	printf("\tSlotDescription       = %s\n", DescBuffer);

	char manuBuffer[33] = {0};
	memcpy(manuBuffer,slotinfo->manufacturerID,32);
	manuBuffer[32] = 0; 
	for(i = 31; i > 0; --i)
		if(' ' == manuBuffer[i])
			manuBuffer[i] = '\0';
		else
			break;
	
	printf("\tManufacturerID        = %s\n", manuBuffer);
	printf("\tFlags                 = %08X\n", slotinfo->flags);
	printf("\tFirmwareVersion.major = %d\n", slotinfo->firmwareVersion.major);
	printf("\tFirmwareVersion.minor = %d\n", slotinfo->firmwareVersion.minor);
	printf("\tHardwareVersion.major = %d\n", slotinfo->hardwareVersion.major);
	printf("\tHardwareVersion.minor = %d\n\n", slotinfo->hardwareVersion.minor);

}

void CGetInfos::ShowTokenInfo(CK_TOKEN_INFO_PTR tokenInfo)
{
	printf("\nToken information:\n");
	char label[33] = {0};
	memcpy(label, tokenInfo->label, 32);
	printf("\tLabel                 = %s\n", label);

	char manuBuffer[33] = {0};
	memcpy(manuBuffer,tokenInfo->manufacturerID,32);
	manuBuffer[32] = 0; 
	printf("\tManufacturerID        = %s\n", manuBuffer);

	char modelBuffer[17] = {0};
	memcpy(modelBuffer,tokenInfo->model,16);
	manuBuffer[16] = 0; 
	printf("\tModel                 = %s\n", modelBuffer);

	char SNBuffer[17] = {0};
	memcpy(SNBuffer,tokenInfo->serialNumber,16);
	manuBuffer[16] = 0; 
	printf("\tSerialNumber          = %s\n", SNBuffer);

	printf("\tFlags                 = 0x%08X\n", tokenInfo->flags);
	printf("\tulMaxSessionCount     = %d\n", tokenInfo->ulMaxSessionCount);
	printf("\tulSessionCount        = %d\n", tokenInfo->ulSessionCount);
	printf("\tulMaxRwSessionCount   = %d\n",tokenInfo->ulMaxRwSessionCount);
	printf("\tulRwSessionCount      = %d\n",tokenInfo->ulRwSessionCount);
	printf("\tulMaxPinLen           = %d\n",tokenInfo->ulMaxPinLen);
	printf("\tulMinPinLen           = %d\n",tokenInfo->ulMinPinLen); 
	printf("\tulTotalPublicMemory   = %d\n", tokenInfo->ulTotalPublicMemory);
	printf("\tulTotalPrivateMemory  = %d\n", tokenInfo->ulTotalPrivateMemory);
	printf("\tulFreePublicMemory    = %d\n", tokenInfo->ulFreePublicMemory);
	printf("\tulFreePrivateMemory   = %d\n", tokenInfo->ulFreePrivateMemory);
	printf("\tHardwareVersion.major = %d\n", tokenInfo->hardwareVersion.major);
	printf("\tHardwareVersion.minor = %d\n", tokenInfo->hardwareVersion.minor);
	printf("\tFirmwareVersion.major = %d\n", tokenInfo->firmwareVersion.major);
	printf("\tFirmwareVersion.minor = %d\n", tokenInfo->firmwareVersion.minor);
	printf("\tToken utcTime         = %d\n", 0);

}

unsigned long CGetInfos::GetCryptokiInfos(CK_INFO_PTR pInfo)
{

	memset(pInfo, 0,  sizeof(pInfo));
	if(m_pToken->C_GetInfo(pInfo) != CKR_OK)
	{
		return CKR_FUNCTION_FAILED;
	}
	return CKR_OK;
}

void CGetInfos::ShowCryptokiInfos(CK_INFO_PTR pInfo)
{
	printf("\nCryptoki informations:\n");
	printf("\tCryptokiVersion.major = %d\n", pInfo->cryptokiVersion.major);
	printf("\tCryptokiVersion.minor = %d\n", pInfo->cryptokiVersion.minor);
	printf("\tLibraryVersion.major  = %d\n", pInfo->libraryVersion.major);
	printf("\tLibraryVersion.minor  = %d\n", pInfo->libraryVersion.minor);
	printf("\tFlags                 = 0x%08X\n", pInfo->flags);

	char LibDescBuffer[33] = {0};
	memcpy(LibDescBuffer,pInfo->libraryDescription,32);
	LibDescBuffer[32] = 0; 
	
	printf("\tLibraryDescription    = %s\n", LibDescBuffer);

	char manuBuffer[33] = {0};
	memcpy(manuBuffer,pInfo->manufacturerID,32);
	manuBuffer[32] = 0;

	printf("\tManufacturerID        = %s\n\n", manuBuffer);
}

bool CGetInfos::LoadLib()
{
#if defined(WIN32)
	m_hDll = LoadLibrary(m_path);
#else
	m_hDll = dlopen(m_path, RTLD_NOW);
#endif
	
	if(m_hDll == NULL_PTR)
	{
		printf("Load Library Error!");
		return false;
	}

	typedef CK_RV (* C_GETFUNCTIONLISTPROC)(CK_FUNCTION_LIST_PTR_PTR);
	C_GETFUNCTIONLISTPROC pC_GetFunctionList = (C_GETFUNCTIONLISTPROC)GetProcAddress(m_hDll,"C_GetFunctionList");
	if(pC_GetFunctionList == NULL_PTR)
	{
		printf("Get function list failed.\n");
		return false;
	}
	if(CKR_OK!=pC_GetFunctionList(&m_pToken))
	{
		printf("Get function list failed.\n");
		return false;
	}
	if(CKR_OK != m_pToken->C_Initialize(NULL_PTR))
	{
		printf("Call C_Initialize failed.\n");
		return false;
	}
	return true;
}
