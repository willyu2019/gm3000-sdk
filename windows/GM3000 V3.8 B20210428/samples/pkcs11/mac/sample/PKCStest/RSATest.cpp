

#include "RSATest.h"
#include "Common.h"
RSATest::RSATest()
{
}

RSATest::~RSATest()
{
}

void RSATest::RsaKeyGenerationTest(void)
{
	
	SHOW_INFO("\n\nThe next demo will generate a RSA key pair on UsbToken.")
	CK_RV rv = CKR_OK;
	CK_BBOOL bTrue = TRUE;
	CK_ULONG ulModulusBits = MODULUS_BIT_LENGTH;
	CK_BYTE subject[] = "Sample RSA Key Pair";
	CK_ULONG keyType = CKK_RSA;
	
	CK_OBJECT_HANDLE hPubKey = 0;
	CK_OBJECT_CLASS pubClass = CKO_PUBLIC_KEY;
	CK_ATTRIBUTE pubTemplate[] =
	{ 
		{CKA_CLASS,			&pubClass,		sizeof(pubClass)},
		{CKA_KEY_TYPE,		&keyType,		sizeof(keyType)},
		{CKA_SUBJECT,		subject,		sizeof(subject)},
		{CKA_MODULUS_BITS,	&ulModulusBits, sizeof(ulModulusBits)},
		{CKA_ENCRYPT,		&bTrue,			sizeof(bTrue)},
		{CKA_TOKEN,			&bTrue,			sizeof(bTrue)},
		{CKA_WRAP,			&bTrue,			sizeof(bTrue)},
	};	
	CK_OBJECT_HANDLE hPriKey		= 0;
	CK_OBJECT_CLASS priClass	= CKO_PRIVATE_KEY;
	CK_ATTRIBUTE priTemplate[] = {
		{CKA_CLASS,			&priClass,	sizeof(priClass)},
		{CKA_KEY_TYPE,		&keyType,	sizeof(keyType)},
		{CKA_SUBJECT,		subject,	sizeof(subject)},
		{CKA_DECRYPT,		&bTrue,		sizeof(bTrue)},
		{CKA_PRIVATE,		&bTrue,		sizeof(bTrue)},
		{CKA_SENSITIVE,		&bTrue,		sizeof(bTrue)},
		{CKA_TOKEN,			&bTrue,		sizeof(bTrue)},
		{CKA_EXTRACTABLE,	&bTrue,		sizeof(bTrue)},
		{CKA_UNWRAP,		&bTrue,		sizeof(bTrue)},
	};
    CK_MECHANISM keyGenMechanism = {CKM_RSA_PKCS_KEY_PAIR_GEN, NULL_PTR, 0};
	CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
	CK_BYTE pbMsg[] = "UsbToken RunRsaKeyGenerationTest...";
	CK_ULONG ulMsgLen = strlen((const char *)pbMsg);
	CK_BYTE bSignatureBuffer[MODULUS_BIT_LENGTH] = {0};
	CK_ULONG ulSignatureLen = 0;
	CK_BYTE_PTR pbCipherBuffer = NULL;
	CK_ULONG ulCipherLen = 0;
	CK_BYTE_PTR pbRestoredMsg = NULL;
	CK_ULONG ulRestoredMsgLen = 0;
	
	do {
		START_OP("Generating public/private key pair...");
		rv =  m_gToken->C_GenerateKeyPair(hSession, &keyGenMechanism,
								   pubTemplate, countof(pubTemplate),
								   priTemplate, countof(priTemplate),
								   &hPubKey, &hPriKey);
		CHECK_OP(rv)
			
    	// Try Sign and Verify operations with the key pair.
		// Sign a message. 
		SHOW_INFO("\nThe message to be signed is \"")
		ShowData(pbMsg, ulMsgLen);
		
		ulSignatureLen	= sizeof(bSignatureBuffer);
		START_OP("Signing initialize.");
		rv =  m_gToken->C_SignInit(hSession, &ckMechanism, hPriKey);
		CHECK_OP(rv);
		
		START_OP("Sign the message.")
		rv =  m_gToken->C_Sign(hSession, 
							pbMsg,
							ulMsgLen, 
							bSignatureBuffer, &ulSignatureLen);
		CHECK_OP(rv)
		SHOW_INFO("\nSignature is:\n");	
		ShowData(bSignatureBuffer, ulSignatureLen);

		// Verify the previously signed message.
		START_OP("Verifying initialize.")
		rv =  m_gToken->C_VerifyInit(hSession, &ckMechanism, hPubKey);
		CHECK_OP(rv)
			
		START_OP("Verify the message.")
		rv =  m_gToken->C_Verify(hSession, 
									pbMsg, ulMsgLen, 
									bSignatureBuffer, ulSignatureLen);
		CHECK_OP(rv)
			
		// Encrypt a message. 
		SHOW_INFO("\nThe message to be Encrypt is: ")
		ShowData(pbMsg, ulMsgLen);
		
		START_OP("Encrypting initialize.")
			rv =  m_gToken->C_EncryptInit(hSession,
											&ckMechanism,
											hPubKey);
		CHECK_OP(rv)
			
		START_OP("Obtain the size of the encrypted message....")
		rv =  m_gToken->C_Encrypt(hSession, pbMsg, ulMsgLen, NULL_PTR, &ulCipherLen);
		CHECK_OP(rv)
			
		START_OP("Allocate buffer for the encrypted message.")
		pbCipherBuffer = (CK_BYTE_PTR)malloc(ulCipherLen);
		if (! pbCipherBuffer)
			CHECK_OP(CKR_HOST_MEMORY)
		else
			CHECK_OP(CKR_OK);
		
		START_OP("Encrypt the message....");
		memset(pbCipherBuffer, 0, ulCipherLen);
		rv =  m_gToken->C_Encrypt(hSession, pbMsg, ulMsgLen, 
											pbCipherBuffer, &ulCipherLen);
		CHECK_OP(rv);
		SHOW_INFO("Data encrypted: ");
		ShowData(pbCipherBuffer, ulCipherLen);
		
		START_OP("Decrypting initialize....")
		rv =  m_gToken->C_DecryptInit(hSession,
										&ckMechanism,
										hPriKey);
		CHECK_OP(rv)
			
			START_OP("Obtain the size of the decrypted message....")
			rv =  m_gToken->C_Decrypt(hSession, pbCipherBuffer, 
										ulCipherLen, NULL_PTR, &ulRestoredMsgLen);
		CHECK_OP(rv)
		START_OP("Allocate buffer for the decrypted message.")
		pbRestoredMsg = (CK_BYTE_PTR)malloc(ulRestoredMsgLen + 1);
		if (! pbRestoredMsg)
			CHECK_OP(CKR_HOST_MEMORY)
			else
			CHECK_OP(CKR_OK);
		memset(pbRestoredMsg, 0, ulRestoredMsgLen + 1);
		
		START_OP("Decrypt the message.")
		rv =  m_gToken->C_Decrypt(hSession, pbCipherBuffer, ulCipherLen, 
									pbRestoredMsg, &ulRestoredMsgLen);
		CHECK_OP(rv)
			
		// Decrypt a message. 
		SHOW_INFO("\nThe message decrypted is \n")
		ShowData(pbRestoredMsg, ulRestoredMsgLen);
		
		START_OP("Verify the message.");
		if(0 == memcmp(pbMsg, pbRestoredMsg, ulRestoredMsgLen))
			CHECK_OP(CKR_OK)
		else
			SHOW_INFO("....[FAILED]\n")
			
		// Remove the RSA key pair from the UsbToken.
		START_OP("Proceed to remove the RSA key pair from the token.");
		rv =  m_gToken->C_DestroyObject(hSession, hPubKey);
		CHECK_OP(rv);
		
		START_OP("Remove private key obeject.");
		rv =  m_gToken->C_DestroyObject(hSession, hPriKey);
		CHECK_OP(rv);
		
	}while (0);
		
		// Release the memory.
		if(pbCipherBuffer)
		{
			free(pbCipherBuffer);
			pbCipherBuffer = NULL;
		}
		if(pbRestoredMsg)
		{
			free(pbRestoredMsg);
			pbCipherBuffer = NULL;
		}
}
