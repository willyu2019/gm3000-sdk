#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/cryptoki.h"

bool bInited = false; 
CK_SLOT_ID_PTR pSlotList = NULL_PTR;

void CheckResult(CK_RV rvRet)
{
	if(CKR_OK == (rvRet))
	{
		printf("....[OK]\n");
	}
	else
	{
		printf("....[FAILED]\n\t\t\tErrCode = 0x%08X.", rvRet);
		if (pSlotList)
			free(pSlotList);
		if(bInited)
			C_Finalize(NULL_PTR);

		exit(-1);
	}
}

void ExitProg(char* szInfo)
{
	printf("\n\nERROR: %s\n", szInfo);
	if (pSlotList)
		free(pSlotList);
	if(bInited)
		C_Finalize(NULL_PTR);

	exit(-1);
}

int main(int argc, char* argv[])
{
	CK_RV rv;
	CK_ULONG ulCount = 0;		

	printf("\n");
	printf("[]==================================================[]\n");
	printf(" |                    PKCS#11 Demo                  |\n");				
	printf("[]==================================================[]\n");

	//  Initialize the PKCS#11 library.
	printf("\nInitialize the PKCS#11 library.");
	rv = C_Initialize(NULL_PTR);
	CheckResult(rv);

	// Get the number of slots with a token attached.
	printf("\nGet the number of slots with token attached.");
	rv = C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	CheckResult(rv);
	if(ulCount <= 0)
		ExitProg("No slot with token attached.");
	printf("\nThere are %d slots with the token attached.\n", ulCount);

	// Allocate memory buffer for the slot list.
	printf("\nAllocate memory for the slot list.");
	pSlotList = (CK_SLOT_ID_PTR)malloc(ulCount * sizeof(CK_SLOT_ID));
	if (! pSlotList) 
		CheckResult(CKR_HOST_MEMORY);
	else
		CheckResult(CKR_OK);

	// Okay, we have the buffer allocated. Let's get the slot list.
	printf("\nGet the slots list.");
	rv = C_GetSlotList(TRUE, pSlotList, &ulCount);
	CheckResult(rv);

	CK_TOKEN_INFO  m_Info;
	for(CK_ULONG i = 0; i < ulCount; i++)
	{
		printf("\nGet the serial number of the %d Token", i + 1);
		rv = C_GetTokenInfo(pSlotList[i], &m_Info);
		CheckResult(rv);

		CK_BYTE sn[17];
		sn[16] = 0;
		memcpy(sn, m_Info.serialNumber, 16);
		printf("\nSerial number = [%s]\n", sn);
	}

	// Free any allocated memory prior to exit.
	if (pSlotList)
		free(pSlotList);

	// Don't forget to invoke the C_Finalize function before exiting your routine.
	printf("\nFinalize the library.");
	C_Finalize(NULL_PTR);
	CheckResult(CKR_OK);
}
