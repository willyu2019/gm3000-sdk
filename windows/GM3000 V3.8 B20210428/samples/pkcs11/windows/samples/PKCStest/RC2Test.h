/*
[]=========================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	rc2test.h

DESC:
[]=========================================================================[]
*/
#ifndef  _RC2_H_
#define _RC2_H_

#include "BaseAll.h"
class RC2Test : public CBaseAll  
{
public:
	void Test(void);
	RC2Test(char* dll_file_path);
	virtual ~RC2Test();

private:
	void GenerateKey(void);
	void crypt_Single(void);
	void crypt_Update(void);
private:
	CK_OBJECT_HANDLE m_hKey;
};

#endif // _RC2_H_
