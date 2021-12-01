
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <wincrypt.h>

#define CSP_NAME  "Longmai mToken GM3000 CSP V1.1"
#define ContName  "Gm3000Test"
void MyHandleError(char *s);

void main(void)
{
	//-------------------------------------------------------------
	// Declare and initialize variables.
	
	HCRYPTPROV hProv;
	BYTE *pbBuffer= (BYTE *)"The data that is to be hashed and signed.";//data to be signed
	DWORD dwBufferLen = strlen((char *)pbBuffer)+1;
	HCRYPTHASH hHash;
	HCRYPTKEY hKey;
	BYTE *pbKeyBlob;			//signer's public key
	BYTE *pbSignature;			//signature
	DWORD dwSigLen;
	DWORD dwBlobLen;

	//--------------------------------------------------------------------
	// get provider handle
	printf("The following phase of this program is signature.\n\n");
	if(CryptAcquireContext(
		&hProv, 
		ContName, 
		CSP_NAME,
		PROV_RSA_FULL, 
		0)) 
	{
		printf("CSP context acquired.\n");
	}
	else	//create container if it doesn't exist
	{
		if(CryptAcquireContext(
			&hProv, 
			ContName, 
			CSP_NAME, 
			PROV_RSA_FULL, 
			CRYPT_NEWKEYSET)) 
		{
			printf("A new key container has been created.\n");
		}
		else
		{
			MyHandleError("Error during CryptAcquireContext.");
		}
	}

	//--------------------------------------------------------------------
	// get user key from container
	if(CryptGetUserKey(   
		hProv,    
		AT_SIGNATURE,    
		&hKey)) 
	{
		printf("The signature key has been acquired. \n");
	}
	else
	{
		if(CryptGenKey(
			hProv,			//provider handle
			AT_SIGNATURE,	//create signature key pair
			0,				//use default key length
			&hKey)) 		//returned key handle
		{
			printf("Created a signature key pair.\n");
		}
		else
		{
			MyHandleError("Error occurred creating a signature key.\n"); 
		}
	}

	//--------------------------------------------------------------------
	// export public key to receiver.
	if(CryptExportKey(   
		hKey,    
		NULL,    
		PUBLICKEYBLOB,
		0,    
		NULL, 
		&dwBlobLen))  //public key blob length
	{
		printf("Size of the BLOB for the public key determined. \n");
	}
	else
	{
		MyHandleError("Error computing BLOB length.");
	}

	//--------------------------------------------------------------------
	// malloc a buffer to hold the public key blob which receiver will use
	if(pbKeyBlob = (BYTE*)new BYTE[dwBlobLen])
	{
		printf("Memory has been allocated for the BLOB. \n");
	}
	else
	{
		MyHandleError("Out of memory. \n");
	}

	//--------------------------------------------------------------------
	// export public key blob
	if(CryptExportKey(   
		hKey, 
		NULL,    
		PUBLICKEYBLOB,    
		0,    
		pbKeyBlob,    //this public key blob can be saved in file sent to receiver
		&dwBlobLen))
	{
		printf("Contents have been written to the BLOB. \n");
	}
	else
	{
		delete[] pbKeyBlob;
		MyHandleError("Error during CryptExportKey.");
	}

	//--------------------------------------------------------------------
	// Create hash
	if(CryptCreateHash(
		hProv, 
		CALG_MD5, 
		0, 
		0, 
		&hHash)) 
	{
		printf("Hash object created. \n");
	}
	else
	{
		delete[] pbKeyBlob;
		MyHandleError("Error during CryptCreateHash.");
	}

	//--------------------------------------------------------------------
	// digest signed data
	if(CryptHashData(
		hHash, 
		pbBuffer, 
		dwBufferLen, 
		0)) 
	{
		printf("The data buffer has been hashed.\n");
	}
	else
	{
		delete[] pbKeyBlob;
		MyHandleError("Error during CryptHashData.");
	}

	//--------------------------------------------------------------------
	// sign digest with signature key

	dwSigLen= 0;
	if(CryptSignHash(
		hHash, 
		AT_SIGNATURE, 
		NULL, 
		0, 
		NULL, 
		&dwSigLen)) //get signature length
	{
		printf("Signature length %d found.\n",dwSigLen);
	}
	else
	{
		delete[] pbKeyBlob;
		MyHandleError("Error during CryptSignHash.");
	}

	//--------------------------------------------------------------------
	// malloc buffer to hold signature
	if(pbSignature = (BYTE *)new BYTE[dwSigLen])
	{
		printf("Memory allocated for the signature.\n");
	}
	else
	{
		delete[] pbKeyBlob;
		MyHandleError("Out of memory.");
	}

	//--------------------------------------------------------------------
	// get signature
	if(CryptSignHash(
		hHash, 
		AT_SIGNATURE, 
		NULL, 
		0, 
		pbSignature, //signature, will be sent to receiver with plaintext message
		&dwSigLen)) 
	{
		printf("pbSignature is the hash signature.\n");
	}
	else
	{
		delete[] pbKeyBlob;
		delete[] pbSignature;
		MyHandleError("Error during CryptSignHash.");
	}

	//--------------------------------------------------------------------
	if(hHash) 
		CryptDestroyHash(hHash);
	
	printf("The hash object has been destroyed.\n");
	printf("The signing phase of this program is completed.\n\n");
	
/********************************************************************************
	The following code fragment is for receiver, we put it here in.
*********************************************************************************/
	
	printf("The following phase of this program is verify signature.\n\n");
	
	// import public key of signer
	HCRYPTKEY hPubKey;
	if(CryptImportKey(
		hProv,
		pbKeyBlob,
		dwBlobLen,
		0,
		0,
		&hPubKey))
	{
		printf("The key has been imported.\n");
	}
	else
	{
		delete[] pbKeyBlob;
		delete[] pbSignature;
		MyHandleError("Public key import failed.");
	}

	delete[] pbKeyBlob;

	if(CryptCreateHash(
		hProv, 
		CALG_MD5, 
		0, 
		0, 
		&hHash)) 
	{
		printf("The hash object has been recreated. \n");
	}
	else
	{
		delete[] pbSignature;
		MyHandleError("Error during CryptCreateHash.");
	}

	if(CryptHashData(
		hHash, 
		pbBuffer, 
		dwBufferLen, 
		0)) 
	{
		printf("The new hash has been created.\n");
	}
	else
	{
		delete[] pbSignature;
		MyHandleError("Error during CryptHashData.");
	}

	if(CryptVerifySignature(
		hHash, 
		pbSignature,	//signature
		dwSigLen, 
		hPubKey,		//signer's public key
		NULL, 
		0)) 
	{
		printf("The signature has been verified.\n");
	}
	else
	{
		DWORD LE  = GetLastError();
		delete[] pbSignature;
		pbSignature = NULL;
		printf("Signature not validated!ErrorCode:%lu\n",LE);
	}

	//--------------------------------------------------------------------
	// Free memory to be used to store signature.
	if(pbSignature)
		delete[] pbSignature;

	//--------------------------------------------------------------------
	// Destroy the hash object.
	if(hHash) 
		CryptDestroyHash(hHash);
	
	//--------------------------------------------------------------------
	// Release the provider handle.
	if(hProv) 
		CryptReleaseContext(hProv, 0);

	printf("\n\nPress any key to exit...\n");
	_getch();
} //  End of main

//--------------------------------------------------------------------
//  This example uses the function MyHandleError, a simple error
//  handling function, to print an error message to the standard error 
//  (stderr) file and exit the program. 
//  For most applications, replace this function with one 
//  that does more extensive error reporting.

void MyHandleError(char *s)
{
    fprintf(stderr,"An error occurred in running the program. \n");
    fprintf(stderr,"%s\n",s);
    fprintf(stderr, "Error number %x.\n", GetLastError());
    fprintf(stderr, "Program terminating. \n");
    exit(1);
} // End of MyHandleError

