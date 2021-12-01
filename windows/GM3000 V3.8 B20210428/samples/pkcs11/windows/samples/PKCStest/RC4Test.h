/*
[]=========================================================================[]

	Copyright(C) Feitian Technologies Co., Ltd.
	All rights reserved.

FILE:
	rc4test.h

DESC:
[]=========================================================================[]
*/

#if !defined _RC4_H_
#define _RC4_H_

#include "BaseAll.h"
class RC4Test : public CBaseAll  
{
public:
	void Test(void);
	RC4Test(char* dll_file_path);
	virtual ~RC4Test();

private:
	void GenerateKey(void);
	void crypt_single(void);
	void crypt_Update(void);
private:
	CK_OBJECT_HANDLE m_hKey;
};

#endif // _RC4_H_
