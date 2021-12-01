#include "skfapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define	TRUE	1
#define FALSE	0
#define ERROR_THROW(r) {if((r) != SAR_OK) goto END_OF_FUN;}

#ifdef WIN32
	#pragma comment(lib, "mtoken_gm3000.lib")
#endif

void main()
{
	ULONG ulRslt = SAR_OK;
	HANDLE hdev = NULL;
	HANDLE happ = NULL;
	HANDLE hcont = NULL;
	HANDLE hkey = NULL;
	char	szDevName[256] = {0};
	char	szAppName[256] = {0};
	char	szContName[256] = {0};
	char	pUserPin[64] = "12345678";
	ULONG	ulDevNameLen = 256;
	ULONG	ulAppNameLen = 256;
	ULONG	ulContNameLen = 256;
	ULONG	ulRetryCount = 0;
	BYTE	pbData[2048] = {0};
	BYTE	pbEncryptedData[2048] = {0};
	BYTE	*pbEncrypted_ptr = pbEncryptedData;
	ULONG	ulEncryptedDataLen = 2048;
	ECCPUBLICKEYBLOB	ecc_pub = {0};
	PECCCIPHERBLOB		pecc_cipher = NULL;
	ULONG	ulEccpubLen = sizeof(ECCPUBLICKEYBLOB);
	BLOCKCIPHERPARAM bp = {0};
	unsigned char data[ 1024 * 8] = {0};
	ULONG dataLen = 8192;
	char* fileName = "FILE1";


	memset(data, 0xFF, dataLen);

	ulRslt = SKF_EnumDev(TRUE, szDevName, &ulDevNameLen);
	ERROR_THROW(ulRslt)

	char *pdevname = szDevName;
	ulRslt = SKF_ConnectDev(pdevname, &hdev);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_EnumApplication(hdev, szAppName, &ulAppNameLen);
	ERROR_THROW(ulRslt)

	char *pappname = szAppName;
	ulRslt = SKF_OpenApplication(hdev, pappname, &happ);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_VerifyPIN(happ, USER_TYPE, pUserPin, &ulRetryCount);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_CreateFile(happ, fileName, 0x1000, SECURE_USER_ACCOUNT, SECURE_USER_ACCOUNT);
	ERROR_THROW(ulRslt);

	ulRslt = SKF_WriteFile(happ, fileName, 0,  data, dataLen);
	ERROR_THROW(ulRslt);

	printf("read 4k\n");
	ulRslt = SKF_ReadFile(happ, fileName, 0, 0x1000, data, &dataLen);
	if(ulRslt != 0 || dataLen != 0x1000)
	{
		printf("Failed to read 4K");
	}



	printf("Finished.");
	getchar();
END_OF_FUN:
	if(ulRslt != 0)
	{
		printf("Error occured");
	}

	if(happ != NULL)
		SKF_CloseApplication(happ);
	if(hdev != NULL)
		SKF_DisConnectDev(hdev);	
	if(pecc_cipher != NULL)
	{
		free(pecc_cipher);
		pecc_cipher = NULL;
	}
	return ;

}