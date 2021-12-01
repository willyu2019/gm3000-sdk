#include "../include/skfapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	TRUE	1
#define FALSE	0
#define ERROR_THROW(r) {if((r) != SAR_OK) goto END_OF_FUN;}


int main()
{
	ULONG ulRslt = SAR_OK;
	HANDLE hdev = NULL;
	HANDLE happ = NULL;
	HANDLE hkey = NULL;
	HANDLE hcont = NULL;
	char   szDevName[256] = {0};
	ULONG	ulDevNameLen = 256;
	char	szAppName[256] = {0};
	ULONG	ulAppNameLen = 256;
	char	szContName[256] = {0};
	ULONG	ulContName = 256;
	char	*pUserPin = "123456";
	ULONG	ulRetryCount = 0;

	BYTE	pbEncrypt[256] = {0};
	ULONG   ulEncryptLen = 256;

	char    *pData = "1234567812345678";
	BYTE    pbRandom[32] = {0};
	BLOCKCIPHERPARAM bp = {0};

	int  nDatalen = strlen(pData);
	char *pContName = szContName;
	char *pdevname = szDevName;
	char *pappname = szAppName;

	ulRslt = SKF_EnumDev(TRUE, szDevName, &ulDevNameLen);
	ERROR_THROW(ulRslt)

	
	ulRslt = SKF_ConnectDev(pdevname, &hdev);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_EnumApplication(hdev, szAppName, &ulAppNameLen);
	ERROR_THROW(ulRslt)

	
	ulRslt = SKF_OpenApplication(hdev, pappname, &happ);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_VerifyPIN(happ, USER_TYPE, pUserPin, &ulRetryCount);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_EnumContainer(happ, szContName, &ulContName);
	ERROR_THROW(ulRslt)

	
	ulRslt = SKF_OpenContainer(happ, pContName, &hcont);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_GenRandom(hdev, pbRandom, 16);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_SetSymmKey(hdev, pbRandom, SGD_SM1_ECB, &hkey);
	ERROR_THROW(ulRslt)

	//bp.PaddingType = 1;
	ulRslt = SKF_EncryptInit(hkey, bp);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_Encrypt(hkey, (BYTE *)pData, nDatalen, pbEncrypt, &ulEncryptLen);
	ERROR_THROW(ulRslt)

	printf("encrypt data ok!\n");
END_OF_FUN:
	if(hkey)
		SKF_CloseHandle(hkey);
	if(hcont)
		SKF_CloseContainer(hcont);
	if(happ)
		SKF_CloseApplication(happ);
	if(hdev)
		SKF_DisConnectDev(hdev);
	return 1;
}
