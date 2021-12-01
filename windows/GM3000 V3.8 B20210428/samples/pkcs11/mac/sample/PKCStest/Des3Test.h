

#ifndef DES3TEST_H
#define DES3TEST_H

#include "../../include/cryptoki.h"

extern CK_SESSION_HANDLE hSession;
extern CK_FUNCTION_LIST_PTR m_gToken;

class Des3Test
{
public:
	Des3Test(void);
	virtual ~Des3Test();
	void Test(void);
private:
	void GenerateKey(void);
	void crypt_Single(void);
	void crypt_Update(void);
	
private:
	CK_OBJECT_HANDLE m_hKey;
};

#endif
