#if !defined(AFX_DLGUSERPIN_H__F6F7331C_D2E1_416D_8DB5_43CE221B7D46__INCLUDED_)
#define AFX_DLGUSERPIN_H__F6F7331C_D2E1_416D_8DB5_43CE221B7D46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUserPIN.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgUserPIN dialog
extern CString g_strUserPIN;


class DlgUserPIN : public CDialog
{
// Construction
public:
	DlgUserPIN(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgUserPIN)
	enum { IDD = IDD_DLG_USERPIN };
	CString	m_strUserPIN;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgUserPIN)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgUserPIN)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUSERPIN_H__F6F7331C_D2E1_416D_8DB5_43CE221B7D46__INCLUDED_)
