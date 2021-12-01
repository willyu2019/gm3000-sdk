#include "../../include/skfapi.h"
#include "sm4.h"
#include "mac.h"
#include "sha1.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#pragma comment(lib,"../../lib/mtoken_gm3000.lib")

#define ERROR_THROW(r) {if((r) != SAR_OK) goto END_OF_FUN;}

unsigned char* sm_encrypt(unsigned char key[16], unsigned char * input, int input_len, int * out_len );

void main()
{
	ULONG ulRslt = SAR_OK;
	char  szDevName[256] = {0};
	char  szAppName[256] = {0};
	ULONG ulNameLen = 256;
	BYTE  pbRandom[32] = {0};
	BYTE  pbSha1Val[32] = {0};
	BYTE  pbCipherPIN[32] = {0};
	BYTE  pbHash[16] = {0};
	BYTE  pbMac[16] = {0};
	char  *admpin = "123456";
	char  *newpin = "123456";
	ULONG ulRetryTimes;
	int	outlen;
	DEVHANDLE hdev = NULL, happ = NULL;

	ulRslt = SKF_EnumDev(1, szDevName, &ulNameLen);
	ERROR_THROW(ulRslt)

	char *pszdev = szDevName;
	ERROR_THROW(ulRslt)

	ulNameLen = 256;
	ulRslt = SKF_EnumApplication(hdev, szAppName, &ulNameLen);
	ERROR_THROW(ulRslt)
	
	char *pszApp = szAppName;
	ulRslt = SKF_OpenApplication(hdev, pszApp, &happ);
	ERROR_THROW(ulRslt)

	ulRslt = SKF_GenRandom(hdev, pbRandom, 8);
	ERROR_THROW(ulRslt)

	//管理员PIN做sha1
	strcpy((char *)pbHash, admpin);
	sha1(pbHash, 16, pbSha1Val);

	unsigned char * encrypted = sm_encrypt(pbSha1Val, (BYTE *)newpin, strlen(newpin), &outlen);

	mac_sm4mac_gen(pbSha1Val,pbRandom, (BYTE *)newpin, strlen(newpin), pbMac);

	memcpy(pbCipherPIN, encrypted, outlen);
	memcpy(pbCipherPIN+outlen, pbMac, 4);

	free(encrypted);
	
	ulRslt = SKF_RemoteUnblockPIN(hdev, pbCipherPIN, outlen+4, &ulRetryTimes);
	ERROR_THROW(ulRslt)

	printf("Remote unblock successful");
END_OF_FUN:
	if(happ != NULL)
		SKF_CloseApplication(happ);
	if(hdev != NULL)
		SKF_DisConnectDev(hdev);
}


unsigned char* sm_encrypt(unsigned char key[16], unsigned char * input, int input_len, int * out_len )
{
	int pad_len = 16;
	int new_len = 0;
	unsigned char * out_buff = NULL;

	new_len = input_len + 2; // fill 2 bytes length
	new_len = ((new_len / pad_len) + 1) * pad_len; //get final length

	out_buff = (unsigned char*)malloc(new_len);
	memset(out_buff,0, new_len);

	int pos = 0;
	memcpy(out_buff, &input_len, 2);
	pos += 2;

	memcpy(out_buff + pos, input, input_len);
	pos += input_len;

	out_buff[pos] = 0x80;

	sm4_ecb(key, out_buff, new_len, SM4_ENCRYPT);

	* out_len = new_len;

	return out_buff;
}
