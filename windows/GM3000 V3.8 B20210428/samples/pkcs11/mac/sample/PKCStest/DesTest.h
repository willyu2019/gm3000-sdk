
#ifndef DESTEST_H
#define DESTEST_H

#include "../../include/cryptoki.h"

extern CK_SESSION_HANDLE hSession;
extern CK_FUNCTION_LIST_PTR m_gToken;

class DesTest
{
public:
	DesTest(void);
	virtual ~DesTest();
	void Test(void);
private:
	void GenerateKey(void);
	void crypt_Single(void);
	void crypt_Update(void);
	
private:
	CK_OBJECT_HANDLE m_hKey;
};

#endif
