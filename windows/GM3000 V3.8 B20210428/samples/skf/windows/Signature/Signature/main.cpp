#include "skfapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	TRUE	1
#define FALSE	0
#define ERROR_THROW(r) {if((r) != SAR_OK) goto END_OF_FUN;}
#ifdef WIN32
#pragma comment(lib, "../../lib/mtoken_gm3000.lib")
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
	char	*pUserPin = "123456";
	ULONG	ulDevNameLen = 256;
	ULONG	ulAppNameLen = 256;
	ULONG	ulContNameLen = 256;
	ULONG	ulRetryCount = 0;
	BYTE	pHashData[256] = {0};
	ULONG	ulHashDataLen = 256;
	ECCPUBLICKEYBLOB	ecc_pub = {0};
	ECCSIGNATUREBLOB	ecc_sign = {0};
	ULONG	ulEccpubLen = sizeof(ECCPUBLICKEYBLOB);
	BLOCKCIPHERPARAM bp = {0};
	char *pappname = szAppName;
	char *pubid = "1234567812345678";
	ULONG idlen = 16;
	char *psrcdata = "1234567812345678";
	char *pdevname = szDevName;
	char *pcontname = szContName;
	
	ulRslt = SKF_EnumDev(TRUE, szDevName, &ulDevNameLen);
	ERROR_THROW(ulRslt);

	
	ulRslt = SKF_ConnectDev(pdevname, &hdev);
	ERROR_THROW(ulRslt);

	ulRslt = SKF_EnumApplication(hdev, szAppName, &ulAppNameLen);
	ERROR_THROW(ulRslt);


	ulRslt = SKF_OpenApplication(hdev, pappname, &happ);
	ERROR_THROW(ulRslt);

	ulRslt = SKF_VerifyPIN(happ, USER_TYPE, pUserPin, &ulRetryCount);
	ERROR_THROW(ulRslt);

	ulRslt = SKF_EnumContainer(happ, szContName, &ulContNameLen);
	ERROR_THROW(ulRslt);


	ulRslt = SKF_OpenContainer(happ, pcontname, &hcont);
	ERROR_THROW(ulRslt);

// 	//生成密钥对
 	ulRslt = SKF_GenECCKeyPair(hcont, SGD_SM2_1, &ecc_pub);
 	ERROR_THROW(ulRslt);

	ulRslt = SKF_ExportPublicKey(hcont, TRUE, &ecc_pub, &ulEccpubLen);
	ERROR_THROW(ulRslt);

	//做摘要
	ulRslt = SKF_DigestInit(hdev, SGD_SM3, &ecc_pub, (BYTE *)pubid, 16, &hkey);
	ERROR_THROW(ulRslt);

	ulRslt = SKF_Digest(hkey, (BYTE *)psrcdata, strlen(psrcdata), pHashData, &ulHashDataLen);
	ERROR_THROW(ulRslt);

	//签名
	ulRslt = SKF_ECCSignData(hcont, pHashData, ulHashDataLen, &ecc_sign);
	ERROR_THROW(ulRslt);

	printf("Signature successful");

	//验签
	ulRslt = SKF_ECCVerify(hdev, &ecc_pub, pHashData, ulHashDataLen, &ecc_sign);
	ERROR_THROW(ulRslt);

	printf("Verify signature successful");
		



END_OF_FUN:
	if(hkey != NULL)
		SKF_CloseHandle(hkey);
	if(hcont != NULL)
		SKF_CloseContainer(hcont);
	if(happ != NULL)
		SKF_CloseApplication(happ);
	if(hdev != NULL)
		SKF_DisConnectDev(hdev);	

	return ;

}