/*
[]=========================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	rsatest.cpp

DESC:
[]=========================================================================[]
*/
#ifndef _RSA_H_
#define _RSA_H_
#include "BaseAll.h"
class RSATest : public CBaseAll 
{
public:
	RSATest(char* dll_file_path);
	virtual ~RSATest();
	void RsaKeyGenerationTest(void);
	
};

#endif 
