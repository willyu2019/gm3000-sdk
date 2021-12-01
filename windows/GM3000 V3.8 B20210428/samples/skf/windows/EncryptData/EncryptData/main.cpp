#include "../../include/skfapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Win32Dlg.h"

#define	TRUE	1
#define FALSE	0
#define ERROR_THROW(r) {if((r) != SAR_OK) goto END_OF_FUN;}
#pragma comment(lib, "../../lib/mtoken_gm3000.lib")



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
	char	pUserPin[64] = {'0', '1', '2', '3', '4', '5', '6', '\0'};
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

	{
		CWin32Dlg dlg;

		if(dlg.RegisterClass())
		{
			return ;
		}

		if(dlg.CreateInstance())
		{
			return ;
		}

		while(dlg.ProcessNextMessage())
		{

		}
		memset(pUserPin, 0 , sizeof(pUserPin));
		strncpy(pUserPin, dlg.GetPin(), 64);
	}
	

	ulRslt = SKF_VerifyPIN(happ, USER_TYPE, pUserPin, &ulRetryCount);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_EnumContainer(happ, szContName, &ulContNameLen);
	ERROR_THROW(ulRslt)

	char *pcontname = szContName;
	ulRslt = SKF_OpenContainer(happ, pcontname, &hcont);
	ERROR_THROW(ulRslt)

	//公钥从证书解析
	ulRslt = SKF_ExportPublicKey(hcont, FALSE, (BYTE *)&ecc_pub, &ulEccpubLen);
	ERROR_THROW(ulRslt)

	//此处需要保存Cipher，解密时需要导入
	pecc_cipher = (PECCCIPHERBLOB)malloc(sizeof(ECCCIPHERBLOB) + 64);
	memset(pecc_cipher, 0, sizeof(ECCCIPHERBLOB) + 64);
	ulRslt = SKF_ECCExportSessionKey(hcont, SGD_SM1_ECB, &ecc_pub, pecc_cipher, &hkey);
	ERROR_THROW(ulRslt)

	//根据加密模式设定IV
	memcpy(bp.IV, "12345678", 8);
	bp.IVLen = 8;
	bp.PaddingType = 1;

	ulRslt = SKF_LockDev(hdev, 0xFFFFFFFF);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_EncryptInit(hkey, bp);
	if(ulRslt != SAR_OK)
	{
		SKF_UnlockDev(hdev);
		goto END_OF_FUN;
	}

	//分组计算时，SKF_EncryptUpdate每包数据长度为1024字节时，计算速度最快
	ulRslt = SKF_EncryptUpdate(hkey, pbData, 1024, pbEncrypted_ptr, &ulEncryptedDataLen);
	if(ulRslt != SAR_OK)
	{
		SKF_UnlockDev(hdev);
		goto END_OF_FUN;
	}

	pbEncrypted_ptr += ulEncryptedDataLen;
	ulEncryptedDataLen = 1024;
	ulRslt = SKF_EncryptFinal(hkey, pbEncrypted_ptr, &ulEncryptedDataLen);
	if(ulRslt != SAR_OK)
	{
		SKF_UnlockDev(hdev);
		goto END_OF_FUN;
	}

	ulRslt = SKF_UnlockDev(hdev);


END_OF_FUN:
	if(hkey != NULL)
		SKF_CloseHandle(hkey);
	if(hcont != NULL)
		SKF_CloseContainer(hcont);
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