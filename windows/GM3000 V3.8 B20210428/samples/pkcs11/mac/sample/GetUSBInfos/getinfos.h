#ifndef GETINFOS_H
#define GETINFOS_H

#include <stdio.h> 
#include <string.h>
#include <dlfcn.h>
#include "../../include/cryptoki.h"

class CGetInfos  
{
public:
	bool LoadLib();
	void ShowCryptokiInfos(CK_INFO_PTR pInfo);
	unsigned long GetCryptokiInfos(CK_INFO_PTR pInfo);
	void ShowTokenInfo(CK_TOKEN_INFO_PTR tokenInfo);
	void ShowSlotInfo(CK_SLOT_INFO_PTR slotinfo);
	void CheckRV(unsigned long rv);
	//Get infos functions:
	unsigned long GetTokenInfos(CK_TOKEN_INFO_PTR pTokenInfo);
	unsigned long GetSlotInfos(CK_SLOT_INFO_PTR pSlotInfo);
	//Initialize and destroy:
	CGetInfos();
	virtual ~CGetInfos();
private:
	CK_FUNCTION_LIST_PTR m_pToken;
	void* m_hSO;
	char* m_path;
};

#endif 
