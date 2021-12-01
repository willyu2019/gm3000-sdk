#include "../include/skfapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define	TRUE	1
#define FALSE	0
#define ERROR_THROW(r) {if((r) != SAR_OK) goto END_OF_FUN;}

int main()
{
	ULONG ulRslt = SAR_OK;
	HANDLE hdev = NULL;
	char   szDevName[256] = {0};
	ULONG	ulDevNameLen = 256;
	char *pdevname = szDevName;
	DEVINFO info = {0};

	for(int i=0; i<10; i++)
	{
	ulDevNameLen = 256;
		pdevname = szDevName;
		ulRslt = SKF_EnumDev(TRUE, szDevName, &ulDevNameLen);
		if(ulRslt != SAR_OK)
		{
			printf("Enum device error.%08x\n", ulRslt);
			goto END_OF_FUN;
		}

		if(strlen(pdevname) == 0)
		{
			printf("\tNot found device. \n");
			goto END_OF_FUN;
		}

		while(strlen(pdevname) > 0)
		{
			sleep(2);
			ulRslt = SKF_ConnectDev(pdevname, &hdev);
			if(ulRslt != SAR_OK)
			{
				printf("Connect device error.%08x\n", ulRslt);
				goto END_OF_FUN;
			}
			memset(&info, 0, sizeof(info));
			ulRslt = SKF_GetDevInfo(hdev, &info);
			if(ulRslt != SAR_OK)
			{
				printf("Get device infomation error.%08x\n", ulRslt);
				goto END_OF_FUN;
			}
			
			printf("\n\tManufacturer: %s \n", info.Manufacturer);
			printf("\nAlgSymCap :%d \n" ,info.AlgSymCap);
			printf("\nAlgAsymCap :%d,\n", info.AlgAsymCap);
			printf("\nAlgHashCap :%d,\n", info.AlgHashCap);
			printf("\nevAuthAlgId :%d,\n", info.DevAuthAlgId);
			printf("\tIssuer : %s \n", info.Issuer);
			printf("\tLabel : %s \n", info.Label);
			printf("\tSerialNumber : %s \n", info.SerialNumber);
			printf("\tTotal space:%ld \n\n", info.TotalSpace);
		
			//SKF_DisConnectDev(hdev);

			pdevname += strlen(pdevname) + 1;
		}
	}
	

END_OF_FUN:

	return 1;
}
