#include "../include/skfapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define	TRUE	1
#define FALSE	0
#define ERROR_THROW(r) {if((r) != SAR_OK) goto END_OF_FUN;}

void *CancelThread(void *)
{
	ULONG ulRslt = SAR_OK;
	HANDLE hdev = NULL;
	char   szDevName[256] = {0};
	ULONG	ulDevNameLen = 256;
	char *pdevname = szDevName;
	DEVINFO info = {0};
	ULONG ulEvent;
	while(1)
	{

		memset(szDevName, 0, sizeof(szDevName));
		ulDevNameLen = 256;
		printf("SKF_WaitForDevEvent\n");
		ulRslt = SKF_WaitForDevEvent(szDevName, &ulDevNameLen, &ulEvent);
		if(ulEvent == 1)
		{
			ulRslt = SKF_ConnectDev(pdevname, &hdev);
			if(ulRslt != SAR_OK)
			{
				printf("Connect device error.%08x\n", ulRslt);
				goto END_OF_FUN;
				//continue;
			}
			memset(&info, 0, sizeof(info));
			ulRslt = SKF_GetDevInfo(hdev, &info);
			if(ulRslt != SAR_OK)
			{
				printf("Get device infomation error.%08x\n", ulRslt);
				goto END_OF_FUN;
				//continue;
			}

			printf("\n\tManufacturer: %s \n", info.Manufacturer);
			printf("\tIssuer : %s \n", info.Issuer);
			printf("\tLabel : %s \n", info.Label);
			printf("\tSerialNumber : %s \n", szDevName);
			printf("\tTotal space:%d \n\n", info.TotalSpace);

			//SKF_DisConnectDev(hdev);
		}
		
		if(ulEvent == 2)
		{
			printf("\n\t Device moved.\n");
			printf("\t %s \n\n", szDevName);
		}
		
		
		
	}
END_OF_FUN:
	return NULL;
}

int main()
{
	
	pthread_t hThread;

	int nRet = pthread_create(&hThread, NULL, (void* (*)(void*))CancelThread, NULL);
	sleep(200000);


	return 1;
}
