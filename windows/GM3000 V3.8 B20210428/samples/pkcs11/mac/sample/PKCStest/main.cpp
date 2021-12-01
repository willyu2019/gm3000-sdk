
#include "Common.h"
#include "RSATest.h"
#include "RC2Test.h"
#include "RC4Test.h"
#include "DesTest.h"
#include "Des3Test.h"
//#include "SSF33Test.h"
#include <unistd.h>
using namespace std;

void * m_hSo;
CK_FUNCTION_LIST_PTR m_gToken;
CK_SLOT_ID_PTR pSlotList;
CK_SESSION_HANDLE hSession;
typedef CK_RV (* C_GETFUNCTIONLISTPROC)(CK_FUNCTION_LIST_PTR_PTR);

const char * p11_so = "../../x64/libgm3000_pkcs11.dylib";

CK_RV BaseAllEnd()
{
	cout <<"\n[ENTER:CBaseAll::BaseAllEnd:Close session and free the PKCS#11's dll]"<<endl;
	CK_RV rv = CKR_OK;
	cout << "Release the session handle..." << endl;
	if(m_gToken)
	{
		rv = m_gToken->C_CloseSession(hSession);
		if(CKR_OK != rv)
		{
			cout << "....C_CloseSession failed!" << endl;
			return rv;
		}
		rv = m_gToken->C_Finalize(0);
		if(CKR_OK != rv)
		{
			cout << "...C_Finalize failed!" << endl;
			return rv;
		}
		m_gToken = NULL_PTR;
	}
	usleep(500);
	if(m_hSo != NULL_PTR)
	{
		dlclose(m_hSo);
		m_hSo = NULL;
	}
	cout << "[EXIT:CBaseAll::BaseAllEnd()]" <<endl;
	return CKR_OK;
}

CK_RV BaseAllStart()
{
	cout <<"\n[ENTER: CBaseAll::BaseAllStart:LoadLibrary PKCS#11's dll and initialize it!]" <<endl;
	CK_RV rv = CKR_OK;
	m_hSo = dlopen(p11_so,RTLD_NOW);
	if(m_hSo == NULL)
	{
		cout << "......dlopen failed!" << endl;
		return CKR_HOST_MEMORY;
	}

	//Get the list of the PKCS#11 function entries.
	C_GETFUNCTIONLISTPROC pC_GetFunctionList = (C_GETFUNCTIONLISTPROC)dlsym(m_hSo,"C_GetFunctionList");
	if(pC_GetFunctionList == NULL)
	{
		cout << "...Err: (pC_GetFunctionList == NULL)!" << endl;
		return CKR_HOST_MEMORY;
	}

	//Get PKCS#11 function list.
	if(CKR_OK != (rv = pC_GetFunctionList(&m_gToken)))
	{
		cout << "...Get PKCS#11 function list failed!" << endl;
		return rv;
	}
	cout << "Initialize the UsbToken PKCS#11 library..." << endl;
	rv = m_gToken->C_Initialize(NULL_PTR);
	if((CKR_OK != rv) && (CKR_CRYPTOKI_ALREADY_INITIALIZED != rv))
	{
		cout << "...C_Initialize failed! Return Error Code=" << rv << endl;
		return rv;
	}

	// Get the number of slots with a token attached.
	cout << "Get the slots information..." << endl;
	unsigned long ulCount;
	rv = m_gToken->C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if(CKR_OK != rv)
	{
		cout << "...C_GetSlotList failed! Return Error Code=" << rv << endl;
		return rv;
	}
	if(ulCount <= 0)
	{
		cout << "...No slot with token attached." << endl;
		return CKR_TOKEN_NOT_PRESENT;
	}

	// Allocate memory buffer for the slot list.
	cout << "Allocate memory for slots..." << endl;
	pSlotList = (CK_SLOT_ID_PTR) malloc(ulCount * sizeof(CK_SLOT_ID));
	if(!pSlotList)
	{
		cout << "...No slots!" << endl;
		return CKR_HOST_MEMORY;
	}
	else
	{
		cout << "Get " << ulCount << " slot attached to USB port!" << endl;
	}
	// Okay, we have the buffer allocated. Let's get the slot list.
	cout << "Get the slots information." << endl;
	rv = m_gToken->C_GetSlotList(TRUE, pSlotList, &ulCount);
	if(CKR_OK != rv)
	{
		cout << "........C_GetSLotList failed! Return Error Code=" << rv
			 << endl;
		CK_RV rv1 = m_gToken->C_Finalize(0);
		if(CKR_OK != rv1)
		{
			cout << "..........C_Finalize failed!" << endl;
			return rv1;
		}
		m_gToken = NULL_PTR;
		usleep(500);
		if(m_hSo)
		{
			dlclose(m_hSo);
			m_hSo = NULL;
		}
		return rv;
	}
	// Open a session to communicate with the token.
	cout << "Open a session to communicate with the UsbToken..." << endl;
	rv = m_gToken->C_OpenSession(pSlotList[0], CKF_RW_SESSION | CKF_SERIAL_SESSION, NULL_PTR, NULL_PTR, &hSession);
	if(CKR_OK != rv)
	{
		cout << "....C_OpenSession failed! Return Error Code=" << rv << endl;
		BaseAllEnd();
		return rv;
	}

	cout << "Input user pin to login first:" << endl;
	string m_ucPin;
	cin >> m_ucPin;
	rv = m_gToken->C_Login(hSession, CKU_USER, (CK_BYTE_PTR)&m_ucPin[0], m_ucPin.length()); 
	if(rv != CKR_OK)
	{
		cout << "...C_Login failed!" << rv << endl;
		BaseAllEnd();
		return rv;
	} 
	cout << "...C_Login OK!" << endl;
	return rv;
}

void ShowTitle(void)
{
	printf("\n");
	printf("[]==================================================[]\n");
	printf(" |             UsbToken PKCS#11 Demo               |\n");				
	printf("[]==================================================[]\n");
}

int main(int argc, char* argv[])
{
	char iTestItem ;
	if(CKR_OK != BaseAllStart())
	{
		return -1;
	}
	for(;;)
	{
	ShowTitle();
	if(argv[1] == NULL)
	{
		cout<<"1: DES, 2: DES3, 3: RC2, 4:RC4, 5:RSA"<<endl;
		cout<< "0: Exit."<<endl;
		cout<< "Please select a testing item: ";
		string s;
		cin >> s;
		if((!cin) || (s.length() > 1))
		{
			cout<<"0 -- 6 only"<<endl;
			cin.clear();
			continue;
		}
		iTestItem = s[0];
	}
	else
	{
		char *temp = NULL;
		char c, *s = NULL ;
		temp = strdup(argv[1]) ;
		argv[1] = NULL;
		s = temp ;
		while(*s)
		{
			c = toupper(*s++) ;
			*(s-1) = c ;
		}
		cout << temp<<endl;
		if(0==strcmp(temp, "-DES"))
			iTestItem = '1';
		else if(0==strcmp(temp, "-DES3"))
			iTestItem = '2';
		else if(0==strcmp(temp, "-RC2"))
			iTestItem = '3';
		else if(0==strcmp(temp, "-RC4"))
			iTestItem = '4';
		else if(0==strcmp(temp, "-RSA"))
			iTestItem = '5';
		else 
			iTestItem = '0';
	}
	switch(iTestItem)
	{
	case '1':
		{
			DesTest des;
			des.Test();
		}
		break;
	case '2':
		{
			Des3Test des3;	
			des3.Test();
		}
		break;
	case '3':
		{
			RC2Test rc2;
			rc2.Test();
		}
		break;
	case '4':
		{
			RC4Test rc4;
			rc4.Test();
		}
		break;
	case '5':
		{
			RSATest rsa;
			rsa.RsaKeyGenerationTest();
		}

		break;
	case '0':
		{
			printf("\n\n Exit!");
		}
		return 0;
	default:
		printf("0-5 only \n");
		break;
	}
	}
	BaseAllEnd();
	return 0;
}
