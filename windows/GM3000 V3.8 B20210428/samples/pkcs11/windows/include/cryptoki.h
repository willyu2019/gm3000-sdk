/* cryptoki.h include file for PKCS #11. */
/* $Revision: 1.4 $ */


#ifdef WIN32
	#include "cryptoki_win32.h"
#else
	#include "cryptoki_linux.h"
#endif

