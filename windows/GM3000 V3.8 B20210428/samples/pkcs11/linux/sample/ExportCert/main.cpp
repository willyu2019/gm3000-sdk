#include <iostream>
#include <unistd.h>
#include "../../include/cryptoki.h"
#include "exportcert.h"
#include <stdlib.h>

using namespace std;

int main()
{
	CK_RV rv;
	rv = C_Initialize(NULL_PTR);
	if(CKR_OK != rv)
	{
		cout<<"Can not load PKCS#11 lib"<<endl;
		return FALSE;
	}

	CExportcert test;
	rv = test.Init();
	if(CKR_OK != rv)
	{
		C_Finalize(NULL_PTR);
		return FALSE;
	}
	test.Export();

	C_Finalize(NULL_PTR);
	exit(0);
}
