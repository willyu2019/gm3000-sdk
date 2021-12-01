#ifndef EXPORTCER_H
#define EXPORTCER_H

class CExportcert
{
public:
	bool CertState;
	CExportcert();
	~CExportcert();
	CK_RV Init();
	void Enum();
	void Export();

protected:
	CK_SLOT_ID_PTR m_pSlotList;
	CK_VOID_PTR m_pApplication;
	CK_SESSION_HANDLE m_hSession;

};

#endif
