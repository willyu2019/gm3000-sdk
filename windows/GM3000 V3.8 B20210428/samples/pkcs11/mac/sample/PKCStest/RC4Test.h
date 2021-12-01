
#ifndef RC4_H
#define RC4_H
#include "../../include/cryptoki.h"

extern CK_SESSION_HANDLE hSession;
extern CK_FUNCTION_LIST_PTR m_gToken;

class RC4Test
{
public:
	void Test(void);
	RC4Test(void);
	virtual ~RC4Test();

private:
	void GenerateKey(void);
	void crypt_single(void);
	void crypt_Update(void);
private:
	CK_OBJECT_HANDLE m_hKey;
};

#endif
