// EnumObjDlg.h : header file
//

#if !defined(AFX_ENUMOBJDLG_H__C1F0F3E5_6C0E_48F2_B284_178A6DB5EE16__INCLUDED_)
#define AFX_ENUMOBJDLG_H__C1F0F3E5_6C0E_48F2_B284_178A6DB5EE16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "pkcs11lib.h"///////////////////////////////////////////////////////////////////////////////////////////////////
// CEnumObjDlg dialog

class CEnumObjDlg : public CDialog
{
// Construction
public:
	void ShowErr(CString strInfo, CK_RV rv);
	void StartOP();
	void ShowMsg(CString strInfo);
	CEnumObjDlg(CWnd* pParent = NULL);	// standard constructor
	~CEnumObjDlg();
// Dialog Data
	//{{AFX_DATA(CEnumObjDlg)
	enum { IDD = IDD_ENUMOBJ_DIALOG };
	CButton	m_btnSecret;
	CButton	m_btnPublic;
	CButton	m_btnPrivate;
	CButton	m_btnData;
	CButton	m_btnLogin;
	CButton	m_btnEnum;
	CButton	m_btnConnect;
	CEdit	m_edtInfo;
	CString	m_strInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnumObjDlg)
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
	//{{AFX_MSG(CEnumObjDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnClearinfo();
	afx_msg void OnBtnEnum();
	afx_msg void OnBtnLogin();
	afx_msg void OnBtnData();
	afx_msg void OnBtnPublic();
	afx_msg void OnBtnPrivate();
	afx_msg void OnBtnSecret();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	
	PKCS11Wraper*  m_pPkcs11;//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENUMOBJDLG_H__C1F0F3E5_6C0E_48F2_B284_178A6DB5EE16__INCLUDED_)
