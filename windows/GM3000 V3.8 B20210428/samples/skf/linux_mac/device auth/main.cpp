#include "../include/skfapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "../../lib/mtoken_gm3000.lib")

#define ERROR_THROW(r) {if((r) != SAR_OK) goto END_OF_FUN;}
int main()
{
	ULONG		ulRslt;
	DEVHANDLE	hdev = NULL;
	BYTE		pbRandom[32] = {0};
	char		szDevName[256] = {0};
	ULONG		ulNameLen = 256;
	char		new_auth_key[33] = {0};
	BYTE		mac[4] = {0};
	BYTE		data[128] = {0};
	HANDLE		hkey = NULL;
	BLOCKCIPHERPARAM bp = {0};
	char *dev_auth_key = "1234567812345678";
	BYTE szEncryptedData[256] = {0};
	ULONG ulEncryptedDataLen = 256;
	char *pszdev = szDevName;
	
	ulRslt = SKF_EnumDev(1, szDevName, &ulNameLen);
	ERROR_THROW(ulRslt)
	
	ulRslt = SKF_ConnectDev(pszdev, &hdev);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_GenRandom(hdev, pbRandom, 8);
	ERROR_THROW(ulRslt)

	
	ulRslt = SKF_SetSymmKey(hdev, (unsigned char*)dev_auth_key, SGD_SM4_ECB, &hkey);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_EncryptInit(hkey, bp);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_Encrypt(hkey, pbRandom, 16, szEncryptedData, &ulEncryptedDataLen);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_DevAuth(hdev, szEncryptedData, ulEncryptedDataLen);
	ERROR_THROW(ulRslt)

	printf("Authentication successful.\n");

	//Change device authentication

	strcpy(new_auth_key, "8765432112345678");

	ulRslt = SKF_ChangeDevAuthKey(hdev, (BYTE *)new_auth_key, 16);
	ERROR_THROW(ulRslt)

	printf("Change authentication successful.\n");
END_OF_FUN:
	if(hkey != NULL)
	{
		SKF_CloseHandle(hkey);
	}
	if(hdev != NULL)
	{
		SKF_DisConnectDev(hdev);
	}
	return ulRslt;
}
