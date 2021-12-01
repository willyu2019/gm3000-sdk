#include "../include/skfapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	ULONG		ulRslt;
	DEVHANDLE	hdev = NULL;
	BYTE		pbRandom[32] = { 0 };
	char		szDevName[256] = { 0 };
	ULONG		ulNameLen = 256;
	HANDLE		hkey = NULL;
	BLOCKCIPHERPARAM bp = { 0 };
	char *pszdev = szDevName;

	int i = 0;
	while (1)
	{
		memset(szDevName, 0, 256);
		ulNameLen = 256;
		ulRslt = SKF_EnumDev(1, szDevName, &ulNameLen);
		printf("%d %s\n", i, szDevName);
		if (ulRslt != SAR_OK)
			continue;

		ulRslt = SKF_ConnectDev(pszdev, &hdev);
		if (ulRslt != SAR_OK)
			continue;

		ulRslt = SKF_GenRandom(hdev, pbRandom, 8);
		if (ulRslt != SAR_OK)
			continue;

		i++;
	}
	
END_OF_FUN:
	if (hkey != NULL)
	{
		SKF_CloseHandle(hkey);
	}
	if (hdev != NULL)
	{
		SKF_DisConnectDev(hdev);
	}
	return ulRslt;
}