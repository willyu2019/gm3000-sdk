#include <string.h>
#include <stdio.h>
#include "../../include/cryptoki.h"
#include "PKCSDemo.h"

#define	countof(a)			(sizeof(a)/ sizeof(CK_ATTRIBUTE))

char *g_strUserPIN;
CK_BBOOL bTrue = TRUE;
CK_ULONG ulModulusBits = MODULUS_BIT_LENGTH; 
CK_BYTE subject[] = "Sample RSA Key Pair";
CK_ULONG keyType = CKK_RSA;

CK_OBJECT_CLASS pubClass = CKO_PUBLIC_KEY;
CK_ATTRIBUTE pubTemplate[] = { 
	{CKA_CLASS,		&pubClass,		sizeof(pubClass)},
	{CKA_KEY_TYPE,		&keyType,		sizeof(keyType)},
	{CKA_SUBJECT,		subject,		sizeof(subject)},
	{CKA_MODULUS_BITS,	&ulModulusBits, sizeof(ulModulusBits)},
	{CKA_ENCRYPT,		&bTrue,			sizeof(bTrue)},
	{CKA_TOKEN,		&bTrue,			sizeof(bTrue)},
	{CKA_WRAP,		&bTrue,			sizeof(bTrue)},
};	// Without specifying CKA_PRIVATE attribute in this case, 
// a public key will be created by default. 

CK_OBJECT_CLASS priClass	= CKO_PRIVATE_KEY;
CK_ATTRIBUTE priTemplate[] = {
	{CKA_CLASS,		&priClass,	sizeof(priClass)},
	{CKA_KEY_TYPE,		&keyType,	sizeof(keyType)},
	{CKA_SUBJECT,		subject,	sizeof(subject)},
	{CKA_DECRYPT,		&bTrue,		sizeof(bTrue)},
	{CKA_PRIVATE,		&bTrue,		sizeof(bTrue)},
	{CKA_SENSITIVE,		&bTrue,		sizeof(bTrue)},
	{CKA_TOKEN,		&bTrue,		sizeof(bTrue)},
	{CKA_EXTRACTABLE,	&bTrue,		sizeof(bTrue)},
	{CKA_UNWRAP,		&bTrue,		sizeof(bTrue)},
};

CK_MECHANISM keyGenMechanism = {
	CKM_RSA_PKCS_KEY_PAIR_GEN, 
		NULL_PTR, 
		0
};	

CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
CK_BYTE pbMsg[] = "This is a test.";
CK_ULONG ulMsgLen = strlen((const char *)pbMsg);

CPKCSDemo::CPKCSDemo()
{
	m_pSlotList = NULL_PTR;
	m_pApplication = new char[255];
	memset(m_pApplication, 0, 255);
	strcpy((char*)m_pApplication, "PKCS Demo App");
	m_hSession = NULL_PTR;
	m_hPubKey = NULL_PTR;
	m_hPriKey = NULL_PTR;
	m_bKeyGen = FALSE;
	memset(m_pSignature, 0, MODULUS_BIT_LENGTH);
	m_ulSignatureLen = sizeof(m_pSignature);
	m_pbCipherBuffer = NULL_PTR;
	m_ulCipherLen = 0;
}

CPKCSDemo::~CPKCSDemo()
{
	if(m_hSession)
	{
		C_CloseSession(m_hSession);
		m_hSession = NULL_PTR;
	}
	delete[] (char*)m_pApplication;
	if(m_pSlotList)
	{
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
	if(m_pbCipherBuffer)
	{
		delete[] m_pbCipherBuffer;
		m_pbCipherBuffer = NULL_PTR;
		m_ulCipherLen = 0;
	}
}


CK_RV CPKCSDemo::Connect() 
{
	if(m_hSession) return CKR_OK;

	CK_RV rv;
	CK_ULONG ulCount = 0;
	rv = C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if(CKR_OK != rv )
	{
		printf("Can't Acquire information ,Error Code is 0x%08X", rv);
		return rv;
	}
	if(0 >= ulCount)
	{
		printf("\nCan't connect to Token ,Make sure you have inserted Token\n");
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
		printf("Can't Acquire information ,Error Code is 0x%08X\n", rv);
		return rv;
	}
	if(0 >= ulCount)
	{
		printf("Can't connect to Token,Make sure you have inserted Token\n");
		return rv;
	}

	rv = C_OpenSession(
		m_pSlotList[0],  CKF_RW_SESSION | CKF_SERIAL_SESSION,
		&m_pApplication, NULL_PTR, &m_hSession);
	if(CKR_OK != rv )
	{
		printf("\nCan't Acquire information ,Error Code is 0x%08X\n", rv);
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
		return rv;
	}
	else
	{
		printf("Success connect to Token\n");
		return rv;
	}
}

CK_RV CPKCSDemo::Login()
{
	
	printf("You Must input user pin before generating RSA Key pair\n");
	char s[32];
	scanf("%s",s);
	g_strUserPIN = s;
	CK_ULONG ulPIN = strlen(g_strUserPIN);
	CK_BYTE_PTR pPIN = (CK_BYTE_PTR)g_strUserPIN;

	CK_RV rv;
	rv = C_Login(m_hSession, CKU_USER, pPIN, ulPIN);
	if(CKR_OK != rv)
	{
		printf("\nCan't use your pin login to Token ,Error code 0x%08X\n", rv);
		return rv;
	}
	else
	{
		printf("Success Login to Token\n");
		return rv;
	}
}

CK_RV CPKCSDemo::Keypairgen() 
{
	CK_RV rv;

	rv = C_GenerateKeyPair(
		m_hSession, &keyGenMechanism,
		pubTemplate, countof(pubTemplate),
		priTemplate, countof(priTemplate),
		&m_hPubKey, &m_hPriKey);
	if(CKR_OK != rv)
	{
		printf("Can't Generate RSA Key Pair Error code 0x%08X", rv);
		return rv;
	}
	else
	{
		printf("Success Generate Key Pair\n");
		printf("Now,You can Sign,Verify,Encrypt and Decrypt use the Key Pair\n");
		m_bKeyGen = TRUE;
		return rv;
	}
}

void CPKCSDemo::Sign() 
{
	CK_RV rv;
	rv = C_SignInit(m_hSession, &ckMechanism, m_hPriKey);
	if(CKR_OK != rv)
	{
		printf("Fail to call SignInitError code 0x%08X\n", rv);
		return;
	}

	rv = C_Sign(m_hSession, pbMsg, ulMsgLen, m_pSignature, &m_ulSignatureLen);
	if(CKR_OK != rv)
	{
		printf("Fail to SignError code 0x%08X\n", rv);
		return;
	}
	else
	{
		printf("Data:");
		printf("\"%s\"",pbMsg);
		printf(" is Signed success\n""Sign Data is:\n");
		unsigned char *strTemp = m_pSignature;
		int i = 1;
		while(i <= m_ulSignatureLen)
		{
			printf("%02x ",*strTemp++);
			if(!(i%5)) printf("\n");
			i++;
		}
		printf("\nNow you can do RSA Verify\n");
	}
}

void CPKCSDemo::Verify() 
{
	CK_RV rv;
	rv = C_VerifyInit(m_hSession, &ckMechanism, m_hPubKey);
	if(CKR_OK != rv)
	{
		printf("Failed to call Verify Init Error code 0x%08X\n", rv);
		return;
	}

	rv = C_Verify(m_hSession, pbMsg, ulMsgLen, m_pSignature, m_ulSignatureLen);
	if(CKR_OK != rv)
	{
	       	printf("Fail to call verify Error code 0x%08X\n", rv);
	}
	else
	{
		printf("%s\n",pbMsg);
		printf("Success to Verify\n");
	}
}

void CPKCSDemo::Encrypt() 
{
	CK_RV rv;

	if(m_pbCipherBuffer)
	{
		delete[] m_pbCipherBuffer;
		m_pbCipherBuffer = NULL_PTR;
		m_ulCipherLen = 0;
	}

	rv = C_EncryptInit(m_hSession, &ckMechanism, m_hPubKey);
	if(CKR_OK != rv)
	{
		printf("Fail to call Encrypt Init !Error code 0x%08X\n", rv);
		return;
	}

	rv = C_Encrypt(m_hSession, pbMsg, ulMsgLen, NULL_PTR, &m_ulCipherLen);
	if(CKR_OK != rv)
	{
		printf("Can't acquire the size of Data After encrypt,Error code 0x%08X\n", rv);
		return;
	}

	m_pbCipherBuffer = (CK_BYTE_PTR)new CK_BYTE[m_ulCipherLen];
	if (! m_pbCipherBuffer)
	{
		printf("Can't allcate enough memory\n");
		return;
	}
	memset(m_pbCipherBuffer, 0, m_ulCipherLen);

	rv = C_Encrypt(m_hSession, pbMsg, ulMsgLen, m_pbCipherBuffer, &m_ulCipherLen);
	if (CKR_OK != rv)
	{
		printf("Fail to encrypt Error code 0x%08X\n", rv);
		return;
	}
	printf("Data: ");
	printf("\"%s\"",pbMsg);
	printf("was encrypted success\n""Encryted Data is\n");
	int i = 1 ;
	unsigned char * strTemp = m_pbCipherBuffer;
	while(i <= m_ulCipherLen)
	{
		printf("%02x ", *strTemp++);
		if(!(i%5)) printf("\n");
		i++;
	}
	printf("\nNow You can decrypt use RSA private key\n");
}

void CPKCSDemo::Decrypt() 
{
	CK_BYTE_PTR pbRestoredMsg = NULL;
	CK_ULONG ulRestoredMsgLen = 0;
	CK_RV rv;
	rv = C_DecryptInit(m_hSession, &ckMechanism, m_hPriKey);
	if(CKR_OK != rv)
	{
		printf("Fail to call Decrypt Init Error code 0x%08X\n", rv);
		return;
	}

	rv = C_Decrypt(m_hSession, m_pbCipherBuffer, m_ulCipherLen, NULL_PTR, &ulRestoredMsgLen);
	if(CKR_OK != rv)
	{
		printf("Can't acuire size of Data after Decrypt! Error code 0x%08X\n", rv);
		return;
	}

	pbRestoredMsg = (CK_BYTE_PTR)new CK_BYTE[ulRestoredMsgLen + 1];
	if (! pbRestoredMsg)
	{
		printf("Can't allocate enough memory for Decrypt\n");
		return;
	}

	memset(pbRestoredMsg, 0, ulRestoredMsgLen + 1);

	rv = C_Decrypt(m_hSession, m_pbCipherBuffer, m_ulCipherLen, pbRestoredMsg, &ulRestoredMsgLen);
	if (CKR_OK != rv)
	{
		printf("Fail to call decrypt,Error code 0x%08X\n", rv);
		delete[] pbRestoredMsg;
		return;
	}
	printf("Success to Decrypt!\nData after Decrypt is:");
	printf("\"%s\"\n",pbRestoredMsg);
	delete[] pbRestoredMsg;
}
void CPKCSDemo::Destroy()
{
	printf("Now clear the Public and Private key!\n");
	CK_RV ck_rv;
	ck_rv = C_DestroyObject(m_hSession, m_hPubKey);
	if(CKR_OK != ck_rv) printf("Can't clear Public key!\n");
	ck_rv = C_DestroyObject(m_hSession, m_hPriKey);
	if(CKR_OK != ck_rv) printf("Can't clear Private key!\n");
	printf("Clear key is ok!\n");
}
