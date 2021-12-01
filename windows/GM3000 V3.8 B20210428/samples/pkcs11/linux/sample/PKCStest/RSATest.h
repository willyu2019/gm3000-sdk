
#ifndef RSA_H
#define RSA_H

#include "../../include/cryptoki.h"

extern CK_SESSION_HANDLE hSession;
extern CK_FUNCTION_LIST_PTR m_gToken;

class RSATest
{
public:
	RSATest(void);
	virtual ~RSATest();
	void RsaKeyGenerationTest(void);
	
};

#endif 
