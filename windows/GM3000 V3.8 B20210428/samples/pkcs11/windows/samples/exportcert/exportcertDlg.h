// exportcertDlg.h : header file
//

#if !defined(AFX_EXPORTCERTDLG_H__F527412F_8BF4_444F_98BD_F4D84301BB3C__INCLUDED_)
#define AFX_EXPORTCERTDLG_H__F527412F_8BF4_444F_98BD_F4D84301BB3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pkcs11lib.h"///////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CExportcertDlg dialog

class CExportcertDlg : public CDialog
{
// Construction
public:
	BOOL CertState;
	CExportcertDlg(CWnd* pParent = NULL);	// standard constructor
	~CExportcertDlg();
// Dialog Data
	//{{AFX_DATA(CExportcertDlg)
	enum { IDD = IDD_EXPORTCERT_DIALOG };
	CListCtrl	m_CertList;
	CButton	m_btnenum;
	CButton	m_btnlogin;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportcertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	CK_SLOT_ID_PTR m_pSlotList;
	CK_VOID_PTR m_pApplication;
	CK_SESSION_HANDLE m_hSession;

	// Generated message map functions
	//{{AFX_MSG(CExportcertDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnLogin();
	afx_msg void OnBtnEnum();
	afx_msg void OnRclickCert(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	private:
	
	PKCS11Wraper*  m_pPkcs11;//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTCERTDLG_H__F527412F_8BF4_444F_98BD_F4D84301BB3C__INCLUDED_)
