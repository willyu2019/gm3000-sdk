#include <windows.h>
#include <wincrypt.h>
#include <iostream>

#define CSP_NAME  "Longmai mToken GM3000 CSP V1.1"

using namespace std;

int main(int argc, char* argv[])
{
	HCRYPTPROV hCryptProv = NULL;

	cout << "Attempt to acquire context for CSP...";
	if(!CryptAcquireContext(
		&hCryptProv,
		NULL,
		CSP_NAME,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT
	))
	{
		cout << endl << "Can't acquire context, Error code = 0x" << hex << GetLastError() << dec << endl;
		return -1;
	}
	else
		cout << "OK." << endl;

	HCERTSTORE hCertStore = NULL;

	cout << "Attempt to open the certificate storage...";
	hCertStore = CertOpenStore(
		CERT_STORE_PROV_SYSTEM,   // The store provider type.
		0,                        // The encoding type is not needed.
		hCryptProv,               // Use the epassNG HCRYPTPROV.
		CERT_SYSTEM_STORE_CURRENT_USER,
		L"MY"
		);
	if(NULL == hCertStore)
	{
		cout << endl << "Can't open certificates store. Error code = 0x" << hex << GetLastError() << dec << endl;
	}
	else
		cout << "OK." << endl;

	// Enum the certficates. pCertContext must be NULL at the first time.
	PCCERT_CONTEXT pCertContext = NULL;
	DWORD dwTotalCert = 0;

	while(pCertContext = CertEnumCertificatesInStore( hCertStore, pCertContext))
	{
		if(NULL == pCertContext)
			break;

//////////////////////////////////////////////////////////////////////////
// How to get the subject name, see below:
		// First we must get the length of the subject name.
		DWORD dwNameLen = 0;
		dwNameLen = CertGetNameString(
					pCertContext,
					CERT_NAME_SIMPLE_DISPLAY_TYPE,
					0, NULL, NULL, 0
		);
		if(0 == dwNameLen)
		{
			cout << "Can't get the subject name of the certificate." << endl;
			break;
		}

		// Now let's alloc memory for the subject name.
		char* pszTemp = NULL;
		pszTemp = new char[dwNameLen + 1];	// Plus 1 to hold the '\0'.
		if(NULL == pszTemp)
			break;
		ZeroMemory(pszTemp, dwNameLen + 1);

		// Second call to CertGetNameString() to get the subject name.
		if(CertGetNameString(
				pCertContext,
				CERT_NAME_SIMPLE_DISPLAY_TYPE,
				0,
				NULL,
				pszTemp,
				dwNameLen + 1))
		{
			cout << "Oh, the subject name is : " << pszTemp << endl;
		}

		// Free the memory allocated for the subject name.
		delete[] pszTemp;
		pszTemp = NULL;
		++dwTotalCert;

// Ok, we got the subject name.
//////////////////////////////////////////////////////////////////////////
	}

	if(0 == dwTotalCert)
	{
		cout << "No certficate find." << endl; 
	}

	// Close the store.
	if(CertCloseStore(hCertStore, 0))
	{
		cout << "Store closed. All information free." << endl;
	}
	else
	{
		cout << "Store closed. But some information in use." << endl;
	}
    getchar();
	return 0;
}
