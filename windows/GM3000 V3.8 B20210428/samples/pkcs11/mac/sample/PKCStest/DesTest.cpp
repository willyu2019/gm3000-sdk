
#include "DesTest.h"
#include "Common.h"
DesTest::DesTest()
{
	m_hKey = 0;
}

DesTest::~DesTest()
{
}

void DesTest::Test()
{
	GenerateKey();
	if(m_hKey == 0)
	{
		return ;
	}
	crypt_Single();
	crypt_Update();
}
void DesTest::GenerateKey()
{
	do{
		SHOW_INFO("Generate Des key to test...");
		CK_OBJECT_CLASS oClass = CKO_SECRET_KEY;
		CK_KEY_TYPE keyType = CKK_DES; 
		CK_BBOOL bTrue = true;
		CK_BBOOL bFalse = false;
		CK_ULONG ulLen = 8;
		CK_MECHANISM mechanism = {CKM_DES_KEY_GEN, NULL_PTR, 0};
		CK_ATTRIBUTE Destem[] = {
			{CKA_CLASS, &oClass, sizeof(CK_OBJECT_CLASS)},
			{CKA_KEY_TYPE, &keyType, sizeof(CK_KEY_TYPE)},
			{CKA_TOKEN, &bFalse, sizeof(CK_BBOOL)},
			{CKA_PRIVATE, &bTrue, sizeof(CK_BBOOL)},
			{CKA_ENCRYPT, &bTrue, sizeof(CK_BBOOL)},
			{CKA_DECRYPT, &bTrue, sizeof(CK_BBOOL)},
			{CKA_VALUE_LEN, &ulLen, sizeof(CK_ULONG)}, 
		};
		CK_ULONG ulCount = 7;
		//generate key:
		START_OP("generate DES key...")
			CK_RV rv =  m_gToken->C_GenerateKey(hSession, &mechanism, Destem, ulCount, &m_hKey); 
		CHECK_OP(rv)
	}while(0);
}
void DesTest::crypt_Single()
{

	const CK_ULONG DATA_LENGTH = 1024*3;
	CK_BYTE bIn[DATA_LENGTH] = {0}, bTemp[DATA_LENGTH] = {0}, bOut[DATA_LENGTH] = {0};
	CK_ULONG ulIn = 0, ulOut = 0, ulTemp = 0;
	CK_ULONG Mechanism[3] = {CKM_DES_CBC, CKM_DES_ECB, CKM_DES_CBC_PAD};
	CK_BYTE_PTR bHint[3] = {(CK_BYTE_PTR)"CKM_DES_CBC: ",\
							(CK_BYTE_PTR)"CKM_DES_ECB: ",
							(CK_BYTE_PTR)"CKM_DES_CBC_PAD: "};
	SHOW_INFO("\nDES: C_Encrypt/C_Decrypt: \n");
	do{
		for(int i=0;i<3;i++)
		{
			ulIn = 1000;
			if(i==2)
				ulIn = 1000;
			for(register CK_ULONG i0 = 0;i0<ulIn;i0++)
				bIn[i0] = (CK_BYTE)i0;
			
			
			SHOW_INFO("\n*	*	*	*	*	*	*	*	*	*	*	\n");
			SHOW_INFO(bHint[i]);
			//ecnrypt init:
			CK_BYTE iv[8] = {'*','2','1','0','4','z','y','b'};
			CK_MECHANISM ckMechanism = {Mechanism[i], iv, 8};
			START_OP("Encrypting initialize.")  
			CK_RV rv =  m_gToken->C_EncryptInit(hSession, &ckMechanism, m_hKey); 
			CHECK_OP(rv)

			START_OP("Encrypt the message.")
			//Get the encrypted buffer's size:
			//{{{Here, I do not invoke "C_Encrypt" twice for I had declared bTemp with a size=1024.
			//If you do not declare the result's buffer previous,
			//you should invoke twice to get the buffer's size, such as:[Decrypt is similar]
			rv =  m_gToken->C_Encrypt(hSession, bIn, ulIn, NULL, &ulTemp);
			//}}}
			CHECK_RV("C_Encrypt[get buffer's size]", rv);
			//encrypt:
			rv =  m_gToken->C_Encrypt(hSession, bIn, ulIn, bTemp, &ulTemp);
			CHECK_RV("C_Encrypt", rv);
			CHECK_OP(rv);
			SHOW_INFO("Data encrypted: \n");
			ShowData(bTemp, ulTemp);

			START_OP("Decrypting initialize.");
			rv =  m_gToken->C_DecryptInit(hSession, &ckMechanism, m_hKey);
			CHECK_OP(rv);
			START_OP("Decrypt the message.");
			//Get buffer's size:
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
		}
	}while(0);

}

void DesTest::crypt_Update()
{

	const CK_ULONG DATA_LENGTH = 1024*3;
	CK_BYTE bIn[DATA_LENGTH] = {0}, bTemp[DATA_LENGTH] = {0}, bOut[DATA_LENGTH] = {0};
	CK_ULONG ulIn = 0, ulOut = 0, ulTemp = 0;
	CK_ULONG Mechanism[3] = {CKM_DES_CBC, CKM_DES_ECB, CKM_DES_CBC_PAD};
	CK_BYTE_PTR bHint[3] = {(CK_BYTE_PTR)"CKM_DES_CBC: ",\
									(CK_BYTE_PTR)"CKM_DES_ECB: ",\
									(CK_BYTE_PTR)"CKM_DES_CBC_PAD: "};
	SHOW_INFO("\n*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*\n");
	do{
		for(int i=0;i<3;i++)
		{
			ulIn = 1000;
			if(i == 2)
			{//CKM_RC2_CBC_PAD
				ulIn = 1000;
			}
			for(register CK_ULONG i0 = 0;i0<ulIn;i0++)
				bIn[i0] = (CK_BYTE)i0;

			SHOW_INFO("\n");
			SHOW_INFO("\nDES: C_EncryptUpdate/C_DecryptUpdate: \n");
			SHOW_INFO(bHint[i]);
			//ecnrypt init:
			CK_BYTE iv[8] = {'*','2','1','0','4','z','y','b'};
			CK_MECHANISM ckMechanism = {Mechanism[i], iv, sizeof(iv)};
			START_OP("Encrypting initialize.")  
			CK_RV rv =  m_gToken->C_EncryptInit(hSession, &ckMechanism, m_hKey); 
			CHECK_OP(rv)
			
			CK_ULONG ulEncrypted = 0;
			START_OP("Encrypt the message.");
			//invoked twice:
			const CK_ULONG ulEnc1stPice = 33;
			rv =  m_gToken->C_EncryptUpdate(hSession, bIn, ulEnc1stPice, NULL, &ulTemp);//get buffer's size.
			rv =  m_gToken->C_EncryptUpdate(hSession, bIn, ulEnc1stPice, bTemp, &ulTemp);
			//}}}
			CHECK_RV("C_Encrypt[get buffer's size]", rv);
			ulEncrypted+=ulTemp;
			ulTemp = 0;
			//encrypt:
			//invoked twice:
			START_OP("Encrypting initialize.")  
			rv =  m_gToken->C_EncryptUpdate(hSession,  &(bIn[ulEnc1stPice]), ulIn-ulEnc1stPice, NULL, &ulTemp);//get buffer's size.
			rv =  m_gToken->C_EncryptUpdate(hSession, &(bIn[ulEnc1stPice]), ulIn-ulEnc1stPice, &(bTemp[ulEncrypted]), &ulTemp);
			CHECK_RV("C_Encrypt", rv);
			ulEncrypted+=ulTemp;
			ulTemp = 0;
			START_OP("C_EncryptFinal...");
			rv = m_gToken->C_EncryptFinal(hSession, NULL, &ulTemp);//get buffer's size
			CHECK_OP(rv);
			rv = m_gToken->C_EncryptFinal(hSession, &(bTemp[ulEncrypted]), &ulTemp);
			CHECK_OP(rv);
			ulEncrypted+=ulTemp;
			ulTemp = 0;
			SHOW_INFO("\nData encrypted: \n");
			ShowData(bTemp, ulEncrypted);
			 
			START_OP("Decrypting initialize.");
			rv =  m_gToken->C_DecryptInit(hSession, &ckMechanism, m_hKey);
			CHECK_OP(rv);
			START_OP("Decrypt the message.");
			//Get buffer's size:
			CK_ULONG ulDecrypt = 0;
			const CK_ULONG ulDec1stPice = 11;
			rv =  m_gToken->C_DecryptUpdate(hSession, bTemp, ulDec1stPice, NULL, &ulOut);
			rv =  m_gToken->C_DecryptUpdate(hSession, bTemp, ulDec1stPice, bOut, &ulOut);
			ulDecrypt +=ulOut;
			ulOut = 0;
			//Get decrypted data:
			rv =  m_gToken->C_DecryptUpdate(hSession, &(bTemp[ulDec1stPice]), ulEncrypted-ulDec1stPice, NULL, &ulOut);
			rv =  m_gToken->C_DecryptUpdate(hSession, &(bTemp[ulDec1stPice]), ulEncrypted-ulDec1stPice, &(bOut[ulDecrypt]), &ulOut);
			CHECK_OP(rv);
			ulDecrypt +=ulOut;
			ulOut = 0;
			START_OP("C_DecryptFinale...");
			rv = m_gToken->C_DecryptFinal(hSession, &(bOut[ulDecrypt]), &ulOut);
			CHECK_OP(rv);
			ulDecrypt +=ulOut;
			
			SHOW_INFO("\nData decrypted: \n");
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
		}
	}while(0);
}
