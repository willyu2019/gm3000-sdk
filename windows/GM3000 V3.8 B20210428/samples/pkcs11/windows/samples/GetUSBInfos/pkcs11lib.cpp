#include "pkcs11lib.h"
#include <windows.h>



HMODULE hPkcs11Lib = 0;

PKCS11Wraper PKCS11;

long Pkcs11Initialize(char * libName) 
{
	CK_RV result;
	
	if (hPkcs11Lib != NULL)
		return 0;

	hPkcs11Lib = LoadLibrary(libName);
	if(hPkcs11Lib == 0)
		return 1;
	
	CK_FUNCTION_LIST_PTR functions;

	PKCS11.C_GetFunctionList = (CK_C_GetFunctionList)GetProcAddress(hPkcs11Lib,"C_GetFunctionList");
	result = PKCS11.C_GetFunctionList(&functions);
	if (result != CKR_OK)
		return result;

	PKCS11.C_Initialize = functions->C_Initialize;
	PKCS11.C_Finalize = functions->C_Finalize;
	PKCS11.C_GetInfo = functions->C_GetInfo;
	PKCS11.C_GetFunctionList = functions->C_GetFunctionList;
	PKCS11.C_GetSlotList = functions->C_GetSlotList;
	PKCS11.C_GetSlotInfo = functions->C_GetSlotInfo;
	PKCS11.C_GetTokenInfo = functions->C_GetTokenInfo;
	PKCS11.C_GetMechanismList = functions->C_GetMechanismList;
	PKCS11.C_GetMechanismInfo = functions->C_GetMechanismInfo;
	PKCS11.C_InitToken = functions->C_InitToken;
	PKCS11.C_InitPIN = functions->C_InitPIN;
	PKCS11.C_SetPIN = functions->C_SetPIN;
	PKCS11.C_OpenSession = functions->C_OpenSession;
	PKCS11.C_CloseSession = functions->C_CloseSession;
	PKCS11.C_CloseAllSessions = functions->C_CloseAllSessions;
	PKCS11.C_GetSessionInfo = functions->C_GetSessionInfo;
	PKCS11.C_GetOperationState = functions->C_GetOperationState;
	PKCS11.C_SetOperationState = functions->C_SetOperationState;
	PKCS11.C_Login = functions->C_Login;
	PKCS11.C_Logout = functions->C_Logout;
	PKCS11.C_CreateObject = functions->C_CreateObject;
	PKCS11.C_CopyObject = functions->C_CopyObject;
	PKCS11.C_DestroyObject = functions->C_DestroyObject;
	PKCS11.C_GetObjectSize = functions->C_GetObjectSize;
	PKCS11.C_GetAttributeValue = functions->C_GetAttributeValue;
	PKCS11.C_SetAttributeValue = functions->C_SetAttributeValue;
	PKCS11.C_FindObjectsInit = functions->C_FindObjectsInit;
	PKCS11.C_FindObjects = functions->C_FindObjects;
	PKCS11.C_FindObjectsFinal = functions->C_FindObjectsFinal;
	PKCS11.C_EncryptInit = functions->C_EncryptInit;
	PKCS11.C_Encrypt = functions->C_Encrypt;
	PKCS11.C_EncryptUpdate = functions->C_EncryptUpdate;
	PKCS11.C_EncryptFinal = functions->C_EncryptFinal;
	PKCS11.C_DecryptInit = functions->C_DecryptInit;
	PKCS11.C_Decrypt = functions->C_Decrypt;
	PKCS11.C_DecryptUpdate = functions->C_DecryptUpdate;
	PKCS11.C_DecryptFinal = functions->C_DecryptFinal;
	PKCS11.C_DigestInit = functions->C_DigestInit;
	PKCS11.C_Digest = functions->C_Digest;
	PKCS11.C_DigestUpdate = functions->C_DigestUpdate;
	PKCS11.C_DigestKey = functions->C_DigestKey;
	PKCS11.C_DigestFinal = functions->C_DigestFinal;
	PKCS11.C_SignInit = functions->C_SignInit;
	PKCS11.C_Sign = functions->C_Sign;
	PKCS11.C_SignUpdate = functions->C_SignUpdate;
	PKCS11.C_SignFinal = functions->C_SignFinal;
	PKCS11.C_SignRecoverInit = functions->C_SignRecoverInit;
	PKCS11.C_SignRecover = functions->C_SignRecover;
	PKCS11.C_VerifyInit = functions->C_VerifyInit;
	PKCS11.C_Verify = functions->C_Verify;
	PKCS11.C_VerifyUpdate = functions->C_VerifyUpdate;
	PKCS11.C_VerifyFinal = functions->C_VerifyFinal;
	PKCS11.C_VerifyRecoverInit = functions->C_VerifyRecoverInit;
	PKCS11.C_VerifyRecover = functions->C_VerifyRecover;
	PKCS11.C_DigestEncryptUpdate = functions->C_DigestEncryptUpdate;
	PKCS11.C_DecryptDigestUpdate = functions->C_DecryptDigestUpdate;
	PKCS11.C_SignEncryptUpdate = functions->C_SignEncryptUpdate;
	PKCS11.C_DecryptVerifyUpdate = functions->C_DecryptVerifyUpdate;
	PKCS11.C_GenerateKey = functions->C_GenerateKey;
	PKCS11.C_GenerateKeyPair = functions->C_GenerateKeyPair;
	PKCS11.C_WrapKey = functions->C_WrapKey;
	PKCS11.C_UnwrapKey = functions->C_UnwrapKey;
	PKCS11.C_DeriveKey = functions->C_DeriveKey;
	PKCS11.C_SeedRandom = functions->C_SeedRandom;
	PKCS11.C_GenerateRandom = functions->C_GenerateRandom;
	PKCS11.C_GetFunctionStatus = functions->C_GetFunctionStatus;
	PKCS11.C_CancelFunction = functions->C_CancelFunction;
	PKCS11.C_WaitForSlotEvent = functions->C_WaitForSlotEvent;
	
	result = PKCS11.C_Initialize(NULL_PTR);
	if (result != CKR_OK)
	{
		FreeLibrary(hPkcs11Lib);
		hPkcs11Lib = NULL;
	}

	return result;
}

PKCS11Wraper * GetPkcs11Wrapper()
{
	return &PKCS11;
}