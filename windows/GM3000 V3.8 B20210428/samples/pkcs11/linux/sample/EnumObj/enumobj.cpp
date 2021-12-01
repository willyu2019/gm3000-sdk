#include<iostream>
#include <string.h>
#include <stdio.h>
#include "../../include/cryptoki.h"
#include "enumobj.h"

using namespace std;

EnumObj::EnumObj()
{
	m_pSlotList = NULL_PTR;
	m_hSession = NULL_PTR;
}

EnumObj::~EnumObj()
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

CK_RV EnumObj::Connect() 
{
	CK_RV rv;
	CK_ULONG ulCount = 0;
	rv = C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if(CKR_OK != rv )
	{
		printf("\nCan't Acquire the Inoformation of Token.Error Code 0x%08X.\n", rv);
		return rv;
	}
	if(0 >= ulCount)
	{
		printf("\nCan't Connect to Token.Make Sure You have inserted USB Token.\n");
		return CKR_GENERAL_ERROR;
	}
	
	m_pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
	if (! m_pSlotList) 
	{
		printf("\nCan't allocate enough memory\n");
		return CKR_HOST_MEMORY;
	}
	
	rv = C_GetSlotList(TRUE, m_pSlotList, &ulCount);
	if(CKR_OK != rv )
	{
		printf("\nCan't Acquire the Inoformation of Token.Error Code 0x%08X.\n", rv);
		return rv;
	}
	if(0 >= ulCount)
	{
		printf("\nCan't Connect to Token.Make Sure You have inserted Token.\n");
		return CKR_HOST_MEMORY;
	}
	
	rv = C_OpenSession(
		m_pSlotList[0],  CKF_RW_SESSION | CKF_SERIAL_SESSION,
		NULL_PTR, NULL_PTR, &m_hSession);
	if(CKR_OK != rv )
	{
		printf("\nCan't Connect to Token.Error Code 0x%08X.\n", rv);
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
		return rv;
	}
	else
	{
		printf("Success connect to Token\n");
		return CKR_OK;
	}
}

void EnumObj::StartOP()
{
	printf("\n================================================");
}

void EnumObj::Enum() 
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
	int numObj=0;
	do
	{
		ckrv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
		if(CKR_OK != ckrv)
		{
			break;	
		}
		if(1 != ulRetCount)
			break;
		
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

		numObj++;
		char *strvale = (char*)pAttrTemp[2].pValue;
		
		unsigned char *strsubject;
		strsubject = (unsigned char*)pAttrTemp[3].pValue;
		unsigned char *strckaid=(unsigned char*)pAttrTemp[4].pValue;
		unsigned char *strckavalue=(unsigned char*)pAttrTemp[5].pValue;
		
		printf("\n");
		StartOP();
		printf("\nBegin this Object's Output:\n");
		printf("The Attribute CKA_CLASS of this Obj is:: CKO_CERTIFICATE\n");
		
		if(*(int*)pAttrTemp[1].pValue==CKC_X_509)
		{
		printf("The Attribute CKA_CERTIFICATE_TYPE is: CKC_X_509\n");
		}
		else 
			if(*(int*)pAttrTemp[1].pValue==CKC_X_509_ATTR_CERT)
			{
				printf("CKA_CERTIFICATE_TYPE is CKC_X_509_ATTR_CERT");
			}
		printf("The Attribute CKA_LABEL of this Obj is: ");
		printf("%s\n",strvale);
		printf("The Attribute CKA_SUBJECT of this Obj is: \n");
		unsigned int i = 1;
		while(i <= pAttrTemp[3].ulValueLen)
		{
			printf("%02x ",*strsubject++);
			if(!(i%5)) printf("\n");
			i++;
		}
		printf("\nThe Attribute CKA_ID of this Obj is: \n");
		i = 1;
		while(i <= pAttrTemp[4].ulValueLen)
		{
			printf("%02x ",*strckaid++);
			if(!(i%5)) printf("\n");
			i++;
		}
		printf("\nThe Content of this Obj(CKA_VALUE) is:\n");
		i = 1;
		while(i <= pAttrTemp[5].ulValueLen)
		{
			printf("%02x ",*strckavalue++);
			if(!(i%5)) printf("\n");
			i++;
		}
		printf("\nFinish Output Obj\n");
		
		
		delete[](char *) pAttrTemp[0].pValue;
		delete[](char *) pAttrTemp[1].pValue;
		delete[](char *) pAttrTemp[2].pValue;
		delete[](char *) pAttrTemp[3].pValue;
		delete[](char *) pAttrTemp[4].pValue;
		delete[](char *) pAttrTemp[5].pValue;
		
	}while(true);
	if(numObj==0)
	{
		StartOP();
		printf("\nCan't find X.509 Certificate Obj\n");
	}
	else
	{
		StartOP();
		char strNumObj[4];
		sprintf(strNumObj,"%d",numObj);
		printf("\nFind ");
		printf(strNumObj);
		printf(" X.509 Certificate Obj\n");
	}
	
	C_FindObjectsFinal(m_hSession);
	
}

CK_RV EnumObj::Login() 
{
	CK_RV rv;
	string sTemp;

	printf("Please input your PIN number !\n");
	cin>>sTemp;
	rv = C_Login(m_hSession, CKU_USER, (CK_BYTE_PTR)&sTemp[0], sTemp.length());
	if(CKR_OK != rv)
	{
		printf("\nCan't use your pin login to Token,Error Code 0x%08X.\n", rv);
		return CKR_GENERAL_ERROR;
	}
	else
	{
		printf("\nSuccess login to Token\n");
	}

	return CKR_OK;
}

void EnumObj::Data() 
{
	CK_OBJECT_CLASS dataClass = CKO_DATA;
	bool IsToken=true;
	CK_ATTRIBUTE pTempl[] = 
	{
		{CKA_CLASS, &dataClass, sizeof(CKO_DATA)},
		{CKA_TOKEN, &IsToken, sizeof(true)}
	};
	
	C_FindObjectsInit(m_hSession, pTempl, 2);
	
	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_RV ckrv = 0;

	int numObj=0;
	do
	{
		ckrv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
		if(CKR_OK != ckrv)
		{
			break;	
		}
		if(1 != ulRetCount)
			break;
		
		CK_ATTRIBUTE pAttrTemp[] = 
		{
			{CKA_CLASS, NULL, 0},
			{CKA_LABEL, NULL, 0},
			{CKA_APPLICATION, NULL, 0},
			{CKA_VALUE, NULL,0},
		};
		
		ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 4);
		if(ckrv != CKR_OK)
		{
			break;
		}
		
		pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
		pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen + 1];
		pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen + 1];
		pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen];
		memset(pAttrTemp[0].pValue, 0, pAttrTemp[0].ulValueLen);
		memset(pAttrTemp[1].pValue, 0, pAttrTemp[1].ulValueLen + 1);
		memset(pAttrTemp[2].pValue, 0, pAttrTemp[2].ulValueLen + 1);
		memset(pAttrTemp[3].pValue, 0, pAttrTemp[3].ulValueLen );

		ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 4);
		if(ckrv != CKR_OK)
		{
			delete[](char*) pAttrTemp[0].pValue;
			delete[](char*) pAttrTemp[1].pValue;
			delete[](char*) pAttrTemp[2].pValue;
			delete[](char*) pAttrTemp[3].pValue;
			break;
		}
		numObj++;		
		char *strVale = (char*)pAttrTemp[1].pValue;
		char *strApp= (char*)pAttrTemp[2].pValue;
		char *strCkaVale= (char*)pAttrTemp[3].pValue;

		printf("\n");
		StartOP();
		printf("\nBegin this Object's Output:\n");
		printf("The Attribute CKA_CLASS of this Obj is: CKO_DATA\n");
		printf("The Attribute CKA_LABEL of this Obj is: \n");
		unsigned int i = 1;
		while(i <= pAttrTemp[1].ulValueLen)
		{
			printf("%02x ",*strVale++);
			if(!(i%5)) printf("\n");
			i++;
		}
		printf("\nThe Attribute CKA_APPLICATION of this Obj is:\n");
		i = 1;
		while(i <= pAttrTemp[2].ulValueLen)
		{
			printf("%02x ",*strApp++);
			if(!(i%5)) printf("\n");
			i++;
		}
		printf("\nThe Content of this Obj(CKA_VALUE) is:\n");
		i = 1;
		while(i <= pAttrTemp[3].ulValueLen)
		{
			printf("%02x ",*strCkaVale++);
			if(!(i%5)) printf("\n");
			i++;
		}
		printf("\nFinish Output Obj\n");
		
		delete[](char*) pAttrTemp[0].pValue;
		delete[](char*) pAttrTemp[1].pValue;
		delete[](char*) pAttrTemp[2].pValue;
		delete[](char*) pAttrTemp[3].pValue;		
	}while(true);

	if(numObj==0)
	{
		StartOP();
		printf("\nCan't find Data Obj\n");
	}
	else
	{
		StartOP();
		char strNumObj[4];
		sprintf(strNumObj,"%d",numObj);
		printf("\nFind ");
		printf(strNumObj);
		printf(" Data Obj\n");
	}
	
	C_FindObjectsFinal(m_hSession);

}

void EnumObj::Public() 
{
	CK_OBJECT_CLASS dataClass = CKO_PUBLIC_KEY;
	bool IsToken=true;
	CK_ATTRIBUTE pTempl[] = 
	{
		{CKA_CLASS, &dataClass, sizeof(CKO_PUBLIC_KEY)},
		{CKA_TOKEN, &IsToken, sizeof(true)}
	};
	
	C_FindObjectsInit(m_hSession, pTempl, 2);
	
	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_RV ckrv = 0;
	int numObj=0;
	do
	{
		ckrv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
		if(CKR_OK != ckrv)
		{
			break;	
		}
		if(1 != ulRetCount)
			break;
		
		CK_ATTRIBUTE pAttrTemp[] = 
		{
			{CKA_CLASS, NULL, 0},
			{CKA_KEY_TYPE,NULL,0},
			{CKA_LABEL, NULL, 0},
			{CKA_WRAP,NULL,0},
			{CKA_ENCRYPT,NULL,0},
			{CKA_MODULUS,NULL,0},
			{CKA_PUBLIC_EXPONENT,NULL,0},
		};
		
		ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 7);
		if(ckrv != CKR_OK)
		{
			break;
		}
		
		pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
		pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
		pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen+1];
		pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen];
		pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen];
		pAttrTemp[5].pValue = new char[pAttrTemp[5].ulValueLen];
		pAttrTemp[6].pValue = new char[pAttrTemp[6].ulValueLen];
		
		memset(pAttrTemp[0].pValue, 0, pAttrTemp[0].ulValueLen);
		memset(pAttrTemp[1].pValue, 0, pAttrTemp[1].ulValueLen);
		memset(pAttrTemp[2].pValue, 0, pAttrTemp[2].ulValueLen+1);	
		memset(pAttrTemp[3].pValue, 0, pAttrTemp[3].ulValueLen);
		memset(pAttrTemp[4].pValue, 0, pAttrTemp[4].ulValueLen);
		memset(pAttrTemp[5].pValue, 0, pAttrTemp[5].ulValueLen);
		memset(pAttrTemp[6].pValue, 0, pAttrTemp[6].ulValueLen);
		
		ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 7);
		if(ckrv != CKR_OK)
		{
			delete[](char*) pAttrTemp[0].pValue;
			delete[](char*) pAttrTemp[1].pValue;
			delete[](char*) pAttrTemp[2].pValue;
			delete[](char*) pAttrTemp[3].pValue;
			delete[](char*) pAttrTemp[4].pValue;
			delete[](char*) pAttrTemp[5].pValue;
			delete[](char*) pAttrTemp[6].pValue;
			break;
		}

		numObj++;

		printf("\n");
		StartOP();
		printf("\nBegin this Object's Output:\n");
		printf("The Attribute CKA_CLASS of this Obj is:: CKO_PUBLIC_KEY\n");
		
		if(*(int*)pAttrTemp[1].pValue==CKK_RSA)
		{
			printf("The Attribute CKA_KEY_TYPE is: CKK_RSA\n");
		}
		else if(*(int*)pAttrTemp[1].pValue==CKK_DSA)
		{
			printf("The Attribute CKA_KEY_TYPE is CKK_DSA\n");
		}
		else if(*(int*)pAttrTemp[1].pValue==CKK_DH)
		{
			printf("The Attribute CKA_KEY_TYPE is CKK_DH\n");					
		}

		unsigned char *strCkaLabel = (unsigned char*)pAttrTemp[2].pValue;
		printf("The Attribute CKA_LABEL of this Obj is: ");
		printf("%s", strCkaLabel);
		
		if((int)*(char*)pAttrTemp[3].pValue!=0)
		{
			printf("\nThe Attribute CKA_WRAP is: true");
		}
		else
		{
			printf("\nThe Attribute CKA_WRAP is: false");
		}

		if((int)*(char*)pAttrTemp[4].pValue!=0)
		{
			printf("\nThe Attribute CKA_ENCRYPT is: true");
		}
		else
		{
			printf("\nThe Attribute CKA_ENCRYPT is: false");
		}
		
		unsigned char *strCkaModulus =(unsigned char *)pAttrTemp[5].pValue;
		printf("\nThe Attribute CKA_MODULUS(public key) is:\n");
		unsigned int i = 1;
		while(i <= pAttrTemp[5].ulValueLen)
		{
			printf("%02x ", *strCkaModulus++);
			if(!(i%5)) printf("\n");
			i++;
		}
		char *strCkaPublic=(char *)pAttrTemp[6].pValue;
		printf("\nThe Attribute CKA_PUBLIC_EXPONENT is:\n");
		i = 1;
		while(i <= pAttrTemp[6].ulValueLen)
		{
			printf("%02x ", *strCkaPublic++);
			if(!(i%5)) printf("\n");
			i++;
		}

		printf("\nFinish Output Obj\n");
		
		delete[](char *) pAttrTemp[0].pValue;
		delete[](char *) pAttrTemp[1].pValue;
		delete[](char *) pAttrTemp[2].pValue;
		delete[](char *) pAttrTemp[3].pValue;
		delete[](char *) pAttrTemp[4].pValue;
		delete[](char *) pAttrTemp[5].pValue;
		delete[](char *) pAttrTemp[6].pValue;
		
	}while(true);
	if(numObj==0)
	{
		StartOP();
		printf("\nCan't find public key Obj\n");
	}
	else
	{
		StartOP();
		char strNumObj[4];
		sprintf(strNumObj,"%d",numObj);
		printf("\nFind ");
		printf(strNumObj);
		printf(" public key\n");
	}

	C_FindObjectsFinal(m_hSession);

}

void EnumObj::Private()
{
	CK_OBJECT_CLASS dataClass = CKO_PRIVATE_KEY;
	bool IsToken=true;
	CK_ATTRIBUTE pTempl[] =
	{
		{CKA_CLASS, &dataClass, sizeof(CKO_PRIVATE_KEY)},
		{CKA_TOKEN, &IsToken, sizeof(true)}
	};
	
	
	C_FindObjectsInit(m_hSession, pTempl, 2);
	
	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_RV ckrv = 0;
	int numObj=0;
	do
	{
		ckrv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
		if(CKR_OK != ckrv)
		{
			break;	
		}
		if(1 != ulRetCount)
			break;
		
		CK_ATTRIBUTE pAttrTemp[] = 
		{
			{CKA_CLASS, NULL, 0},
			{CKA_KEY_TYPE,NULL,0},
			{CKA_LABEL, NULL, 0},
			{CKA_SUBJECT,NULL,0},
			{CKA_ID,NULL,0},
			{CKA_SENSITIVE,NULL,0},
			{CKA_DECRYPT,NULL,0},
			{CKA_SIGN,NULL,0},
			{CKA_MODULUS,NULL,0},
			{CKA_PUBLIC_EXPONENT,NULL,0},
		};
		
		ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 10);
		if(ckrv != CKR_OK)
		{
			break;
		}
		
		pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
		pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
		pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen+1];
		pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen+1];
		pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen+1];
		pAttrTemp[5].pValue = new char[pAttrTemp[5].ulValueLen];
		pAttrTemp[6].pValue = new char[pAttrTemp[6].ulValueLen];
		pAttrTemp[7].pValue = new char[pAttrTemp[7].ulValueLen];
		pAttrTemp[8].pValue = new char[pAttrTemp[8].ulValueLen];
		pAttrTemp[9].pValue = new char[pAttrTemp[9].ulValueLen];
		
		memset(pAttrTemp[0].pValue, 0, pAttrTemp[0].ulValueLen);
		memset(pAttrTemp[1].pValue, 0, pAttrTemp[1].ulValueLen);
		memset(pAttrTemp[2].pValue, 0, pAttrTemp[2].ulValueLen+1);	
		memset(pAttrTemp[3].pValue, 0, pAttrTemp[3].ulValueLen+1);
		memset(pAttrTemp[4].pValue, 0, pAttrTemp[4].ulValueLen+1);
		memset(pAttrTemp[5].pValue, 0, pAttrTemp[5].ulValueLen);
		memset(pAttrTemp[6].pValue, 0, pAttrTemp[6].ulValueLen);
		memset(pAttrTemp[7].pValue, 0, pAttrTemp[7].ulValueLen);
		memset(pAttrTemp[8].pValue, 0, pAttrTemp[8].ulValueLen);
		memset(pAttrTemp[9].pValue, 0, pAttrTemp[9].ulValueLen);
		
		ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 10);
		if(ckrv != CKR_OK)
		{
			delete[](char*) pAttrTemp[0].pValue;
			delete[](char*) pAttrTemp[1].pValue;
			delete[](char*) pAttrTemp[2].pValue;
			delete[](char*) pAttrTemp[3].pValue;
			delete[](char*) pAttrTemp[4].pValue;
			delete[](char*) pAttrTemp[5].pValue;
			delete[](char*) pAttrTemp[6].pValue;
			delete[](char*) pAttrTemp[7].pValue;
			delete[](char*) pAttrTemp[8].pValue;
			delete[](char*) pAttrTemp[9].pValue;
			//			bRet = false;
			break;
		}
		numObj++;
		
		unsigned char *strVale = (unsigned char*)pAttrTemp[2].pValue;
		unsigned char *strSubject=(unsigned char*)pAttrTemp[3].pValue;
		unsigned char *strCkaId=(unsigned char*)pAttrTemp[4].pValue;
		
		printf("\n");
		StartOP();
		printf("\nBegin this Object's Output:\n");
		printf("The Attribute CKA_CLASS of this Obj is:: CKO_PRIVATE_KEY\n");
		
		if(*(int*)pAttrTemp[1].pValue==CKK_RSA)
		{
			printf("The Attribute CKA_KEY_TYPE is : CKK_RSA\n");
		}
		else 
			if(*(int*)pAttrTemp[1].pValue==CKK_DSA)
			{
				printf("The Attribute CKA_KEY_TYPE is CKK_DSA\n");
			}
			else
				if(*(int*)pAttrTemp[1].pValue==CKK_DH)
				{
					printf("The Attribute CKA_KEY_TYPE is CKK_DH\n");					
				}
				
		printf("The Attribute CKA_LABEL of this Obj is: ");
		printf("%s",strVale);
		printf("\nThe Attribute CKA_SUBJECT of this Obj is: ");
		unsigned int i = 1;
		while(i <= pAttrTemp[3].ulValueLen)
		{
			printf("%02x ",*strSubject++);
			if(!(i%5)) printf("\n");
			i++;
		}

		printf("\nThe Attribute CKA_ID of this Obj is: ");
		i = 1;
		while(i <= pAttrTemp[4].ulValueLen)
		{
			printf("%02x ",*strCkaId++);
			if(!(i%5)) printf("\n");
			i++;
		}
			
		if((int)*(char*)pAttrTemp[5].pValue!=0)
		{
			printf("\nThe Attribute CKA_SENSITIVE is: true");
		}
		else
		{
			printf("\nThe Attribute CKA_SENSITIVE is: false");
		}
			
			
		if((int)*(char*)pAttrTemp[6].pValue!=0)
		{
			printf("\nThe Attribute CKA_DECRYPT is: true");
		}
		else
		{
			printf("\nThe Attribute CKA_DECRYPT is: false");
		}

		if((int)*(char*)pAttrTemp[7].pValue!=0)
		{
			printf("\nThe Attribute CKA_SIGN is: true");
		}
		else
		{
			printf("\nThe Attribute CKA_SIGN is: false");
		}
			
			
		printf("\nPrivate Key can't output because of the limit of hardware:\n");

		
		printf("\nFinish Output Obj\n");
		
		
		delete[](char*) pAttrTemp[0].pValue;
		delete[](char*) pAttrTemp[1].pValue;
		delete[](char*) pAttrTemp[2].pValue;
		delete[](char*) pAttrTemp[3].pValue;
		delete[](char*) pAttrTemp[4].pValue;
		delete[](char*) pAttrTemp[5].pValue;
		delete[](char*) pAttrTemp[6].pValue;
		delete[](char*) pAttrTemp[7].pValue;
		delete[](char*) pAttrTemp[8].pValue;
		delete[](char*) pAttrTemp[9].pValue;

	}while(true);
	
	if(numObj==0)
	{
		StartOP();
		printf("\nCan's find private key Obj\n");
	}
	else
	{
		StartOP();
		char strNumObj[4];
		sprintf(strNumObj,"%d",numObj);
		printf("\nFind ");
		printf(strNumObj);
		printf(" private key\n");
	}
	
	C_FindObjectsFinal(m_hSession);

}

void EnumObj::Secret() 
{
	CK_OBJECT_CLASS dataClass = CKO_SECRET_KEY;
	bool IsToken=true;
	CK_ATTRIBUTE pTempl[] = 
	{
		{CKA_CLASS, &dataClass, sizeof(CKO_PUBLIC_KEY)},
		{CKA_TOKEN, &IsToken, sizeof(true)}
	};
	
	C_FindObjectsInit(m_hSession, pTempl, 2);
	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_RV ckrv = 0;
	int numObj=0;

	do
	{
		ckrv = C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
		if(CKR_OK != ckrv)
		{
			break;
		}
		if(1 != ulRetCount)
			break;
		
		CK_ATTRIBUTE pAttrTemp[] = 
		{
			{CKA_CLASS, NULL, 0},
			{CKA_KEY_TYPE,NULL,0},
			{CKA_LABEL, NULL, 0},
			{CKA_DERIVE,NULL,0},
			{CKA_VALUE,NULL,0},
		};
		
		ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 5);
		if(ckrv != CKR_OK)
		{
			break;
		}
		
		pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
		pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
		pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen+1];
		pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen];
		pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen];

		
		memset(pAttrTemp[0].pValue, 0, pAttrTemp[0].ulValueLen);
		memset(pAttrTemp[1].pValue, 0, pAttrTemp[1].ulValueLen);
		memset(pAttrTemp[2].pValue, 0, pAttrTemp[2].ulValueLen+1);	
		memset(pAttrTemp[3].pValue, 0, pAttrTemp[3].ulValueLen);
		memset(pAttrTemp[4].pValue, 0, pAttrTemp[4].ulValueLen);

		
		ckrv = C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 5);
		if(ckrv != CKR_OK)
		{
			delete[](char*) pAttrTemp[0].pValue;
			delete[](char*) pAttrTemp[1].pValue;
			delete[](char*) pAttrTemp[2].pValue;
			delete[](char*) pAttrTemp[3].pValue;
			delete[](char*) pAttrTemp[4].pValue;
			break;
		}

		numObj++;

		char *strVale = (char*)pAttrTemp[2].pValue;
		
		printf("\n");
		StartOP();
		printf("\nBegin this Object's Output:\n");
		printf("The Attribute CKA_CLASS of this Obj is:: CKO_SECRET_KEY\n");
		
		if(*(int*)pAttrTemp[1].pValue==CKK_GENERIC_SECRET)
		{
		printf("The Attribute CKA_KEY_TYPE is: CKK_GENERIC_SECRET\n");
		}
		else if(*(int*)pAttrTemp[1].pValue==CKK_RC2)
		{
			printf("The Attribute CKA_KEY_TYPE is CKK_RC2\n");
		}
		else if(*(int*)pAttrTemp[1].pValue==CKK_RC4)
		{
			printf("The Attribute CKA_KEY_TYPE is CKK_RC4\n");					
		}
		else if(*(int*)pAttrTemp[1].pValue==CKK_DES)
		{
			printf("The Attribute CKA_KEY_TYPE is CKK_DES\n");					
		}
		else if(*(int*)pAttrTemp[1].pValue==CKK_DES2)
		{
			printf("The Attribute CKA_KEY_TYPE is CKK_DES2\n");					
		}
		else if(*(int*)pAttrTemp[1].pValue==CKK_DES3)
		{
			printf("The Attribute CKA_KEY_TYPE is CKK_DES3\n");					
		}
					
		printf("The Attribute CKA_LABEL of this Obj is: ");
		printf("%s",strVale);
		
		if((int)*(char*)pAttrTemp[3].pValue!=0)
		{
			printf("\nThe Attribute CKA_DERIVE is: true");
		}
		else
		{
			printf("\nThe Attribute CKA_DERIVE is : false");
		}

		unsigned char *strCkaValue = (unsigned char *)pAttrTemp[4].pValue;
		printf("\nCKA_VALUE(Content of key) is:\n");
		unsigned int i = 1;
		while(i <= pAttrTemp[4].ulValueLen)
		{
			printf("%02x ", *strCkaValue++);
			if(!(i%5)) printf("\n");
			i++;
		}
		printf("\nFinish Output Obj\n");
		
		delete[](char*) pAttrTemp[0].pValue;
		delete[](char*) pAttrTemp[1].pValue;
		delete[](char*) pAttrTemp[2].pValue;
		delete[](char*) pAttrTemp[3].pValue;
		delete[](char*) pAttrTemp[4].pValue;
		
	}while(true);

	if(numObj==0)
	{
		StartOP();
		printf("\nCan't find secret key\n");
	}
	else
	{
		StartOP();
		char strNumObj[4];
		sprintf(strNumObj,"%d",numObj);
		printf("\nFind ");
		printf(strNumObj);
		printf(" secret key\n");
	}
	
	C_FindObjectsFinal(m_hSession);
}
