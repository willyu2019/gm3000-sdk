// DlgUserPIN.cpp : implementation file
//

#include "stdafx.h"
#include "PKCSDemo.h"
#include "DlgUserPIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgUserPIN dialog

CString g_strUserPIN = "";

DlgUserPIN::DlgUserPIN(CWnd* pParent /*=NULL*/)
	: CDialog(DlgUserPIN::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgUserPIN)
	m_strUserPIN = _T("");
	//}}AFX_DATA_INIT
}


void DlgUserPIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgUserPIN)
	DDX_Text(pDX, IDC_USERPIN, m_strUserPIN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgUserPIN, CDialog)
	//{{AFX_MSG_MAP(DlgUserPIN)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgUserPIN message handlers

void DlgUserPIN::OnCancel() 
{
	g_strUserPIN = "";
	CDialog::OnCancel();
}

void DlgUserPIN::OnOK() 
{
	UpdateData();
	g_strUserPIN = m_strUserPIN;
	CDialog::OnOK();
}
