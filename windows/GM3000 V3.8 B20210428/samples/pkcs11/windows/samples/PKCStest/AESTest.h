// AESTest.h: interface for the AESTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AESTEST_H__B4299709_9AC7_4220_9EBD_F9CE227CEA49__INCLUDED_)
#define AFX_AESTEST_H__B4299709_9AC7_4220_9EBD_F9CE227CEA49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BaseAll.h"
class AESTest   : public CBaseAll  
{
public:
	AESTest(char* dll_file_path);
	virtual ~AESTest();
	void Test(void);
private:
	void GenerateKey(void);
	void crypt_Single(void);
	void crypt_Update(void);
	
private:
	CK_OBJECT_HANDLE m_hKey;

};

#endif // !defined(AFX_AESTEST_H__B4299709_9AC7_4220_9EBD_F9CE227CEA49__INCLUDED_)
