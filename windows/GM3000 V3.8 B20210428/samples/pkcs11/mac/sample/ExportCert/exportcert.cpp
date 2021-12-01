#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../include/cryptoki.h"
#include "exportcert.h"
#include <iostream>
using namespace std;

CExportcert::CExportcert()
{
	m_pSlotList = NULL_PTR;
	m_hSession = NULL_PTR;
}

CExportcert::~CExportcert()
{
	if(m_hSession)
	{
		C_CloseSession(m_hSession);
		m_hSession = NULL_PTR;
	}
	if(m_pSlotList)
	{
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
}

CK_RV CExportcert::Init() 
{
	if(m_hSession) return CKR_OK;
	
	CK_RV rv;
	CK_ULONG ulCount = 0;
	rv = C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if(CKR_OK != rv )
	{
		printf("Can't aquire Token Information\n");
		return rv;
	}
	if(0 >= ulCount)
	{
		printf("Can't connect to Token, Make sure you have inserted Token.\n");
		return CKR_GENERAL_ERROR;
	}
	
	m_pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
	if (! m_pSlotList) 
	{
		printf("can't allocate enough memory\n");
		return CKR_HOST_MEMORY;
	}
	
	rv = C_GetSlotList(TRUE, m_pSlotList, &ulCount);
	if(CKR_OK != rv )
	{
		printf("Can't aquire Token Information\n");
		return rv;
	}
	if(0 >= ulCount)
	{
		printf("Can't connect to Token, Make sure you have inserted Token.\n");
		return CKR_GENERAL_ERROR;
	}
	
	rv = C_OpenSession(
		m_pSlotList[0],  CKF_RW_SESSION | CKF_SERIAL_SESSION,
		NULL_PTR, NULL_PTR, &m_hSession);
	if(CKR_OK != rv )
	{
		printf("Can't aquire Token Information\n");
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
		return rv;
	}
	return rv;
}

void CExportcert::Export() 
{
	CK_OBJECT_CLASS dataClass = CKO_CERTIFICATE;
	bool IsToken=true;
	CK_ATTRIBUTE pTempl[] = 
	{
		{CKA_CLASS, &dataClass, sizeof(CKO_CERTIFICATE)},
		{CKA_TOKEN, &IsToken, sizeof(true)}
	};
	
	C_FindObjectsInit(m_hSession, pTempl, 2);

	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_RV ckrv = 0;
	CK_ULONG conut = 0;
	CK_LONG i = 0;
	do
	{
		ckrv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
		if(CKR_OK != ckrv)
		{
			break;	
		}
		if(1 != ulRetCount)
		{
			break;
		}
		
		CK_ATTRIBUTE pAttrTemp[] = 
		{
			{CKA_CLASS, NULL, 0},
			{CKA_CERTIFICATE_TYPE,NULL,0},
			{CKA_LABEL, NULL, 0},
			{CKA_SUBJECT,NULL,0},
			{CKA_ID,NULL,0},
			{CKA_VALUE,NULL,0}
		};
		
		ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 6);
		if(ckrv != CKR_OK)
		{
			break;
		}
		
		pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
		pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
		pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen+1];
		pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen+1];
		pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen+1];
		pAttrTemp[5].pValue = new char[pAttrTemp[5].ulValueLen ];
	
		memset(pAttrTemp[0].pValue, 0, pAttrTemp[0].ulValueLen);
		memset(pAttrTemp[1].pValue, 0, pAttrTemp[1].ulValueLen);
		memset(pAttrTemp[2].pValue, 0, pAttrTemp[2].ulValueLen+1);	
		memset(pAttrTemp[3].pValue, 0, pAttrTemp[3].ulValueLen+1);
		memset(pAttrTemp[4].pValue, 0, pAttrTemp[4].ulValueLen+1);
		memset(pAttrTemp[5].pValue, 0, pAttrTemp[5].ulValueLen);
		
		ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 6);
		if(ckrv != CKR_OK)
		{
			delete[](char*) pAttrTemp[0].pValue;
			delete[](char*) pAttrTemp[1].pValue;
			delete[](char*) pAttrTemp[2].pValue;
			delete[](char*) pAttrTemp[3].pValue;
			delete[](char*) pAttrTemp[4].pValue;
			delete[](char*) pAttrTemp[5].pValue;
			break;
		}
		
		char *strvale = (char*)pAttrTemp[2].pValue;
		int fd;
		char s[64];
		i++;
		int j,h = strlen(strvale);
		for(j = 0; j < h;j++)
		{
			if(strvale[j] == ' ')
				strvale[j] = '_';
		}
		sprintf(s,"%s%d.cer",strvale,i);
		fd = open(s,O_RDWR|O_CREAT,0644);
		if(0 > fd)
		{
			printf("Open file fault\n");
			return ;
		}
		write(fd, pAttrTemp[5].pValue, pAttrTemp[5].ulValueLen);
		close(fd);
		printf("Export successful.\n");

		delete[](char*) pAttrTemp[0].pValue;
		delete[](char*) pAttrTemp[1].pValue;
		delete[](char*) pAttrTemp[2].pValue;
		delete[](char*) pAttrTemp[3].pValue;
		delete[](char*) pAttrTemp[4].pValue;
		delete[](char*) pAttrTemp[5].pValue;
		conut++;
	}while(true);
	if(conut > 1)
	{
		printf("Export %d certificates\n", conut);
	}else
	{
		printf("Export %d certificate\n", conut);
	}
	
	C_FindObjectsFinal(m_hSession);

}
