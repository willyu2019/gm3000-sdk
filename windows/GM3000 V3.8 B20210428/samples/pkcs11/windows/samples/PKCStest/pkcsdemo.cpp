/*
[]======================================================================[]
	pkcsdemo.cpp
	Copyright (C) Feitian Technologies Co., Ltd. All rights reserved.

	Comment :
		Demonstrates the use of the PKCS#11 APIs to interface with the
		UsbToken and perform cryptographic operations.

	Note :
		This sample program requires generation of an RSA public/private
		key pair on the UsbToken. Before running this program, please
		make sure that a properly initialized UsbToken is available and
		plugged into the USB port on your system.

	Warning :
		Please DO NOT remove the UsbToken from your system while running
		this demo program. This program writes keys to the UsbToken. If
		the UsbToken is removed while the library is writing its key data,
		the data may become corrupted. Once data becomes corrupted, you may
		need to reinitialize the token before it can be used by a PKCS#11-
		based application.
[]======================================================================[]
*/

#include "Common.h"
#include "RSATest.h"
#include "RC2Test.h"
#include "RC4Test.h"
#include "DesTest.h"
#include "AESTest.h"
#include "Des3Test.h"


void ShowTitle(void)
{
	printf("[]==================================================[]\n");
	printf(" |             UsbToken PKCS#11 Demo               |\n");				
	printf("[]==================================================[]\n");
}

#define  PKCS11_LIB_NAME "gm3000_pkcs11.dll"

int main(int argc, char* argv[])
{
	char* strTestItem = NULL;
	int iTestItem = 0xFF;
	ShowTitle();
	for(;;)
	{
	if(argv[1] == NULL)
	{
		cout<<"1: DES, 2: DES3, 3: RC2, 4:RC4, 5:RSA, 6: AES"<<endl;
		cout<< "0: Exit."<<endl;
		char szTest[32] = {0};
		cout<< "Please select a testing item: ";
		cin >> szTest;
		iTestItem = atoi(szTest);
	}
	else
	{
		char *temp = NULL;
		temp = _strupr( _strdup(argv[1]));
		cout << temp<<endl;
		if(0==strcmp(temp, "-DES"))
			iTestItem = 1;
		else if(0==strcmp(temp, "-DES3"))
			iTestItem = 2;
		else if(0==strcmp(temp, "-RC2"))
			iTestItem = 3;
		else if(0==strcmp(temp, "-RC4"))
			iTestItem = 4;
		else if(0==strcmp(temp, "-RSA"))
			iTestItem = 5;
		else if(0==strcmp(temp, "-AES"))
			iTestItem = 6;

		else 
			iTestItem = 0;
	}
	switch(iTestItem)
	{
	case 1:
		{
			DesTest test(PKCS11_LIB_NAME);
			test.Test();
		}
		break;
	case 2:
		{
			Des3Test test(PKCS11_LIB_NAME);	
			test.Test();
		}
		break;
	case 3:
		{
			RC2Test test(PKCS11_LIB_NAME);
			test.Test();
		}
		break;
	case 4:
		{
			RC4Test test(PKCS11_LIB_NAME);
			test.Test();
		}
		break;
	case 5:
		{
			RSATest rsa(PKCS11_LIB_NAME);
			rsa.RsaKeyGenerationTest();
		}
		break;
	case 6:
	{
		AESTest test(PKCS11_LIB_NAME);
		test.Test();
	}
	break;
	case 0:
		{
			printf("\n\n Exit!");
		}
		return 0;
	default:
		break;
	}
}
	
	printf("\n\nProgram terminated.\n\nPress any key to exit ....\n");
	return 0;
}



