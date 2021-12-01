#include "generic.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool EncryptFile(const string &strSource,
		const string &strDestination,
		const string &strPassword); 

//--------------------------------------------------------------------
//   Begin main.

void main(void)
{
	char szSrcFile[1024] = {0};		//sourcefile
	char szTgtFile[1024] = {0};		//encryptfile
	char szPassword[1024] = {0};	//password

	printf("\nEncrypt a file.\n\nEnter the name of the file to be encrypt:\n");
	scanf("%s", szSrcFile);

	printf("Enter the name of the output file:\n");
	scanf("%s", szTgtFile);

	printf("Enter the password to encrypt the file:\n");
	scanf("%s", szPassword);

	if(EncryptFile(szSrcFile, szTgtFile, szPassword))
	{
		printf("Encrypt file successfully.\n");
	}
	else
	{
		printf("Encrypt file failed.\n");
	}
}

//--------------------------------------------------------------------
//   Code for the function EncryptFile called by main.
bool EncryptFile(const string &strSource,
		const string &strDestination,
		const string &strPassword)
{
	//--------------------------------------------------------------------
	//   Declare and initialize local variables.

	FILE *hSource = NULL;
	FILE *hDestination = NULL;

	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hKey = NULL;
	HCRYPTHASH hHash = NULL;

	//--------------------------------------------------------------------
	// Open source file. 
	BeginAction("Open source file for read");
	if(NULL != (hSource = fopen(strSource.c_str(), "rb")))
	{
		ActionSuccess();
	}
	else
	{
		ActionFailed(GetLastError());
		return FALSE;
	} 

	//--------------------------------------------------------------------
	// Open destination file. 
	BeginAction("Open target file for write");
	if(NULL != (hDestination = fopen(strDestination.c_str(), "wb")))
	{
		ActionSuccess();
	}
	else
	{
		ActionFailed(GetLastError());
		return FALSE;
	}

	// Get a CSP handle.
	BeginAction("CryptAcquireContext()");
	if(CryptAcquireContext(&hCryptProv,
			TEST_CONTAINER,
			CSP_NAME,
			PROV_RSA_FULL,
			0))
	{
		ActionSuccess();
	}
	else // Container does not exists, let us create a new one.
	{
		ActionFailed(GetLastError());

		BeginAction("CryptAcquireContext() CRYPT_NEWKEYSET");
		if(CryptAcquireContext(&hCryptProv,
				TEST_CONTAINER,
				CSP_NAME,
				PROV_RSA_FULL,
				CRYPT_NEWKEYSET))
		{
			ActionSuccess();
		}
		else
		{
			ActionFailed(GetLastError());
			return FALSE;
		}
	}

	HCRYPTPROV_Holder holder(hCryptProv);

	BeginAction("CryptCreateHash()");
	if(CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))		
	{
		ActionSuccess();
	}
	else
	{
		ActionFailed(GetLastError());
		return FALSE;
	}  

	BeginAction("CryptHashData()");
	if(CryptHashData(hHash,										
			(BYTE *) strPassword.c_str(),
			strPassword.length(),
			0))
	{
		ActionSuccess();
	}
	else
	{
		ActionFailed(GetLastError());
		return FALSE;
	}

	BeginAction("CryptDeriveKey()");							
	if(CryptDeriveKey(hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey))
	{
		ActionSuccess();
	}
	else
	{
		ActionFailed(GetLastError());
		return FALSE;
	}

	BeginAction("CryptDestroyHash()");							
	if(CryptDestroyHash(hHash))
	{
		hHash = NULL; 
		ActionSuccess();
	}
	else
	{
		ActionFailed(GetLastError());
		return FALSE;
	}

	DWORD dwBlockLen = 0;
	DWORD dwBufferLen = 0;
	DWORD dwCount = 0;

	dwBlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE; 

	//--------------------------------------------------------------------
	// Determine the block size. If a block cipher is used, 
	// it must have room for an extra block. 

	if(ENCRYPT_BLOCK_SIZE > 1)
	{
		dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE;
	}
	else
	{
		dwBufferLen = dwBlockLen;
	} 

	vector<BYTE> pbBuffer;
	pbBuffer.resize(dwBufferLen);

	//--------------------------------------------------------------------
	// In a do loop, encrypt the source file and write to the source file. 
	do
	{
		//--------------------------------------------------------------------
		// Read up to dwBlockLen bytes from the source file. 
		dwCount = fread(&pbBuffer[0], 1, dwBlockLen, hSource); 
		if(ferror(hSource))
		{
			ShowSysError("Read plain text", GetLastError());
			return FALSE;
		}
		//--------------------------------------------------------------------
		// Encrypt
		if(!CryptEncrypt(hKey,
					0,
					feof(hSource),		//file end
					0,
					&pbBuffer[0],		// [in] sourcefile; [out] encryptdata
					&dwCount,			//encryptdatalength
					dwBufferLen))		// encrypt data length
		{
			ShowSysError("CryptEncrypt()", GetLastError());
			return FALSE;
		} 

		//--------------------------------------------------------------------
		// Write data to the destination file. 
		fwrite(&pbBuffer[0], 1, dwCount, hDestination); 
		if(ferror(hDestination))
		{
			ShowSysError("Write cipher text", GetLastError());
			return FALSE;
		}
	}
	while(!feof(hSource)); 
	//--------------------------------------------------------------------
	//  End the do loop when the last block of the source file has been
	//  read, encrypted, and written to the destination file.

	//--------------------------------------------------------------------
	// Close files.
	if(hSource)
	{
		fclose(hSource);
	} 
	if(hDestination)
	{
		fclose(hDestination);
	} 

	if(hKey)
	{
		CryptDestroyKey(hKey);
	} 

	return TRUE;
} // End of Encryptfile
