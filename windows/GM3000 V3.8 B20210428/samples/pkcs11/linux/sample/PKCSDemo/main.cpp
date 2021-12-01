#include <stdio.h>
#include <iostream>
#include "../../include/cryptoki.h"
#include "PKCSDemo.h"

using namespace std;

int main(void )
{
	CK_RV rv;
	
	cout<<"[]==================================================[]"<<endl;
	cout<<" |             	PKCS#11 Demo 		           |"<<endl;
	cout<<"[]==================================================[]"<<endl;

	rv = C_Initialize(NULL_PTR);
	if(CKR_OK != rv)
	{
		cout<<"Can not load PKCS#11 lib\n"<<endl;
		C_Finalize(NULL_PTR);
		return FALSE;
	}
	CPKCSDemo demo;
	rv = demo.Connect();	
	if(CKR_OK != rv)
	{
		C_Finalize(NULL_PTR);
		return FALSE;
	}
	rv = demo.Login();
	if(CKR_OK != rv)
	{
		C_Finalize(NULL_PTR);
		return FALSE;
	}
	cout<<"Now,generating key pair... ..."<<endl;
	rv = demo.Keypairgen();
	if(CKR_OK != rv)
	{
		C_Finalize(NULL_PTR);
		return FALSE;
	}
	bool sign = true;
	bool canSing = false;
	bool canDecrypt = false;
	while(sign)
	{
		cout<<"1: Sign,";
		if(canSing) cout<<"2: Verify,";
		cout<<"3: Encrypt,";
		if(canDecrypt) cout<<"4: Decrypt,";
		cout<<"5: Exit"<<endl;
		string s;
		cin>>s;
		if((!cin) || (s.length() > 1)) 
		{
			cout<<"1-5 only"<<endl;
			cin.clear();
			cin.get();
			continue;
		}
		switch(s[0])
		{
			case '1':
				demo.Sign();
				canSing = true;
				break;
			case '2':
				if(canSing)
				{
					demo.Verify();
				}else
				{
					cout<<"need \"Sign\" first!"<<endl;
				}
				break;
			case '3':
				demo.Encrypt();
				canDecrypt = true;
				break;
			case '4':
				if(canDecrypt)
				{
					demo.Decrypt();
				}else 
				{
					cout<<"need \"Encrypt\" first!" <<endl;
				}
				break;
			case '5':
				cout<<"Exit"<<endl;
				demo.Destroy();
				sign = false;
				break;
			default:
				cout<<"1-5 only"<<endl;
				break;
		}
	}
	C_Finalize(NULL_PTR);

	return FALSE;
}
