// exportcertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "exportcert.h"
#include "exportcertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DlgUserPIN.h"

#define LIBNAME "gm3000_pkcs11.dll"

//////////////////////////////////////////////////////////////////////////
//byte to string according with special format

static CString nByteToStr(DWORD dwSize, void* pData, DWORD dwByte, DWORD dwSplit);

CString nByteToStr(DWORD dwSize, void* pData, DWORD dwByte, DWORD dwSplit)
{
	BYTE* pBuf = (BYTE*)pData; // local pointer to a BYTE in the BYTE array.
	
	CString strRet("");
	DWORD nLine = 0;
	DWORD dwLines = 0;
	DWORD dwRest = 0;
	bool bNeedSplit = true;
	char szTemp[20] = {0, };
	
	DWORD dwBlock = 0;	
	if(0 == dwSplit)
	{
		dwSplit = dwSize;
		bNeedSplit = false;
	}
	
	dwRest = dwSize % dwSplit;
	dwLines = dwSize / dwSplit;
	
	DWORD i, j, k, m;
	for(i = 0; i < dwLines; i++)
	{
		DWORD dwRestTemp = dwSplit % dwByte;
		DWORD dwByteBlock = dwSplit / dwByte;
		
		for(j = 0; j < dwByteBlock; j++)
		{
			for(k = 0; k < dwByte; k++)
			{
				wsprintf(szTemp, "%02X", pBuf[i * dwSplit + j * dwByte + k]);
				strRet += szTemp;
			}
			strRet += " ";
		}
		if(dwRestTemp)
		{
			for(m = 0; m < dwRestTemp; m++)
			{
				wsprintf(
					szTemp, "%02X",
					pBuf[i * dwSplit + j * dwByte + k - 3 + dwRestTemp]);
				strRet += szTemp;
			}
		}
		if(bNeedSplit)
			strRet += "\r\n";
	}
	
	if(dwRest)
	{
		DWORD dwRestTemp = dwRest % dwByte;
		DWORD dwByteBlock = dwRest / dwByte;
		for(j = 0; j < dwByteBlock; j++)
		{
			for(k = 0; k < dwByte; k++)
			{
				wsprintf(szTemp, "%02X", pBuf[dwSize - dwRest + k]);
				strRet += szTemp;
			}
			strRet += " ";
		}
		if(dwRestTemp)
		{
			for(m = 0; m < dwRestTemp; m++)
			{
				wsprintf(
					szTemp, "%02X",
					pBuf[dwSize - dwRest + k - 1 + dwRestTemp]);
				strRet += szTemp;
			}
		}
		if(bNeedSplit)
			strRet += "\r\n";
	}
	
	
	return strRet;
}  // End of ByteToStr

/////////////////////////////////////////////////////////////////////////////
// CExportcertDlg dialog

CExportcertDlg::CExportcertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportcertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportcertDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSlotList = NULL_PTR;
	m_pApplication = new char[255];
	ZeroMemory(m_pApplication, 255);
	lstrcpy((char*)m_pApplication, "Enum Object App");
	m_hSession = NULL_PTR;
	CertState=0;
}
CExportcertDlg::~CExportcertDlg()
{
	if(m_hSession)
	{
		m_pPkcs11->C_CloseSession(m_hSession);
		m_hSession = NULL_PTR;
	}
	delete[] m_pApplication;
	if(m_pSlotList)
	{
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
}


void CExportcertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportcertDlg)
	DDX_Control(pDX, IDC_CERT, m_CertList);
	DDX_Control(pDX, IDC_BTN_ENUM, m_btnenum);
	DDX_Control(pDX, IDC_BTN_LOGIN, m_btnlogin);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExportcertDlg, CDialog)
	//{{AFX_MSG_MAP(CExportcertDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBtnLogin)
	ON_BN_CLICKED(IDC_BTN_ENUM, OnBtnEnum)
	ON_NOTIFY(NM_RCLICK, IDC_CERT, OnRclickCert)
	ON_COMMAND(ID_EXPORT, OnExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportcertDlg message handlers

BOOL CExportcertDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_btnenum.EnableWindow(FALSE);
	

	m_CertList.DeleteAllItems();
	m_CertList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_CertList.InsertColumn(0,"Certificate Type");
	m_CertList.InsertColumn(1,"Certificate Label");
	m_CertList.InsertColumn(2,"Certificate Subject");
	m_CertList.InsertColumn(3,"Certificate Value");

	m_CertList.SetColumnWidth(0,120);
	m_CertList.SetColumnWidth(1,120);
	m_CertList.SetColumnWidth(2,120);
	m_CertList.SetColumnWidth(3,120);

	m_CertList.SetSelectionMark(0);

	
	m_CertList.DeleteAllItems();

	if(0 != Pkcs11Initialize(LIBNAME))
	{
		AfxMessageBox("Failed to load pkcs11 module.");
		return FALSE;
	}
	
	m_pPkcs11 = GetPkcs11Wrapper();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExportcertDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExportcertDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CExportcertDlg::OnBtnLogin() 
{
	// TODO: Add your control notification handler code here
	if(m_hSession)
		return;
	
	::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	
	CK_RV rv = CKR_OK;
	CK_ULONG ulCount = 0;
	rv = m_pPkcs11->C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if(CKR_OK != rv )
	{
		AfxMessageBox("Can't acquire token information.", MB_OK | MB_ICONERROR);
		return;
	}
	if(0 >= ulCount)
	{
		AfxMessageBox("Can't connect token, make sure one token has been inserted.", MB_OK | MB_ICONERROR);
		return;
	}
	
	m_pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
	if (! m_pSlotList) 
	{
		AfxMessageBox("Not enough memory!", MB_OK | MB_ICONERROR);
		return;
	}
	
	rv = m_pPkcs11->C_GetSlotList(TRUE, m_pSlotList, &ulCount);
	if(CKR_OK != rv )
	{
		AfxMessageBox("Can't acquire token information.", MB_OK | MB_ICONERROR);
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
		return;
	}
	if(0 >= ulCount)
	{
		AfxMessageBox("Can't connect token, make sure one token has been inserted.", MB_OK | MB_ICONERROR);
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
		return;
	}

	CK_TOKEN_INFO tokenInfo = {0};
	rv = m_pPkcs11->C_GetTokenInfo(m_pSlotList[0], &tokenInfo);
	if (CKR_OK != rv)
	{
		AfxMessageBox("Can't get token information.", MB_OK | MB_ICONERROR);
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
		return;
	}
	
	rv = m_pPkcs11->C_OpenSession(
		m_pSlotList[0],  CKF_RW_SESSION | CKF_SERIAL_SESSION,
		&m_pApplication, NULL_PTR, &m_hSession);

	if(m_pSlotList)
	{
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
		
	if(CKR_OK != rv )
	{
		AfxMessageBox("Can't acquire token information", MB_OK | MB_ICONERROR);
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
	else
	{
		DlgUserPIN* dlgUserPIN = new DlgUserPIN;
		dlgUserPIN->DoModal();
		delete dlgUserPIN;
		if("" == g_strUserPIN)
		{
			AfxMessageBox("You must input your User PIN first.", MB_OK | MB_ICONERROR);
			if(m_hSession)
			{
				m_pPkcs11->C_CloseSession(m_hSession);
				m_hSession = NULL_PTR;
			}
			return;
		}
		
		CK_ULONG ulPIN = g_strUserPIN.GetLength();
		CK_BYTE_PTR pPIN = (CK_BYTE_PTR)g_strUserPIN.GetBuffer(ulPIN);
		
		::SetCursor(::LoadCursor(NULL, IDC_WAIT));
		rv = m_pPkcs11->C_Login(m_hSession, CKU_USER, pPIN, ulPIN);

		if(CKR_OK != rv)
		{
			AfxMessageBox("Invalid User PIN!", MB_OK | MB_ICONERROR);
			if(m_hSession)
			{
				m_pPkcs11->C_CloseSession(m_hSession);
				m_hSession = NULL_PTR;
			}
			if(m_pSlotList)
			{
				delete[] m_pSlotList;
				m_pSlotList = NULL_PTR;
			}
			
			return;
		}
		else
			AfxMessageBox("Login Successfully.");
			m_btnlogin.EnableWindow(false);
			m_btnenum.EnableWindow(true);
	}
	
}

void CExportcertDlg::OnBtnEnum() 
{
	// TODO: Add your control notification handler code here
	
	CK_OBJECT_CLASS dataClass = CKO_CERTIFICATE;
	BOOL IsToken=true;
	CK_ATTRIBUTE pTempl[] = 
	{
		{CKA_CLASS, &dataClass, sizeof(CKO_CERTIFICATE)},
		{CKA_TOKEN, &IsToken, sizeof(true)}
	};

	
	m_pPkcs11->C_FindObjectsInit(m_hSession, pTempl, 2);


	m_CertList.DeleteAllItems();
	int index=0;
	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_RV ckrv = 0;
	do
	{
		ckrv = m_pPkcs11->C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
		if(CKR_OK != ckrv)
		{
			break;
		}
		if(1 != ulRetCount)
			break;
		
		CK_ATTRIBUTE pAttrTemp[] = 
		{
			{CKA_CLASS, NULL, 0},
			{CKA_CERTIFICATE_TYPE,NULL,0},
			{CKA_LABEL, NULL, 0},
			{CKA_SUBJECT,NULL,0},
			{CKA_ID,NULL,0},
			{CKA_VALUE,NULL,0}
		};
		
		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 6);
		if(ckrv != CKR_OK)
		{
			break;
		}
		
		pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
		pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
		pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen+1];
		pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen+1];
		pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen+1];
		pAttrTemp[5].pValue = new char[pAttrTemp[5].ulValueLen ];
	
		ZeroMemory(pAttrTemp[0].pValue, pAttrTemp[0].ulValueLen);
		ZeroMemory(pAttrTemp[1].pValue, pAttrTemp[1].ulValueLen);
		ZeroMemory(pAttrTemp[2].pValue, pAttrTemp[2].ulValueLen+1);	
		ZeroMemory(pAttrTemp[3].pValue, pAttrTemp[3].ulValueLen+1);
		ZeroMemory(pAttrTemp[4].pValue, pAttrTemp[4].ulValueLen+1);
		ZeroMemory(pAttrTemp[5].pValue, pAttrTemp[5].ulValueLen);
		
		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 6);
		if(ckrv != CKR_OK)
		{
			delete[] pAttrTemp[0].pValue;
			delete[] pAttrTemp[1].pValue;
			delete[] pAttrTemp[2].pValue;
			delete[] pAttrTemp[3].pValue;
			delete[] pAttrTemp[4].pValue;
			delete[] pAttrTemp[5].pValue;
			break;
		}
		
		CString strCertType;
		if(*(int*)pAttrTemp[1].pValue==CKC_X_509)
		{
			strCertType="CKC_X_509";
		}
		else 
			if(*(int*)pAttrTemp[1].pValue==CKC_X_509_ATTR_CERT)
			{
				strCertType="CKC_X_509_ATTR_CERT";
			}
			
		int itemIndex = m_CertList.InsertItem(LVIF_TEXT | LVIF_PARAM, index, strCertType, 0, 0, 0, 0);

		CString strvale = (char*)pAttrTemp[2].pValue;
		m_CertList.SetItem(itemIndex,1,LVIF_TEXT, strvale , 0, 0, 0, 0);	

		CString strsubject;
		CERT_NAME_BLOB  SubName;
		SubName.cbData=pAttrTemp[3].ulValueLen;
		SubName.pbData=(BYTE*)pAttrTemp[3].pValue;
		
		DWORD dwStrDetailType =
			CERT_NAME_STR_NO_QUOTING_FLAG | 
			CERT_NAME_STR_REVERSE_FLAG |
			CERT_SIMPLE_NAME_STR |
			CERT_OID_NAME_STR |
			CERT_NAME_STR_CRLF_FLAG
			;
		
		DWORD dwName = 0;
		dwName = CertNameToStr(
			MY_ENCODING_TYPE,
			&SubName,
			dwStrDetailType,
			NULL,
			0);
		
		if (0 == dwName)
		{
			AfxMessageBox("Error: Can not get length of subject name.");
			return;
		}
		
		char* pszTemp = NULL;
		if(!(pszTemp = new char[dwName]))
		{
			AfxMessageBox("Not enough memory.");
			return;
		}
		
		//--------------------------------------------------------------------
		//       Call the function again to get the string 
		dwName = CertNameToStr(
			MY_ENCODING_TYPE,
			&SubName,
			dwStrDetailType,
			pszTemp,
			dwName);
		
		//--------------------------------------------------------------------
		//      If the function succeeded, it returns the 
		//      number of bytes copied to the pszName buffer.
		if (1 < dwName)
		{
			strsubject = pszTemp;
		}
		delete[] pszTemp;

		m_CertList.SetItem(itemIndex,2,LVIF_TEXT, strsubject , 0, 0, 0, 0);

		CString strvalue=nByteToStr(pAttrTemp[5].ulValueLen, pAttrTemp[5].pValue, 1, 16);
		m_CertList.SetItem(itemIndex,3,LVIF_TEXT, strvalue , 0, 0, 0, 0);

		CertState=true;
		delete[] pAttrTemp[0].pValue;
		delete[] pAttrTemp[1].pValue;
		delete[] pAttrTemp[2].pValue;
		delete[] pAttrTemp[3].pValue;
		delete[] pAttrTemp[4].pValue;
		delete[] pAttrTemp[5].pValue;

		index++;

	}while(true);
	
	
}


void CExportcertDlg::OnRclickCert(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(CertState)
	{
	
		CMenu PopMenu;
		CMenu *pMenu;
		
		CPoint pt;
		CRect rect;
		int tem=m_CertList.GetNextItem(-1,LVNI_SELECTED);
		m_CertList.GetItemPosition(tem,&pt);



		PopMenu.LoadMenu(IDR_MENU);
		ClientToScreen (&pt);

		pMenu = PopMenu.GetSubMenu (0);
		pMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				pt.x+300, pt.y+50, this);
	}
	
	*pResult = 0;
}

void CExportcertDlg::OnExport() 
{
	// TODO: Add your command handler code here
	int nItem = m_CertList.GetNextItem(-1,LVNI_SELECTED);
	if(nItem==-1) 
	{
		AfxMessageBox("Please select a certificate to export.");
		return ;
	}

	CString   strlabel =m_CertList.GetItemText(nItem,1);
	
	CK_OBJECT_CLASS dataClass = CKO_CERTIFICATE;
	BOOL IsToken=true;
	CK_ATTRIBUTE pTempl[] = 
	{
		{CKA_CLASS, &dataClass, sizeof(CKO_CERTIFICATE)},
		{CKA_TOKEN, &IsToken, sizeof(true)}
	};

	
	m_pPkcs11->C_FindObjectsInit(m_hSession, pTempl, 2);



	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_RV ckrv = 0;
	do
	{
		ckrv = m_pPkcs11->C_FindObjects(m_hSession, &hCKObj, 1, &ulRetCount);
		if(CKR_OK != ckrv)
		{
			break;	
		}
		if(1 != ulRetCount)
			break;
		
		CK_ATTRIBUTE pAttrTemp[] = 
		{
			{CKA_CLASS, NULL, 0},
			{CKA_CERTIFICATE_TYPE,NULL,0},
			{CKA_LABEL, NULL, 0},
			{CKA_SUBJECT,NULL,0},
			{CKA_ID,NULL,0},
			{CKA_VALUE,NULL,0}
		};
		
		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 6);
		if(ckrv != CKR_OK)
		{
			break;
		}
		
		pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
		pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
		pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen+1];
		pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen+1];
		pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen+1];
		pAttrTemp[5].pValue = new char[pAttrTemp[5].ulValueLen ];
	
		ZeroMemory(pAttrTemp[0].pValue, pAttrTemp[0].ulValueLen);
		ZeroMemory(pAttrTemp[1].pValue, pAttrTemp[1].ulValueLen);
		ZeroMemory(pAttrTemp[2].pValue, pAttrTemp[2].ulValueLen+1);	
		ZeroMemory(pAttrTemp[3].pValue, pAttrTemp[3].ulValueLen+1);
		ZeroMemory(pAttrTemp[4].pValue, pAttrTemp[4].ulValueLen+1);
		ZeroMemory(pAttrTemp[5].pValue, pAttrTemp[5].ulValueLen);
		
		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 6);
		if(ckrv != CKR_OK)
		{
			delete[] pAttrTemp[0].pValue;
			delete[] pAttrTemp[1].pValue;
			delete[] pAttrTemp[2].pValue;
			delete[] pAttrTemp[3].pValue;
			delete[] pAttrTemp[4].pValue;
			delete[] pAttrTemp[5].pValue;
			break;
		}
		

		CString strvale = (char*)pAttrTemp[2].pValue;
		if(strlabel==strvale)
		{
			CFileDialog CertFileDialog(false, NULL, "token_cert.cer");
			if(CertFileDialog.DoModal()==IDOK)
			{			
				CString CertFileName=CertFileDialog.GetPathName();
				CFile  certFile;
				certFile.Open(CertFileName,CFile::modeCreate|CFile::modeWrite);
				certFile.Write(pAttrTemp[5].pValue,pAttrTemp[5].ulValueLen);
				certFile.Close();
				AfxMessageBox("Certificate exported successfully.");
			}
			return;
		}


		delete[] pAttrTemp[0].pValue;
		delete[] pAttrTemp[1].pValue;
		delete[] pAttrTemp[2].pValue;
		delete[] pAttrTemp[3].pValue;
		delete[] pAttrTemp[4].pValue;
		delete[] pAttrTemp[5].pValue;

		
		
	}while(true);
    AfxMessageBox("Can't find certificate to export.");
	
}
