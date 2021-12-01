/*
[]=========================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	rc4test.cpp

DESC:
	implementation of the RC4Test class.
[]=========================================================================[]
*/

#include "RC4Test.h"
#include "common.h"
RC4Test::RC4Test(char* dll_file_path):CBaseAll(dll_file_path)
{
	m_hKey = 0;
}

RC4Test::~RC4Test()
{
	m_hKey = 0;
}

void RC4Test::Test()
{
	if(CKR_OK != BaseAllStart())
		return;
	
	GenerateKey();
	if(m_hKey == 0)
		return ;
	crypt_single();
	crypt_Update();
	BaseAllEnd();
}

void RC4Test::GenerateKey()
{
	do{
		CK_OBJECT_CLASS oClass = CKO_SECRET_KEY;
		CK_KEY_TYPE keyType = CKK_RC4; 
		CK_BBOOL bTrue = true;
		CK_BBOOL bFalse = false;
		CK_ULONG ulLen = 16;
		CK_ATTRIBUTE rc4tem[] = {
			{CKA_CLASS, &oClass, sizeof(CK_OBJECT_CLASS)},
			{CKA_KEY_TYPE, &keyType, sizeof(CK_KEY_TYPE)},
			{CKA_TOKEN, &bFalse, sizeof(CK_BBOOL)},
			{CKA_PRIVATE, &bTrue, sizeof(CK_BBOOL)},
			{CKA_ENCRYPT, &bTrue, sizeof(CK_BBOOL)},
			{CKA_DECRYPT, &bTrue, sizeof(CK_BBOOL)},
			{CKA_VALUE_LEN, &ulLen, sizeof(CK_ULONG)}, 
		};
		CK_ULONG ulCount = 7;
		CK_MECHANISM mechanism = {CKM_RC4_KEY_GEN, NULL, 0};
		//generate key:
		START_OP("generate RC4 key...");
		CK_RV rv =  m_gToken->C_GenerateKey(hSession, &mechanism, rc4tem, ulCount, &m_hKey); 
		CHECK_OP(rv);
	}while(0);

}

void RC4Test::crypt_single()
{
	const CK_ULONG DATA_LENGTH = 1024;
	CK_BYTE bIn[DATA_LENGTH] = {0}, bTemp[DATA_LENGTH] = {0}, bOut[DATA_LENGTH] = {0};
	CK_ULONG ulIn = 0, ulOut = 0, ulTemp = 0;
	CK_ULONG Mechanism = {CKM_RC4};
	CK_BYTE_PTR bHint = {(CK_BYTE_PTR)"CKM_RC4: "};
	SHOW_INFO("\nRC4: C_Encrypt/C_Decrypt: \n");
	do{
		ulIn = 256;
		for(register CK_ULONG i0 = 0;i0<ulIn;i0++)
			bIn[i0] = (CK_BYTE)i0;
		SHOW_INFO("\n*	*	*	*	*	*	*	*	*	*	*	\n");
		SHOW_INFO(bHint);
		//ecnrypt init:
		CK_MECHANISM ckMechanism = {Mechanism, NULL, 0};
		START_OP("Encrypting initialize.");
		CK_RV rv =  m_gToken->C_EncryptInit(hSession, &ckMechanism, m_hKey); 
		CHECK_OP(rv);
		
		START_OP("Encrypt the message.");
		//Get seze:
		ulTemp = 0;
		rv =  m_gToken->C_Encrypt(hSession, bIn, ulIn, NULL, &ulTemp);
		//encrypt:
		rv =  m_gToken->C_Encrypt(hSession, bIn, ulIn, bTemp, &ulTemp);
		CheckRV("C_Encrypt", rv);
		CHECK_OP(rv);
		SHOW_INFO("Data encrypted: \n");
		ShowData(bTemp, ulTemp);
		
		START_OP("Decrypting initialize.");
		rv =  m_gToken->C_DecryptInit(hSession, &ckMechanism, m_hKey);
		CHECK_OP(rv);
		START_OP("Decrypt the message.");
		//Get size:
		ulOut = 0;
		rv =  m_gToken->C_Decrypt(hSession, bTemp, ulTemp, NULL, &ulOut);
		//Get decrypted data:
		rv =  m_gToken->C_Decrypt(hSession, bTemp, ulTemp, bOut, &ulOut);
		CHECK_OP(rv);
		SHOW_INFO("Data decrypted: \n");
		ShowData(bOut, ulOut);
		
		START_OP("Compare the original message and decrypted data: ");
		if(0 == memcmp(bIn, bOut, ulOut))
		{
			CHECK_OP(CKR_OK);
		}
		else
		{
			SHOW_INFO("....[FAILED]\n");
		}
	}while(0);
}

void RC4Test::crypt_Update()
{
	const CK_ULONG DATA_LENGTH = 1024;
	CK_BYTE bIn[DATA_LENGTH] = {0}, bTemp[DATA_LENGTH] = {0}, bOut[DATA_LENGTH] = {0};
	CK_ULONG ulIn = 0, ulOut = 0, ulTemp = 0;
	CK_BYTE_PTR bHint = {(CK_BYTE_PTR)"CKM_RC4 "};
	SHOW_INFO("\n*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*\n");
	do{
		ulIn = 257;
		for(register CK_ULONG i0 = 0;i0<ulIn;i0++)
			bIn[i0] = (CK_BYTE)i0;
		
		SHOW_INFO("\n");
		SHOW_INFO("\nRC4: C_EncryptUpdate/C_DecryptUpdate: \n");
		SHOW_INFO(bHint);
		//ecnrypt init:
		CK_MECHANISM ckMechanism = {CKM_RC4, NULL, 0};
		START_OP("Encrypting initialize.")  
			CK_RV rv =  m_gToken->C_EncryptInit(hSession, &ckMechanism, m_hKey); 
		CHECK_OP(rv)
			
			CK_ULONG ulEncrypted = 0;
		START_OP("Encrypt the message.");
		//invoked twice:
		const CK_ULONG ulEnc1stPice = 33;
		//Get size:
		rv =  m_gToken->C_EncryptUpdate(hSession, bIn, ulEnc1stPice, NULL, &ulTemp);
		CheckRV("C_Encrypt[get buffer's size]", rv);
		//alloc memory(omit...)
		//Get encrypted result:
		rv =  m_gToken->C_EncryptUpdate(hSession, bIn, ulEnc1stPice, bTemp, &ulTemp);
		//}}}
		ulEncrypted+=ulTemp;
		ulTemp = 0;
		//encrypt:
		//invoked twice:
		rv =  m_gToken->C_EncryptUpdate(hSession, bIn, ulIn-ulEnc1stPice, NULL, &ulTemp);
		rv =  m_gToken->C_EncryptUpdate(hSession, &(bIn[ulEnc1stPice]), ulIn-ulEnc1stPice, &(bTemp[ulEncrypted]), &ulTemp);
		CheckRV("C_Encrypt", rv);
		CHECK_OP(rv);
		ulEncrypted+=ulTemp;
		ulTemp = 0;
		START_OP("C_EncryptFinal...");
		rv = m_gToken->C_EncryptFinal(hSession, NULL, &ulTemp);
		rv = m_gToken->C_EncryptFinal(hSession, &(bTemp[ulEncrypted]), &ulTemp);
		CHECK_OP(rv);
		ulEncrypted+=ulTemp;
		ulTemp = 0;
		SHOW_INFO("Data encrypted: \n");
		ShowData(bTemp, ulEncrypted);
		
		START_OP("Decrypting initialize.");
		rv =  m_gToken->C_DecryptInit(hSession, &ckMechanism, m_hKey);
		CHECK_OP(rv);
		START_OP("Decrypt the message.");
		//Get buffer's size:
		CK_ULONG ulDecrypt = 0;
		const CK_ULONG ulDec1stPice = 11;
		rv =  m_gToken->C_DecryptUpdate(hSession, bTemp, ulDec1stPice, NULL, &ulOut);
		//alloc memory(omit...)
		rv =  m_gToken->C_DecryptUpdate(hSession, bTemp, ulDec1stPice, bOut, &ulOut);
		ulDecrypt +=ulOut;
		ulOut = 0;
		rv =  m_gToken->C_DecryptUpdate(hSession, bTemp, ulEncrypted-ulDec1stPice, NULL, &ulOut);
		//Get decrypted data:
		rv =  m_gToken->C_DecryptUpdate(hSession, &(bTemp[ulDec1stPice]), ulEncrypted-ulDec1stPice, &(bOut[ulDecrypt]), &ulOut);
		CHECK_OP(rv);
		ulDecrypt +=ulOut;
		ulOut = 0;
		START_OP("C_DecryptFinale...");
		rv = m_gToken->C_DecryptFinal(hSession, NULL, &ulOut);
		rv = m_gToken->C_DecryptFinal(hSession, &(bOut[ulDecrypt]), &ulOut);
		CHECK_OP(rv);
		ulDecrypt +=ulOut;
		
		SHOW_INFO("Data decrypted: \n");
		ShowData(bOut, ulDecrypt);
		
		START_OP("Compare the original message and decrypted data: ");
		if(0 == memcmp(bIn, bOut, ulDecrypt))
		{
			CHECK_OP(CKR_OK);
		}
		else
		{
			SHOW_INFO("....[FAILED]\n");
		}
	}while(0);
}
