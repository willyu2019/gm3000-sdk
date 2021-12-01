
#ifndef  RC2_H
#define RC2_H

#include "../../include/cryptoki.h"

extern CK_SESSION_HANDLE hSession;
extern CK_FUNCTION_LIST_PTR m_gToken;

class RC2Test
{
public:
	void Test(void);
	RC2Test(void);
	virtual ~RC2Test();

private:
	void GenerateKey(void);
	void crypt_Single(void);
	void crypt_Update(void);
private:
	CK_OBJECT_HANDLE m_hKey;
};

#endif
