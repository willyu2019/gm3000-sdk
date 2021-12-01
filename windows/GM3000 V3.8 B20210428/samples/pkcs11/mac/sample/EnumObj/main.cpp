#include <iostream>
#include "../../include/cryptoki.h"
#include "enumobj.h"

using namespace std;

int main()
{

	cout<<"[]==================================================[]"<<endl;
	cout<<"|              PKCS#11 Demo  		            |"<<endl;
	cout<<"[]==================================================[]"<<endl;

	CK_RV rv;
	rv = C_Initialize(NULL_PTR);
	if(CKR_OK != rv)
	{
		cout<<"Can't Load PKCS#11 Library"<<endl;
		return FALSE;
	}

	EnumObj test;
	rv = test.Connect();
	if(CKR_OK != rv)
	{
		C_Finalize(NULL_PTR);
		return FALSE;
	}

	rv = test.Login();
	if(CKR_OK != rv)
	{
		C_Finalize(NULL_PTR);
		return FALSE;
	}

	bool flag = true;
	while(flag)
	{
		cout<<"Enum Object"<<endl;
		cout<<"1:Data 2:Certificate 3: Public key 4:Private key 5:Secret 0:Exit"<<endl;
		string i;
		cin>>i;
		if((!cin) || (i.length() > 1))
		{
			cout<<"0 -- 5 only!"<<endl;
			cin.clear();
			cin.get();
			continue;
		}
		switch(i[0])
		{
			case '1':
				test.Data();
				break;
			case '2':
				test.Enum();
				break;
			case '3':
				test.Public();
				break;
			case '4':
				test.Private();
				break;
			case '5':
				test.Secret();
				break;
			case '0':
				cout<<"Exit"<<endl;
				flag = false;
				break;
			default:
				cout<<"A fault number!"<<endl;
				break;
		}
	}

	C_Finalize(NULL_PTR);
	return FALSE;
}
