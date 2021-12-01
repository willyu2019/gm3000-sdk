/*
[]=========================================================================[]

FILE:
	getinfos.h
DESC:

[]=========================================================================[]
*/

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETINFOS_H__DF276D83_F3BC_4371_8530_1257E8753DBB__INCLUDED_)
#define AFX_GETINFOS_H__DF276D83_F3BC_4371_8530_1257E8753DBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGetInfos  
{
public:
	bool LoadLib();
	void ShowCryptokiInfos(CK_INFO_PTR pInfo);
	unsigned long GetCryptokiInfos(CK_INFO_PTR pInfo);
	void ShowTokenInfo(CK_TOKEN_INFO_PTR tokenInfo);
	void ShowSlotInfo(CK_SLOT_INFO_PTR slotinfo);
	bool CheckRV(const char* szInfo, unsigned long rv);
	//{{{Get infos functions:
	unsigned long GetTokenInfos(CK_TOKEN_INFO_PTR pTokenInfo);
	unsigned long GetSlotInfos(CK_SLOT_INFO_PTR pSlotInfo);
	//}}}
	//{{{Initialize and destroy:
	CGetInfos();
	virtual ~CGetInfos();
	//}}}
private:
	CK_FUNCTION_LIST_PTR m_pToken;
#if defined(WIN32)
	HMODULE m_hDll;
#else
	void* m_hDll;
#endif

	char* m_path;
};

#endif // !defined(AFX_GETINFOS_H__DF276D83_F3BC_4371_8530_1257E8753DBB__INCLUDED_)
