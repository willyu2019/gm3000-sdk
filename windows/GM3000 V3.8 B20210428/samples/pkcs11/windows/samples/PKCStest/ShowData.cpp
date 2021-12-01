/*
[]=========================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	showdata.cpp

DESC:
	Implement of ShowData method.
[]=========================================================================[]
*/

#include "common.h"

void ShowData(unsigned char *pData, unsigned long ulDataLen)
{
	cout <<" Length of data to be showed is: "<< dec<<ulDataLen<<endl;
	if(pData == NULL)
	{
		cout << "...Data to be showed is NULL!" <<endl;
		return ;
	}
	else
	{
		for(register unsigned long i=0;i<ulDataLen;i++)
		{
			if(i%16==0)
			{
				cout << endl;
			}
			cout << cout.width(3) << hex << (int)pData[i] << " ";
		}//for
		cout << endl;
	}//else
}