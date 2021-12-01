#include "getinfos.h"

#define  DEBUG_PATH_NG_LINUX	"../../x86/libgm3000_pkcs11.so"

//Initialize the token Cryptoki library:
CGetInfos::CGetInfos():m_hSO(NULL_PTR)
{
	m_path = DEBUG_PATH_NG_LINUX;

	m_pToken = NULL_PTR;
	if(m_hSO != NULL_PTR)
	{
		dlclose(m_hSO);
	}
}

//Free the token Cryptoki library:Finish.
CGetInfos::~CGetInfos()
{
	//if load library failed ,but m_pToke is NULL,so call C_Finalize will be wrong
	if(m_pToken != NULL)
	{
		m_pToken->C_Finalize(NULL_PTR);
		m_pToken = NULL_PTR;
	}
	
	if(m_hSO)
	{
		dlclose(m_hSO);
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
		ShowSlotInfo(pSlotInfo);
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
		ShowTokenInfo(pTokenInfo);
	}
	delete [] pSlotList;
	pSlotList = NULL_PTR;
	return CKR_OK;
}

void CGetInfos::CheckRV(unsigned long rv)
{
	switch(rv)
	{
	case CKR_OK:
		printf("...Function OK!\n");
		break;
	case CKR_SLOT_ID_INVALID:
		printf("...Can not get infos from specifically slot!\n");
		break;
	case CKR_TOKEN_NOT_PRESENT:
		printf("...No usb token in the slot!\n");
		break;
	case CKR_FUNCTION_FAILED:
		printf("...PKCS#11's Function failed!\n");
		break;
	case CKR_DEVICE_ERROR:
		printf("...No slot or no token!\n");	
		break;
	case CKR_HOST_MEMORY:
		printf("Not enough memory!\n");
		break;
	default:
		printf("...Other fault from usb token dll!\n");
	}
}

void CGetInfos::ShowSlotInfo(CK_SLOT_INFO_PTR slotinfo)
{
	printf("Slot information are:\n");

	char DescBuffer[65] = {0};
	memcpy(DescBuffer,slotinfo->slotDescription,64);
	DescBuffer[64] = 0; 
	printf("\tSlotinfo.SlotDescription=%s\n", DescBuffer);
	//
	char manuBuffer[33] = {0};
	memcpy(manuBuffer,slotinfo->manufacturerID,32);
	manuBuffer[32] = 0; 
	
	printf("\tSlotinfo.ManufacturerID=%s\n", manuBuffer);
	printf("\tSlotinfo.flags=%08x\n", slotinfo->flags);
	printf("\tSlotinfo.firmwareVersion.major=%d\n", slotinfo->firmwareVersion.major);
	printf("\tSlotinfo.firmwareVersion.minor=%d\n", slotinfo->firmwareVersion.minor);
	printf("\tSlotinfo.hardwareVersion.major=%d\n", slotinfo->hardwareVersion.major);
	printf("\tSlotinfo.hardwareVersion.minor=%d\n\n", slotinfo->hardwareVersion.minor);

}

void CGetInfos::ShowTokenInfo(CK_TOKEN_INFO_PTR tokenInfo)
{
	printf("\n\tToken information are:\n");
	char label[33] = {0};
	memcpy(label, tokenInfo->label, 32);
	printf("\tToken label=%s\n", label);

	char manuBuffer[33] = {0};
	memcpy(manuBuffer,tokenInfo->manufacturerID,32);
	manuBuffer[32] = 0; 
	printf("\tToken manufacturerID=%s\n", manuBuffer);

	char modelBuffer[17] = {0};
	memcpy(modelBuffer,tokenInfo->model,16);
	manuBuffer[16] = 0; 
	printf("\tToken model=%s\n", modelBuffer);

	char SNBuffer[17] = {0};
	memcpy(SNBuffer,tokenInfo->serialNumber,16);
	manuBuffer[16] = 0; 
	
	printf("\tToken Serial=%s\n", SNBuffer);
	printf("\tToken flags=%d\n", tokenInfo->flags);
	printf("\tToken ulMaxSessionCount=%d\n", tokenInfo->ulMaxSessionCount);
	printf("\tToken ulSessionCount=%d\n", tokenInfo->ulSessionCount);
	printf("\tToken ulMaxRwSessionCount=%d\n",tokenInfo->ulMaxRwSessionCount);
	printf("\tToken ulRwSessionCount=%d\n",tokenInfo->ulRwSessionCount);
	printf("\tToken ulMaxPinLen=%d\n",tokenInfo->ulMaxPinLen);
	printf("\tToken ulMinPinLen=%d\n",tokenInfo->ulMinPinLen); 
	printf("\tToken ulTotalPublicMemory=%d\n", tokenInfo->ulTotalPublicMemory);
	printf("\tToken ulTotalPrivateMemory=%d\n", tokenInfo->ulTotalPrivateMemory);
	printf("\tToken ulFreePublicMemory=%d\n", tokenInfo->ulFreePublicMemory);
	printf("\tToken ulFreePrivateMemory=%d\n", tokenInfo->ulFreePrivateMemory);
	printf("\tToken HardwareVersion.major=%d\n", tokenInfo->hardwareVersion.major);
	printf("\tToken HardwareVersion.minor=%d\n", tokenInfo->hardwareVersion.minor);
	printf("\tToken FirmwareVersion.major=%d\n", tokenInfo->firmwareVersion.major);
	printf("\tToken FirmwareVersion.minor=%d\n", tokenInfo->firmwareVersion.minor);
	printf("\tToken utcTime=%s\n", 0);
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
	printf("\nCryptoki information are:\n");
	printf("\tCryptokiVersion.major=%d\n", pInfo->cryptokiVersion.major);
	printf("\tCryptokiVersion.minor=%d\n", pInfo->cryptokiVersion.minor);
	printf("\tLibraryVersion.major=%d\n", pInfo->libraryVersion.major);
	printf("\tLibraryVersion.minor=%d\n", pInfo->libraryVersion.minor);
	printf("\tFlags=%d\n", pInfo->flags);

	char LibDescBuffer[33] = {0};
	memcpy(LibDescBuffer,pInfo->libraryDescription,32);
	LibDescBuffer[32] = 0; 
	
	printf("\tLibraryDescription=%s\n", LibDescBuffer);

	char manuBuffer[33] = {0};
	memcpy(manuBuffer,pInfo->manufacturerID,32);
	manuBuffer[32] = 0;

	printf("\tManufacturerID=%s\n\n", manuBuffer);
}

/*
if put these operations in Construction,whether the results is success or failed ,
it will continue,and will lead to make error
*/
bool CGetInfos::LoadLib()
{
	m_hSO = dlopen(m_path, RTLD_NOW);
	
	if(m_hSO == NULL_PTR)
	{
		printf("Load Library Error");
		return false;
	}

	typedef CK_RV (* C_GETFUNCTIONLISTPROC)(CK_FUNCTION_LIST_PTR_PTR);
	C_GETFUNCTIONLISTPROC pC_GetFunctionList = (C_GETFUNCTIONLISTPROC)dlsym(m_hSO,"C_GetFunctionList");
	if(pC_GetFunctionList == NULL_PTR)
	{
		printf("Get Function List Failed");
		return false;
	}
	if(CKR_OK!=pC_GetFunctionList(&m_pToken))
	{
		printf("Get Function List Failed");
		return false;
	}
	if(CKR_OK != m_pToken->C_Initialize(NULL_PTR))
	{
		printf("Call C_Initialize Failed");
		return false;
	}
	return true;
}
