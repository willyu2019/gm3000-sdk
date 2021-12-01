// PKCSDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PKCSDemo.h"
#include "PKCSDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DlgUserPIN.h"

#define NEWLINE "\r\n"

#define	countof(a)			(sizeof(a)/ sizeof(CK_ATTRIBUTE))

#define LIBNAME "gm3000_pkcs11.dll"

//////////////////////////////////////////////////////////////////////////////
CK_BBOOL bTrue = TRUE;
CK_ULONG ulModulusBits = MODULUS_BIT_LENGTH; 
CK_BYTE subject[] = "Sample RSA Key Pair";
CK_ULONG keyType = CKK_RSA;

CK_OBJECT_CLASS pubClass = CKO_PUBLIC_KEY;
CK_ATTRIBUTE pubTemplate[] = { 
	{CKA_CLASS,			&pubClass,		sizeof(pubClass)},
	{CKA_KEY_TYPE,		&keyType,		sizeof(keyType)},
	{CKA_SUBJECT,		subject,		sizeof(subject)},
	{CKA_MODULUS_BITS,	&ulModulusBits, sizeof(ulModulusBits)},
	{CKA_ENCRYPT,		&bTrue,			sizeof(bTrue)},
	{CKA_TOKEN,			&bTrue,			sizeof(bTrue)},
	{CKA_WRAP,			&bTrue,			sizeof(bTrue)},
};	// Without specifying CKA_PRIVATE attribute in this case, 
// a public key will be created by default. 

CK_OBJECT_CLASS priClass	= CKO_PRIVATE_KEY;
CK_ATTRIBUTE priTemplate[] = {
	{CKA_CLASS,			&priClass,	sizeof(priClass)},
	{CKA_KEY_TYPE,		&keyType,	sizeof(keyType)},
	{CKA_SUBJECT,		subject,	sizeof(subject)},
	{CKA_DECRYPT,		&bTrue,		sizeof(bTrue)},
	{CKA_PRIVATE,		&bTrue,		sizeof(bTrue)},
	{CKA_SENSITIVE,		&bTrue,		sizeof(bTrue)},
	{CKA_TOKEN,			&bTrue,		sizeof(bTrue)},
	{CKA_EXTRACTABLE,	&bTrue,		sizeof(bTrue)},
	{CKA_UNWRAP,		&bTrue,		sizeof(bTrue)},
};

CK_MECHANISM keyGenMechanism = {
	CKM_RSA_PKCS_KEY_PAIR_GEN, 
		NULL_PTR, 
		0
};	
CK_MECHANISM ckMechanism = {CKM_RSA_PKCS, NULL_PTR, 0};
CK_BYTE pbMsg[] = "USB token bring you more security.";
CK_ULONG ulMsgLen = strlen((const char *)pbMsg);


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//////////////////////////////////////////////////////////////////////////
// byte to string according with special format
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
			strRet += NEWLINE;
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
			strRet += NEWLINE;
	}
	
	
	return strRet;
}  // End of ByteToStr

/////////////////////////////////////////////////////////////////////////////
// CPKCSDemoDlg dialog

CPKCSDemoDlg::CPKCSDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPKCSDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPKCSDemoDlg)
	m_strInfo = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSlotList = NULL_PTR;
	m_pApplication = new char[255];
	ZeroMemory(m_pApplication, 255);
	lstrcpy((char*)m_pApplication, "PKCS Demo App");
	m_hSession = NULL_PTR;
	m_hPubKey = NULL_PTR;
	m_hPriKey = NULL_PTR;
	m_bKeyGen = FALSE;
	ZeroMemory(m_pSignature, MODULUS_BIT_LENGTH);
	m_ulSignatureLen = sizeof(m_pSignature);
	m_pbCipherBuffer = NULL_PTR;
	m_ulCipherLen = 0;
}

CPKCSDemoDlg::~CPKCSDemoDlg()
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
	if(m_pbCipherBuffer)
	{
		delete[] m_pbCipherBuffer;
		m_pbCipherBuffer = NULL_PTR;
		m_ulCipherLen = 0;
	}
}

void CPKCSDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPKCSDemoDlg)
	DDX_Control(pDX, IDC_BTN_VERIFY, m_btnVerify);
	DDX_Control(pDX, IDC_BTN_SIGN, m_btnSign);
	DDX_Control(pDX, IDC_BTN_ENCRYPT, m_btnEncrypt);
	DDX_Control(pDX, IDC_BTN_DECRYPT, m_btnDecrypt);
	DDX_Control(pDX, IDC_BTN_KEYPAIRGEN, m_btnKeyPairGen);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_INFO, m_edtInfo);
	DDX_Text(pDX, IDC_INFO, m_strInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPKCSDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CPKCSDemoDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_KEYPAIRGEN, OnBtnKeypairgen)
	ON_BN_CLICKED(IDC_BTN_SIGN, OnBtnSign)
	ON_BN_CLICKED(IDC_BTN_VERIFY, OnBtnVerify)
	ON_BN_CLICKED(IDC_BTN_CLEARINFO, OnBtnClearinfo)
	ON_BN_CLICKED(IDC_BTN_ENCRYPT, OnBtnEncrypt)
	ON_BN_CLICKED(IDC_BTN_DECRYPT, OnBtnDecrypt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPKCSDemoDlg message handlers

BOOL CPKCSDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	ShowMsg("Before any operation, you should connect to token!"NEWLINE);

	m_btnKeyPairGen.EnableWindow(FALSE);
	m_btnSign.EnableWindow(FALSE);
	m_btnVerify.EnableWindow(FALSE);
	m_btnEncrypt.EnableWindow(FALSE);
	m_btnDecrypt.EnableWindow(FALSE);


	if( 0 != Pkcs11Initialize(LIBNAME))
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

void CPKCSDemoDlg::OnPaint() 
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
HCURSOR CPKCSDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPKCSDemoDlg::ShowMsg(CString strInfo)
{
	m_strInfo += strInfo;
	UpdateData(FALSE);

	int nLastLine = m_edtInfo.GetLineCount();// GetFirstVisibleLine();

	if (nLastLine > 0)
	{
		m_edtInfo.LineScroll(nLastLine, 0);
	}
}

void CPKCSDemoDlg::StartOP(void)
{
	ShowMsg(NEWLINE"================================================");
}

void CPKCSDemoDlg::ShowErr(CString strInfo, CK_RV rv)
{
	CString strTemp("");
	strTemp.Format(strInfo.GetBuffer(strInfo.GetLength()), rv);
	ShowMsg(strTemp);
}

void CPKCSDemoDlg::ClearMsg(void)
{
	m_strInfo = "";
	UpdateData(FALSE);
}

void CPKCSDemoDlg::OnBtnConnect() 
{
	if(m_hSession)
		return;

	::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	StartOP();

	CK_RV rv;
	CK_ULONG ulCount = 0;
	rv = m_pPkcs11->C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if(CKR_OK != rv )
	{
		ShowErr(NEWLINE"Can't acquire information of slot, ErrorCode: 0x%08X."NEWLINE, rv);
		return;
	}
	if(0 >= ulCount)
	{
		ShowMsg(NEWLINE"Can't connect to token, make sure one token has been inserted."NEWLINE);
		return;
	}

	m_pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
	if (! m_pSlotList) 
	{
		ShowMsg(NEWLINE"Not enough memory!"NEWLINE);
		return;
	}

	rv = m_pPkcs11->C_GetSlotList(TRUE, m_pSlotList, &ulCount);
	if(CKR_OK != rv )
	{
		ShowErr(NEWLINE"Can't acquire information of slot, ErrorCode: 0x%08X."NEWLINE, rv);
		return;
	}
	if(0 >= ulCount)
	{
		ShowMsg(NEWLINE"Can't connect to token, make sure one token has been inserted."NEWLINE);
		return;
	}

	rv = m_pPkcs11->C_OpenSession(
		m_pSlotList[0],  CKF_RW_SESSION | CKF_SERIAL_SESSION,
		&m_pApplication, NULL_PTR, &m_hSession);
	if(CKR_OK != rv )
	{
		ShowErr(NEWLINE"Can't Acquire information of slot, ErrorCode: 0x%08X."NEWLINE, rv);
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
	else
	{
		ShowMsg(NEWLINE"Connect to token Successfully !"NEWLINE);
		m_btnConnect.EnableWindow(FALSE);
		ShowMsg(NEWLINE"Before next test step, need to generate RSA key pair!"NEWLINE);
		m_btnKeyPairGen.EnableWindow(TRUE);
	}
}

void CPKCSDemoDlg::OnBtnKeypairgen() 
{
	StartOP();

	CK_TOKEN_INFO tokenInfo = {0};
	CK_RV rv = m_pPkcs11->C_GetTokenInfo(m_pSlotList[0], &tokenInfo);
	if (CKR_OK != rv)
	{
		ShowMsg(NEWLINE"Can not get token information!"NEWLINE);
		return;
	}

	DlgUserPIN* dlgUserPIN = new DlgUserPIN;
	dlgUserPIN->DoModal();
	delete dlgUserPIN;
	if("" == g_strUserPIN)
	{
		ShowMsg(NEWLINE"You should enter User PIN before generating RSA Key pair!"NEWLINE);
		return;
	}

	CK_ULONG ulPIN = g_strUserPIN.GetLength();
	CK_BYTE_PTR pPIN = (CK_BYTE_PTR)g_strUserPIN.GetBuffer(ulPIN);

	::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	rv = m_pPkcs11->C_Login(m_hSession, CKU_USER, pPIN, ulPIN);

	if(CKR_OK != rv)
	{
		ShowErr(NEWLINE"Can't use your User PIN login to token ,ErrorCode: 0x%08X."NEWLINE, rv);
		return;
	}
	else
		ShowMsg(NEWLINE"Logging in to token Successfully!"NEWLINE);


	::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	rv = m_pPkcs11->C_GenerateKeyPair(
		m_hSession, &keyGenMechanism,
		pubTemplate, countof(pubTemplate),
		priTemplate, countof(priTemplate),
		&m_hPubKey, &m_hPriKey);
	if(CKR_OK != rv)
	{
		ShowErr(NEWLINE"Can't generate RSA key pair, ErrorCode: 0x%08X."NEWLINE, rv);
		return;
	}
	else
	{
		m_btnKeyPairGen.EnableWindow(FALSE);
		m_btnSign.EnableWindow(TRUE);
		m_btnEncrypt.EnableWindow(TRUE);
		ShowMsg(NEWLINE"Generate Key Pair Successfully!"NEWLINE);
		ShowMsg(NEWLINE"Now,You can Sign,Verify,Encrypt and Decrypt use the Key Pair!"NEWLINE);
		m_bKeyGen = TRUE;
	}
}

void CPKCSDemoDlg::OnCancel() 
{
	OnOK();
}

void CPKCSDemoDlg::OnOK() 
{
	CK_RV rv;

	if(m_bKeyGen)
	{
		StartOP();
		BOOL bRet = TRUE;
		ShowMsg(NEWLINE"Clear Key Pair which was generated!"NEWLINE);
		UpdateWindow();
		::SetCursor(::LoadCursor(NULL, IDC_WAIT));

		rv = m_pPkcs11->C_DestroyObject(m_hSession, m_hPubKey);
		if(CKR_OK != rv)
		{
			bRet = FALSE;
			ShowErr(NEWLINE"Can't delete public key ,Error code 0x%08X."NEWLINE, rv);
		}
		rv = m_pPkcs11->C_DestroyObject(m_hSession, m_hPriKey);
		if(CKR_OK != rv)
		{
			bRet = FALSE;
			ShowErr(NEWLINE"Can't delete private key ,Error code 0x%08X."NEWLINE, rv);
		}
		if(bRet)
			ShowMsg(NEWLINE"Clear Key Pair that was generated successfully!"NEWLINE);
	}

	CDialog::OnOK();
}

void CPKCSDemoDlg::OnBtnSign() 
{
	StartOP();
	CK_RV rv;
	rv = m_pPkcs11->C_SignInit(m_hSession, &ckMechanism, m_hPriKey);
	if(CKR_OK != rv)
	{
		ShowErr(NEWLINE"Fail to call SignInit!Error code 0x%08X."NEWLINE, rv);
		return;
	}

	rv = m_pPkcs11->C_Sign(m_hSession, 
		pbMsg,
		ulMsgLen, 
		m_pSignature, &m_ulSignatureLen);
	if(CKR_OK != rv)
	{
		ShowErr(NEWLINE"Fail to Sign!Error code 0x%08X."NEWLINE, rv);
		return;
	}
	else
	{
		ShowMsg(NEWLINE"Data:"NEWLINE);
		ShowMsg((char*)pbMsg);
		ShowMsg(NEWLINE" was Signed successfully!"NEWLINE"Signed Data is:"NEWLINE);
		ShowMsg(nByteToStr(m_ulSignatureLen, m_pSignature, 1, 16));
		ShowMsg(NEWLINE""NEWLINE"Now you can do RSA Verification!"NEWLINE);
		m_btnVerify.EnableWindow(TRUE);
	}
}

void CPKCSDemoDlg::OnBtnVerify() 
{
	StartOP();
	CK_RV rv;
	rv = m_pPkcs11->C_VerifyInit(m_hSession, &ckMechanism, m_hPubKey);
	if(CKR_OK != rv)
	{
		ShowErr(NEWLINE"Failed to call VerifyInit!Error code 0x%08X."NEWLINE, rv);
		return;
	}

	rv = m_pPkcs11->C_Verify(m_hSession,
		pbMsg, ulMsgLen, 
		m_pSignature, m_ulSignatureLen);
	if(CKR_OK != rv)
		ShowErr(NEWLINE"Fail to call verify!Error code 0x%08X."NEWLINE, rv);
	else
		ShowMsg(NEWLINE"Verify Successfully!"NEWLINE);
}

void CPKCSDemoDlg::OnBtnClearinfo() 
{
	m_strInfo = "";
	UpdateData(FALSE);
}

void CPKCSDemoDlg::OnBtnEncrypt() 
{
	StartOP();
	CK_RV rv;

	if(m_pbCipherBuffer)
	{
		delete[] m_pbCipherBuffer;
		m_pbCipherBuffer = NULL_PTR;
		m_ulCipherLen = 0;
	}

	rv = m_pPkcs11->C_EncryptInit(m_hSession,
		&ckMechanism,
		m_hPubKey);
	if(CKR_OK != rv)
	{
		ShowErr(NEWLINE"Fail to call EncryptInit !Error code 0x%08X."NEWLINE, rv);
		return;
	}

	rv = m_pPkcs11->C_Encrypt(m_hSession, pbMsg, ulMsgLen, NULL_PTR, &m_ulCipherLen);
	if(CKR_OK != rv)
	{
		ShowErr(NEWLINE"Can't acquire the size of Data After encrypt,Error code 0x%08X."NEWLINE, rv);
		return;
	}

	m_pbCipherBuffer = (CK_BYTE_PTR)new CK_BYTE[m_ulCipherLen];
	if (! m_pbCipherBuffer)
	{
		ShowMsg(NEWLINE"Can't allocate enough memory!"NEWLINE);
		return;
	}
	ZeroMemory(m_pbCipherBuffer, m_ulCipherLen);

	rv = m_pPkcs11->C_Encrypt(m_hSession, pbMsg, ulMsgLen, m_pbCipherBuffer, &m_ulCipherLen);
	if (CKR_OK != rv)
	{
		ShowErr(NEWLINE"Fail to encrypt!Error code 0x%08X."NEWLINE, rv);
		return;
	}
	ShowMsg(NEWLINE"Data :"NEWLINE);
	ShowMsg((char*)pbMsg);
	ShowMsg(NEWLINE"was encrypted successfully!"NEWLINE"Encryted Data is:"NEWLINE);
	ShowMsg(nByteToStr(m_ulCipherLen, m_pbCipherBuffer, 1, 16));
	ShowMsg(NEWLINE""NEWLINE"Now you can decrypt by using RSA private key!"NEWLINE);
	m_btnDecrypt.EnableWindow(TRUE);
}

void CPKCSDemoDlg::OnBtnDecrypt() 
{
	StartOP();
	CK_BYTE_PTR pbRestoredMsg = NULL;
	CK_ULONG ulRestoredMsgLen = 0;
	CK_RV rv;
	rv = m_pPkcs11->C_DecryptInit(m_hSession,
		&ckMechanism,
		m_hPriKey);
	if(CKR_OK != rv)
	{
		ShowErr(NEWLINE"Fail to call DecryptInit!Error code 0x%08X."NEWLINE, rv);
		return;
	}

	rv = m_pPkcs11->C_Decrypt(m_hSession, m_pbCipherBuffer, m_ulCipherLen, NULL_PTR, &ulRestoredMsgLen);
	if(CKR_OK != rv)
	{
		ShowErr(NEWLINE"Can't acuire size of Data after Decrypt! Error code 0x%08X."NEWLINE, rv);
		return;
	}

	pbRestoredMsg = (CK_BYTE_PTR)new CK_BYTE[ulRestoredMsgLen + 1];
	if (! pbRestoredMsg)
	{
		ShowMsg(NEWLINE"Can't allocate enough memory for Decrypt!"NEWLINE);
		return;
	}

	ZeroMemory(pbRestoredMsg, ulRestoredMsgLen + 1);

	rv = m_pPkcs11->C_Decrypt(m_hSession, m_pbCipherBuffer, m_ulCipherLen, pbRestoredMsg, &ulRestoredMsgLen);
	if (CKR_OK != rv)
	{
		ShowErr(NEWLINE"Fail to call decrypt,Error code 0x%08X."NEWLINE, rv);
		delete[] pbRestoredMsg;
		return;
	}
	ShowMsg(NEWLINE"Decrypt Successfulluy, Data after Decrypt is :"NEWLINE);
	ShowMsg(pbRestoredMsg);
	ShowMsg(NEWLINE);
	delete[] pbRestoredMsg;
}
