#ifndef ENUMOBJ_H
#define ENUMOBJ_H

class EnumObj
{
public:
	void ShowErr(char *strInfo, CK_RV rv);
	void StartOP();
	EnumObj();
	~EnumObj();
	CK_RV Connect();
	CK_RV Login();
	void Enum();
	void Data();
	void Public();
	void Private();
	void Secret();
protected:
	CK_SLOT_ID_PTR m_pSlotList;
	CK_VOID_PTR m_pApplication;
	CK_SESSION_HANDLE m_hSession;
};

#endif
