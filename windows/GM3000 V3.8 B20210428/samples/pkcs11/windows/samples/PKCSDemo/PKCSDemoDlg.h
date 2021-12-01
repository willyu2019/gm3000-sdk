// PKCSDemoDlg.h : header file
//

#if !defined(AFX_PKCSDEMODLG_H__C0508FEC_56AA_4EA9_A8B5_A8C48FA7509C__INCLUDED_)
#define AFX_PKCSDEMODLG_H__C0508FEC_56AA_4EA9_A8B5_A8C48FA7509C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pkcs11lib.h"

/////////////////////////////////////////////////////////////////////////////
// CPKCSDemoDlg dialog
const CK_ULONG MODULUS_BIT_LENGTH = 1024;	// should be  1024/2048

class CPKCSDemoDlg : public CDialog
{
// Construction
public:
	CPKCSDemoDlg(CWnd* pParent = NULL);	// standard constructor
	~CPKCSDemoDlg();

	void ShowMsg(CString strInfo);
	void ShowErr(CString strInfo, CK_RV rv);
	void ClearMsg(void);

	void StartOP(void);

// Dialog Data
	//{{AFX_DATA(CPKCSDemoDlg)
	enum { IDD = IDD_PKCSDEMO_DIALOG };
	CButton	m_btnVerify;
	CButton	m_btnSign;
	CButton	m_btnEncrypt;
	CButton	m_btnDecrypt;
	CButton	m_btnKeyPairGen;
	CButton	m_btnConnect;
	CEdit	m_edtInfo;
	CString	m_strInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPKCSDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CK_SLOT_ID_PTR m_pSlotList;
	CK_VOID_PTR m_pApplication;
	CK_SESSION_HANDLE m_hSession;
	CK_OBJECT_HANDLE m_hPubKey;
	CK_OBJECT_HANDLE m_hPriKey;
	BOOL m_bKeyGen;
	CK_BYTE m_pSignature[MODULUS_BIT_LENGTH];
	CK_BYTE_PTR m_pbCipherBuffer;

	CK_ULONG m_ulSignatureLen;
	CK_ULONG m_ulCipherLen;
	// Generated message map functions
	//{{AFX_MSG(CPKCSDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnKeypairgen();
	afx_msg void OnBtnSign();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBtnVerify();
	afx_msg void OnBtnClearinfo();
	afx_msg void OnBtnEncrypt();
	afx_msg void OnBtnDecrypt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
	PKCS11Wraper*  m_pPkcs11;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PKCSDEMODLG_H__C0508FEC_56AA_4EA9_A8B5_A8C48FA7509C__INCLUDED_)
