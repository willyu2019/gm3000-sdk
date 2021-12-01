// EnumObjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EnumObj.h"
#include "EnumObjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DlgUserPIN.h"

#define LIBNAME "gm3000_pkcs11.dll"

#define NEWLINE "\r\n"
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
				wsprintf(szTemp, "%02X", pBuf[dwSize - dwRest + k+ j * dwByte]);
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

CString parse_bool_val(CK_ATTRIBUTE& attr)
{
	BOOL b = ((BOOL*)(attr.pValue))[0];
	if(b)
		return "TRUE";
	else
		return "FALSE";
}

CString parse_bin_val(CK_ATTRIBUTE& attr)
{
	return nByteToStr(attr.ulValueLen, attr.pValue, 1, 16);
}

CString parse_string_val(CK_ATTRIBUTE& attr)
{
	if('\0' != ((char*)attr.pValue)[attr.ulValueLen])
		return "[!! NOT STRING !!]";
	char* sz = (char*)attr.pValue;
	if(0 == lstrlen(sz))
		return "[!! NULL !!]";
	return sz;
}

bool parse_object(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CString& strRet)
{
	strRet = NEWLINE;
	for(ULONG i = 0; i < ulCount; ++i)
	{
		CK_ATTRIBUTE& attr = pTemplate[i];

		switch(attr.type)
		{
		case CKA_TOKEN:				strRet += "CKA_TOKEN             : "; break;
		case CKA_PRIVATE:			strRet += "CKA_PRIVATE           : "; break;
		case CKA_TRUSTED:			strRet += "CKA_TRUSTED           : "; break;
		case CKA_SENSITIVE:			strRet += "CKA_SENSITIVE         : "; break;
		case CKA_ENCRYPT:			strRet += "CKA_ENCRYPT           : "; break;
		case CKA_DECRYPT:			strRet += "CKA_DECRYPT           : "; break;
		case CKA_WRAP:				strRet += "CKA_WRAP              : "; break;
		case CKA_UNWRAP:			strRet += "CKA_UNWRAP            : "; break;
		case CKA_SIGN:				strRet += "CKA_SIGN              : "; break;
		case CKA_SIGN_RECOVER:		strRet += "CKA_SIGN_RECOVER      : "; break;
		case CKA_VERIFY:			strRet += "CKA_VERIFY            : "; break;
		case CKA_VERIFY_RECOVER:	strRet += "CKA_VERIFY_RECOVER    : "; break;
		case CKA_DERIVE:			strRet += "CKA_DERIVE            : "; break;
		case CKA_EXTRACTABLE:		strRet += "CKA_EXTRACTABLE       : "; break;
		case CKA_LOCAL:				strRet += "CKA_LOCAL             : "; break;
		case CKA_NEVER_EXTRACTABLE:	strRet += "CKA_NEVER_EXTRACTABLE : "; break;
		case CKA_ALWAYS_SENSITIVE:	strRet += "CKA_ALWAYS_SENSITIVE  : "; break;
		case CKA_MODIFIABLE:		strRet += "CKA_MODIFIABLE        : "; break;
		case CKA_LABEL:				strRet += "CKA_LABEL             : "; break;
		case CKA_APPLICATION:		strRet += "CKA_APPLICATION       : "; break;
		case CKA_VALUE:				strRet += "CKA_VALUE             : "; break;
		case CKA_OBJECT_ID:			strRet += "CKA_OBJECT_ID         : "; break;
		case CKA_CERTIFICATE_TYPE:	strRet += "CKA_CERTIFICATE_TYPE  : "; break;
		case CKA_ISSUER:			strRet += "CKA_ISSUER            : "; break;
		case CKA_SERIAL_NUMBER:		strRet += "CKA_SERIAL_NUMBER     : "; break;
		case CKA_AC_ISSUER:			strRet += "CKA_AC_ISSUER         : "; break;
		case CKA_OWNER:				strRet += "CKA_OWNER             : "; break;
		case CKA_ATTR_TYPES:		strRet += "CKA_ATTR_TYPES        : "; break;
		case CKA_KEY_TYPE:			strRet += "CKA_KEY_TYPE          : "; break;
		case CKA_SUBJECT:			strRet += "CKA_SUBJECT           : "; break;
		case CKA_ID:				strRet += "CKA_ID                : "; break;
		case CKA_START_DATE:		strRet += "CKA_START_DATE        : "; break;
		case CKA_END_DATE:			strRet += "CKA_END_DATE          : "; break;
		case CKA_MODULUS:			strRet += "CKA_MODULUS           : "; break;
		case CKA_MODULUS_BITS:		strRet += "CKA_MODULUS_BITS      : "; break;
		case CKA_PUBLIC_EXPONENT:	strRet += "CKA_PUBLIC_EXPONENT   : "; break;
		case CKA_PRIVATE_EXPONENT:	strRet += "CKA_PRIVATE_EXPONENT  : "; break;
		case CKA_PRIME_1:			strRet += "CKA_PRIME_1           : "; break;
		case CKA_PRIME_2:			strRet += "CKA_PRIME_2           : "; break;
		case CKA_EXPONENT_1:		strRet += "CKA_EXPONENT_1        : "; break;
		case CKA_EXPONENT_2:		strRet += "CKA_EXPONENT_2        : "; break;
		case CKA_COEFFICIENT:		strRet += "CKA_COEFFICIENT       : "; break;
		case CKA_PRIME:				strRet += "CKA_PRIME             : "; break;
		case CKA_SUBPRIME:			strRet += "CKA_SUBPRIME          : "; break;
		case CKA_BASE:				strRet += "CKA_BASE              : "; break;
		case CKA_PRIME_BITS:		strRet += "CKA_PRIME_BITS        : "; break;
		case CKA_SUBPRIME_BITS:		strRet += "CKA_SUBPRIME_BITS (or CKA_SUB_PRIME_BITS): "; break;
		case CKA_VALUE_BITS:		strRet += "CKA_VALUE_BITS        : "; break;
		case CKA_VALUE_LEN:			strRet += "CKA_VALUE_LEN         : "; break;
		case CKA_KEY_GEN_MECHANISM:	strRet += "CKA_KEY_GEN_MECHANISM : "; break;
		case CKA_ECDSA_PARAMS:		strRet += "CKA_ECDSA_PARAMS (or CKA_EC_PARAMS): "; break;
		case CKA_EC_POINT:			strRet += "CKA_EC_POINT          : "; break;
		case CKA_SECONDARY_AUTH:	strRet += "CKA_SECONDARY_AUTH    : "; break;
		case CKA_AUTH_PIN_FLAGS:	strRet += "CKA_AUTH_PIN_FLAGS    : "; break;
		case CKA_HW_FEATURE_TYPE:	strRet += "CKA_HW_FEATURE_TYPE   : "; break;
		case CKA_RESET_ON_INIT:		strRet += "CKA_RESET_ON_INIT     : "; break;
		case CKA_HAS_RESET:			strRet += "CKA_HAS_RESET         : "; break;
		case CKA_CLASS: 			strRet += "CKA_CLASS             : "; break;
		default: {char szTemp[60] = {0}; sprintf(szTemp, "Unknown Attribute Type: 0x%08X."NEWLINE, attr.type); strRet += szTemp;}
		}

		switch(attr.type)
		{
//============================================================================
// boolean:

		case CKA_TOKEN:
		case CKA_PRIVATE:
		case CKA_TRUSTED:
		case CKA_SENSITIVE:
		case CKA_ENCRYPT:
		case CKA_DECRYPT:
		case CKA_WRAP:
		case CKA_UNWRAP:
		case CKA_SIGN:
		case CKA_SIGN_RECOVER:
		case CKA_VERIFY:
		case CKA_VERIFY_RECOVER:
		case CKA_DERIVE:
		case CKA_EXTRACTABLE:
		case CKA_LOCAL:
		case CKA_NEVER_EXTRACTABLE:
		case CKA_ALWAYS_SENSITIVE:
		case CKA_MODIFIABLE:
			{
				strRet += parse_bool_val(attr);
				strRet += NEWLINE;
			}
			break;


//============================================================================
// string.
		case CKA_LABEL:
		case CKA_APPLICATION:
			{
				strRet += parse_string_val(attr);
				strRet += NEWLINE;
			}
			break;

//============================================================================
// binary.
		case CKA_VALUE:
		case CKA_OBJECT_ID:
		case CKA_AC_ISSUER:
		case CKA_OWNER:
		case CKA_SERIAL_NUMBER:
		case CKA_ID:
		case CKA_MODULUS:
		case CKA_MODULUS_BITS:
		case CKA_PUBLIC_EXPONENT:
		case CKA_PRIVATE_EXPONENT:
			{
				strRet += NEWLINE;
				strRet += parse_bin_val(attr);
				strRet += NEWLINE;
			}
			break;


		case CKA_ATTR_TYPES:
		case CKA_START_DATE:
		case CKA_END_DATE:
		case CKA_BASE:
		case CKA_VALUE_LEN:
		case CKA_KEY_GEN_MECHANISM:
		case CKA_ECDSA_PARAMS:
		case CKA_EC_POINT:
		case CKA_SECONDARY_AUTH:
		case CKA_AUTH_PIN_FLAGS:
		case CKA_HW_FEATURE_TYPE:
		case CKA_RESET_ON_INIT:
		case CKA_HAS_RESET:
		case CKA_PRIME_1:
		case CKA_PRIME_2:
		case CKA_EXPONENT_1:
		case CKA_EXPONENT_2:
		case CKA_COEFFICIENT:
		case CKA_PRIME:
		case CKA_SUBPRIME:
		case CKA_PRIME_BITS:
		case CKA_SUBPRIME_BITS:
		case CKA_VALUE_BITS:
			strRet += "???"NEWLINE;
			break;

		case CKA_CLASS:
			{
				CK_ULONG ulClass = ((CK_ULONG*)(attr.pValue)) [0];
				switch( ulClass )
				{
				case CKO_DATA:
					strRet += "CKO_DATA"NEWLINE;
					break;
				case CKO_CERTIFICATE:
					strRet += "CKO_CERTIFICATE"NEWLINE;
					break;
				case CKO_PUBLIC_KEY:
					strRet += "CKO_PUBLIC_KEY"NEWLINE;
					break;
				case CKO_PRIVATE_KEY:
					strRet += "CKO_PRIVATE_KEY"NEWLINE;
					break;
				case CKO_SECRET_KEY:
					strRet += "CKO_SECRET_KEY"NEWLINE;
					break;
				case CKO_HW_FEATURE:
					strRet += "CKO_HW_FEATURE"NEWLINE;
					break;
				case CKO_DOMAIN_PARAMETERS:
					strRet += "CKO_DOMAIN_PARAMETERS"NEWLINE;
					break;
				default:
					{
						char szTemp[60] = {0};
						sprintf(szTemp, "Unknown Object Class: 0x%08X."NEWLINE, ulClass);
						strRet += szTemp;
					}
					break;
				}
			}
			break;

		case CKA_CERTIFICATE_TYPE:
			{
				CK_ULONG ulType = ((CK_ULONG*)(attr.pValue)) [0];
				switch(ulType)
				{
				case CKC_X_509:
					strRet += "CKC_X_509"NEWLINE;
					break;
				case CKC_X_509_ATTR_CERT:
					strRet += "CKC_X_509_ATTR_CERT"NEWLINE;
					break;
				default:
					{
						char szTemp[60] = {0};
						sprintf(szTemp, "Unknown Certificate Type: 0x%08X."NEWLINE, ulType);
						strRet += szTemp;
					}
					break;
				}
			}
			break;


		case CKA_KEY_TYPE:
			{
				CK_ULONG ulType = ((CK_ULONG*)(attr.pValue)) [0];
				switch( ulType )
				{
				case CKK_RSA:
					strRet += "CKK_RSA"NEWLINE;
					break;
				case CKK_DSA:
					strRet += "CKK_DSA"NEWLINE;
					break;
				case CKK_DH:
					strRet += "CKK_DH"NEWLINE;
					break;
				case CKK_ECDSA:
					strRet += "CKK_ECDSA (or CKK_EC)"NEWLINE;
					break;
				case CKK_X9_42_DH:
					strRet += "CKK_X9_42_DH"NEWLINE;
					break;
				case CKK_KEA:
					strRet += "CKK_KEA"NEWLINE;
					break;
				case CKK_GENERIC_SECRET:
					strRet += "CKK_GENERIC_SECRET"NEWLINE;
					break;
				case CKK_RC2:
					strRet += "CKK_RC2"NEWLINE;
					break;
				case CKK_RC4:
					strRet += "CKK_RC4"NEWLINE;
					break;
				case CKK_DES:
					strRet += "CKK_DES"NEWLINE;
					break;
				case CKK_DES2:
					strRet += "CKK_DES2"NEWLINE;
					break;
				case CKK_DES3:
					strRet += "CKK_DES3"NEWLINE;
					break;
				case CKK_CAST:
					strRet += "CKK_CAST"NEWLINE;
					break;
				case CKK_CAST3:
					strRet += "CKK_CAST3"NEWLINE;
					break;
				case CKK_CAST5:
					strRet += "CKK_CAST5 (or CKK_CAST128)"NEWLINE;
					break;
				case CKK_RC5:
					strRet += "CKK_RC5"NEWLINE;
					break;
				case CKK_IDEA:
					strRet += "CKK_IDEA"NEWLINE;
					break;
				case CKK_SKIPJACK:
					strRet += "CKK_SKIPJACK"NEWLINE;
					break;
				case CKK_BATON:
					strRet += "CKK_BATON"NEWLINE;
					break;
				case CKK_JUNIPER:
					strRet += "CKK_JUNIPER"NEWLINE;
					break;
				case CKK_CDMF:
					strRet += "CKK_CDMF"NEWLINE;
					break;
				case CKK_AES:
					strRet += "CKK_AES"NEWLINE;
					break;
				default:
					{
						char szTemp[60] = {0};
						sprintf(szTemp, "Unknown Key Type: 0x%08X."NEWLINE, ulType);
						strRet += szTemp;
					}
					break;
				}
			}
			break;



		case CKA_ISSUER:
		case CKA_SUBJECT:
			{
				if(0 == attr.ulValueLen)
				{
					strRet += "[!! NULL !!]"NEWLINE;
					continue;
				}

				CString strsubject;
				CERT_NAME_BLOB  SubName;
				SubName.cbData = attr.ulValueLen;
				SubName.pbData = (BYTE*)attr.pValue;

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
					//AfxMessageBox("error get Subject Name length");
					//return;
					strRet += "[!! ERROR !!]"NEWLINE;
					continue;
				}

				char* pszTemp = NULL;
				if(!(pszTemp = new char[dwName]))
				{
					//AfxMessageBox("malloc memory  for subject name fail");
					//return;
					strRet += "[!! ERROR !!]"NEWLINE;
					continue;
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
				if (dwName > 1)
				{
					strRet += pszTemp;
					strRet += NEWLINE;
				}
				else
					strRet += "[!! ERROR !!]"NEWLINE;
				delete[] pszTemp;
			}
			break;

		}

	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CEnumObjDlg dialog

CEnumObjDlg::CEnumObjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnumObjDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnumObjDlg)
	m_strInfo = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_pSlotList = NULL_PTR;
	m_pApplication = new char[255];
	ZeroMemory(m_pApplication, 255);
	lstrcpy((char*)m_pApplication, "Enum Object App");
	m_hSession = NULL_PTR;
}

CEnumObjDlg::~CEnumObjDlg()
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


void CEnumObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnumObjDlg)
	DDX_Control(pDX, IDC_BTN_SECRET, m_btnSecret);
	DDX_Control(pDX, IDC_BTN_PUBLIC, m_btnPublic);
	DDX_Control(pDX, IDC_BTN_PRIVATE, m_btnPrivate);
	DDX_Control(pDX, IDC_BTN_DATA, m_btnData);
	DDX_Control(pDX, IDC_BTN_LOGIN, m_btnLogin);
	DDX_Control(pDX, IDC_BTN_ENUM, m_btnEnum);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_INFO, m_edtInfo);
	DDX_Text(pDX, IDC_INFO, m_strInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEnumObjDlg, CDialog)
	//{{AFX_MSG_MAP(CEnumObjDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_CLEARINFO, OnBtnClearinfo)
	ON_BN_CLICKED(IDC_BTN_ENUM, OnBtnEnum)
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBtnLogin)
	ON_BN_CLICKED(IDC_BTN_DATA, OnBtnData)
	ON_BN_CLICKED(IDC_BTN_PUBLIC, OnBtnPublic)
	ON_BN_CLICKED(IDC_BTN_PRIVATE, OnBtnPrivate)
	ON_BN_CLICKED(IDC_BTN_SECRET, OnBtnSecret)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnumObjDlg message handlers

BOOL CEnumObjDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here	
	ShowMsg("First, you need connect to token!"NEWLINE);
	m_btnLogin.EnableWindow(FALSE);
	m_btnEnum.EnableWindow(FALSE);
	m_btnData.EnableWindow(FALSE);
	m_btnPublic.EnableWindow(FALSE);
	m_btnPrivate.EnableWindow(FALSE);
	m_btnSecret.EnableWindow(FALSE);


	if( 0 != Pkcs11Initialize(LIBNAME))
	{
		AfxMessageBox("Failed to init pkcs11");
		return FALSE;
	}
	
	m_pPkcs11 = GetPkcs11Wrapper();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEnumObjDlg::OnPaint() 
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
HCURSOR CEnumObjDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEnumObjDlg::OnBtnConnect() 
{
	// TODO: Add your control notification handler code here
	if(m_hSession)
		return;
	
	::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	
	StartOP();
	
	CK_RV rv;
	CK_ULONG ulCount = 0;
	rv = m_pPkcs11->C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if(CKR_OK != rv )
	{
		ShowErr(NEWLINE"Can't acquire the information of token, ErrorCode: 0x%08X."NEWLINE, rv);
		return;
	}
	if(0 >= ulCount)
	{
		ShowMsg(NEWLINE"Can't connect to token, make sure one USB Token has been inserted."NEWLINE);
		return;
	}
	
	m_pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
	if (! m_pSlotList) 
	{
		ShowMsg(NEWLINE"Can't allocate enough memory!"NEWLINE);
		return;
	}
	
	rv = m_pPkcs11->C_GetSlotList(TRUE, m_pSlotList, &ulCount);
	if(CKR_OK != rv )
	{
		ShowErr(NEWLINE"Can't acquire the information of token, ErrorCode: 0x%08X."NEWLINE, rv);
		return;
	}
	if(0 >= ulCount)
	{
		ShowMsg(NEWLINE"Can't connect to token, make sure one USB Token has been inserted."NEWLINE);
		return;
	}
	
	rv = m_pPkcs11->C_OpenSession(
		m_pSlotList[0],  CKF_RW_SESSION | CKF_SERIAL_SESSION,
		&m_pApplication, NULL_PTR, &m_hSession);
	if(CKR_OK != rv )
	{
		ShowErr(NEWLINE"Can't Connect to token, Error Code 0x%08X."NEWLINE, rv);
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
	else
	{
		ShowMsg(NEWLINE"Connect to token Successfully!"NEWLINE);
		m_btnConnect.EnableWindow(FALSE);
		ShowMsg(NEWLINE"Now You need to login with User PIN!"NEWLINE);
		m_btnLogin.EnableWindow(TRUE);

	}
	
}

void CEnumObjDlg::ShowMsg(CString strInfo)
{
	m_strInfo += strInfo;
	UpdateData(FALSE);
	
	int nLastLine = m_edtInfo.GetLineCount();// GetFirstVisibleLine();
	
	if (nLastLine > 0)
	{
		m_edtInfo.LineScroll(nLastLine, 0);
	}
}

void CEnumObjDlg::StartOP()
{
	ShowMsg(NEWLINE"================================================");
}

void CEnumObjDlg::ShowErr(CString strInfo, CK_RV rv)
{
	CString strTemp("");
	strTemp.Format(strInfo.GetBuffer(strInfo.GetLength()), rv);
	ShowMsg(strTemp);
}

void CEnumObjDlg::OnBtnClearinfo() 
{
	// TODO: Add your control notification handler code here
	m_strInfo = "";
	UpdateData(FALSE);
}

void CEnumObjDlg::OnBtnEnum() 
{
	
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
	int numObj=0;//object numbers
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

		numObj++;
		//CString strvale = (char*)pAttrTemp[2].pValue;
/*
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
			AfxMessageBox("error get Subject Name length");
			return;
		}
		
		char* pszTemp = NULL;
		if(!(pszTemp = new char[dwName]))
		{
			AfxMessageBox("malloc memory  for subject name fail");
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
*/
//		CString strckaid=(char*)pAttrTemp[4].pValue;
		
		ShowMsg(NEWLINE);
		StartOP();
		CString strShow;
		parse_object(pAttrTemp, 6, strShow);
		ShowMsg(strShow);
/*
		ShowMsg(NEWLINE"Begin this Object's Output:"NEWLINE);
		ShowMsg("The Attribute CKA_CLASS of this Obj is:: CKO_CERTIFICATE"NEWLINE);
		
		if(*(int*)pAttrTemp[1].pValue==CKC_X_509)
		{
		ShowMsg("The Attribute CKA_CERTIFICATE_TYPE is: CKC_X_509"NEWLINE);
		}
		else 
			if(*(int*)pAttrTemp[1].pValue==CKC_X_509_ATTR_CERT)
			{
				ShowMsg("CKA_CERTIFICATE_TYPE is CKC_X_509_ATTR_CERT"NEWLINE);
			}
		ShowMsg("The Attribute CKA_LABEL of this Obj is: ");
		ShowMsg(strvale);
		ShowMsg(NEWLINE"The Attribute CKA_SUBJECT of this Obj is: ");
		ShowMsg(strsubject);
		ShowMsg(NEWLINE"The Attribute CKA_ID of this Obj is: ");
		ShowMsg(strckaid);
		ShowMsg(NEWLINE"The Content of this Obj(CKA_VALUE) is:"NEWLINE);
		ShowMsg(nByteToStr(pAttrTemp[5].ulValueLen, pAttrTemp[5].pValue, 1, 16));
		ShowMsg(NEWLINE"Finish Output Obj"NEWLINE);
*/

		delete[] pAttrTemp[0].pValue;
		delete[] pAttrTemp[1].pValue;
		delete[] pAttrTemp[2].pValue;
		delete[] pAttrTemp[3].pValue;
		delete[] pAttrTemp[4].pValue;
		delete[] pAttrTemp[5].pValue;
		
	}while(true);
	if(numObj==0)
	{
		StartOP();
		ShowMsg(NEWLINE"No certificate object."NEWLINE);
	}
	else
	{
		StartOP();
		CHAR strNumObj[4];
		wsprintf(strNumObj,"%d",numObj);
		ShowMsg(NEWLINE);
		ShowMsg(strNumObj);
		ShowMsg(" certificate object(s) was found."NEWLINE);
	}
}

void CEnumObjDlg::OnBtnLogin() 
{
	// TODO: Add your control notification handler code here
	StartOP();
	CK_RV rv;
	CK_TOKEN_INFO tokenInfo = {0};
	rv = m_pPkcs11->C_GetTokenInfo(m_pSlotList[0], &tokenInfo);
	if (CKR_OK != rv)
	{
		ShowMsg(NEWLINE"Can not get token info!"NEWLINE);
		return;
	}

	DlgUserPIN dlg;
	dlg.DoModal();
	if("" == g_strUserPIN)
	{
		ShowMsg(NEWLINE"To enum all objects, you must input User PIN!"NEWLINE);
		return;
	}
	
	CK_ULONG ulPIN = g_strUserPIN.GetLength();
	CK_BYTE_PTR pPIN = (CK_BYTE_PTR)g_strUserPIN.GetBuffer(ulPIN);
	
	::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	rv = m_pPkcs11->C_Login(m_hSession, CKU_USER, pPIN, ulPIN);

	if(CKR_OK != rv)
	{
		ShowErr(NEWLINE"Can't login to token, ErrorCode: 0x%08X."NEWLINE, rv);
		if(rv==CKR_USER_ALREADY_LOGGED_IN)
			ShowErr(NEWLINE"You have already logged in!",NULL);
		return;
	}
	else
		ShowMsg(NEWLINE"Login to Token Successfully!"NEWLINE);

	m_btnEnum.EnableWindow(TRUE);
	m_btnData.EnableWindow(TRUE);
	m_btnPublic.EnableWindow(TRUE);
	m_btnPrivate.EnableWindow(TRUE);
	m_btnSecret.EnableWindow(TRUE);
}

void CEnumObjDlg::OnBtnData() 
{
	// TODO: Add your control notification handler code here
	CK_OBJECT_CLASS dataClass = CKO_DATA;
	BOOL IsToken=true;
	CK_ATTRIBUTE pTempl[] = 
	{
		{CKA_CLASS, &dataClass, sizeof(CKO_DATA)},
		{CKA_TOKEN, &IsToken, sizeof(true)}
	};
	
	m_pPkcs11->C_FindObjectsInit(m_hSession, pTempl, 2);
	
	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_RV ckrv = 0;

	int numObj=0;
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
			{CKA_LABEL, NULL, 0},
			{CKA_APPLICATION, NULL, 0},
			{CKA_VALUE, NULL,0},
		};

		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 4);
		if(ckrv != CKR_OK)
		{
			break;
		}
		
		pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
		pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen + 1];
		pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen + 1];
		pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen];
		ZeroMemory(pAttrTemp[0].pValue, pAttrTemp[0].ulValueLen);
		ZeroMemory(pAttrTemp[1].pValue, pAttrTemp[1].ulValueLen + 1);
		ZeroMemory(pAttrTemp[2].pValue, pAttrTemp[2].ulValueLen + 1);
		ZeroMemory(pAttrTemp[3].pValue, pAttrTemp[3].ulValueLen );
		

		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 4);
		if(ckrv != CKR_OK)
		{
			delete[] pAttrTemp[0].pValue;
			delete[] pAttrTemp[1].pValue;
			delete[] pAttrTemp[2].pValue;
			delete[] pAttrTemp[3].pValue;
			break;
		}
		numObj++;		
		int nClassTemp = *((int*)pAttrTemp[0].pValue);
		CString strvale = (char*)pAttrTemp[1].pValue;
		CString strApp= (char*)pAttrTemp[2].pValue;

		ShowMsg(NEWLINE);
		StartOP();
		CString strShow;
		parse_object(pAttrTemp, sizeof(pAttrTemp)/sizeof(CK_ATTRIBUTE), strShow);
		ShowMsg(strShow);

		delete[] pAttrTemp[0].pValue;
		delete[] pAttrTemp[1].pValue;
		delete[] pAttrTemp[2].pValue;
		delete[] pAttrTemp[3].pValue;		
	}while(true);

	m_pPkcs11->C_FindObjectsFinal(m_hSession);

	if(numObj==0)
	{
		StartOP();
		ShowMsg(NEWLINE"No data object."NEWLINE);
	}
	else
	{
		StartOP();
		CHAR strNumObj[4];
		wsprintf(strNumObj,"%d",numObj);
		ShowMsg(NEWLINE);
		ShowMsg(strNumObj);
		ShowMsg(" data object(s) was found."NEWLINE);
	}


}

void CEnumObjDlg::OnBtnPublic() 
{
	CK_OBJECT_CLASS dataClass = CKO_PUBLIC_KEY;
	BOOL IsToken=true;
	CK_ATTRIBUTE pTempl[] = 
	{
		{CKA_CLASS, &dataClass, sizeof(CKO_PUBLIC_KEY)},
		{CKA_TOKEN, &IsToken, sizeof(true)}
	};

	
	m_pPkcs11->C_FindObjectsInit(m_hSession, pTempl, 2);
	
	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_RV ckrv = 0;
	int numObj=0;
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
			{CKA_KEY_TYPE,NULL,0},
			{CKA_LABEL, NULL, 0},
			{CKA_WRAP,NULL,0},
			{CKA_ENCRYPT,NULL,0},
			{CKA_MODULUS,NULL,0},
			{CKA_PUBLIC_EXPONENT,NULL,0},
		};
		
		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 7);
		if(ckrv != CKR_OK)
		{
			break;
		}
		
		pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
		pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
		pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen+1];
		pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen];
		pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen];
		pAttrTemp[5].pValue = new char[pAttrTemp[5].ulValueLen];
		pAttrTemp[6].pValue = new char[pAttrTemp[6].ulValueLen];
		
		ZeroMemory(pAttrTemp[0].pValue, pAttrTemp[0].ulValueLen);
		ZeroMemory(pAttrTemp[1].pValue, pAttrTemp[1].ulValueLen);
		ZeroMemory(pAttrTemp[2].pValue, pAttrTemp[2].ulValueLen+1);	
		ZeroMemory(pAttrTemp[3].pValue, pAttrTemp[3].ulValueLen);
		ZeroMemory(pAttrTemp[4].pValue, pAttrTemp[4].ulValueLen);
		ZeroMemory(pAttrTemp[5].pValue, pAttrTemp[5].ulValueLen);
		ZeroMemory(pAttrTemp[6].pValue, pAttrTemp[6].ulValueLen);
		
		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 7);
		if(ckrv != CKR_OK)
		{
			delete[] pAttrTemp[0].pValue;
			delete[] pAttrTemp[1].pValue;
			delete[] pAttrTemp[2].pValue;
			delete[] pAttrTemp[3].pValue;
			delete[] pAttrTemp[4].pValue;
			delete[] pAttrTemp[5].pValue;
			delete[] pAttrTemp[6].pValue;
			break;
		}

		numObj++;

		ShowMsg(NEWLINE);
		StartOP();
		CString strShow;
		parse_object(pAttrTemp, 7, strShow);
		ShowMsg(strShow);

		delete[] pAttrTemp[0].pValue;
		delete[] pAttrTemp[1].pValue;
		delete[] pAttrTemp[2].pValue;
		delete[] pAttrTemp[3].pValue;
		delete[] pAttrTemp[4].pValue;
		delete[] pAttrTemp[5].pValue;
		delete[] pAttrTemp[6].pValue;
	}while(true);

	if(numObj==0)
	{
		StartOP();
		ShowMsg(NEWLINE"No public key object."NEWLINE);
	}
	else
	{
		StartOP();
		CHAR strNumObj[4];
		wsprintf(strNumObj,"%d",numObj);
		ShowMsg(NEWLINE);
		ShowMsg(strNumObj);
		ShowMsg(" public key object(s) was found."NEWLINE);
	}

}

void CEnumObjDlg::OnBtnPrivate() 
{
	// TODO: Add your control notification handler code here
	CK_OBJECT_CLASS dataClass = CKO_PRIVATE_KEY;
	BOOL IsToken=true;
	CK_ATTRIBUTE pTempl[] =
	{
		{CKA_CLASS, &dataClass, sizeof(CKO_PRIVATE_KEY)},
		{CKA_TOKEN, &IsToken, sizeof(true)}
	};
	
	
	m_pPkcs11->C_FindObjectsInit(m_hSession, pTempl, 2);
	
	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_RV ckrv = 0;
	int numObj=0;
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
			{CKA_KEY_TYPE,NULL,0},
			{CKA_LABEL, NULL, 0},
			{CKA_SUBJECT,NULL,0},
			{CKA_ID,NULL,0},
			{CKA_SENSITIVE,NULL,0},
			{CKA_DECRYPT,NULL,0},
			{CKA_SIGN,NULL,0},
			{CKA_MODULUS,NULL,0},
			{CKA_PUBLIC_EXPONENT,NULL,0},
		};
		
		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 10);
		if(ckrv != CKR_OK)
		{
			break;
		}
		
		pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
		pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
		pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen+1];
		pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen+1];
		pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen+1];
		pAttrTemp[5].pValue = new char[pAttrTemp[5].ulValueLen];
		pAttrTemp[6].pValue = new char[pAttrTemp[6].ulValueLen];
		pAttrTemp[7].pValue = new char[pAttrTemp[7].ulValueLen];
		pAttrTemp[8].pValue = new char[pAttrTemp[8].ulValueLen];
		pAttrTemp[9].pValue = new char[pAttrTemp[9].ulValueLen];
		
		ZeroMemory(pAttrTemp[0].pValue, pAttrTemp[0].ulValueLen);
		ZeroMemory(pAttrTemp[1].pValue, pAttrTemp[1].ulValueLen);
		ZeroMemory(pAttrTemp[2].pValue, pAttrTemp[2].ulValueLen+1);	
		ZeroMemory(pAttrTemp[3].pValue, pAttrTemp[3].ulValueLen+1);
		ZeroMemory(pAttrTemp[4].pValue, pAttrTemp[4].ulValueLen+1);
		ZeroMemory(pAttrTemp[5].pValue, pAttrTemp[5].ulValueLen);
		ZeroMemory(pAttrTemp[6].pValue, pAttrTemp[6].ulValueLen);
		ZeroMemory(pAttrTemp[7].pValue, pAttrTemp[7].ulValueLen);
		ZeroMemory(pAttrTemp[8].pValue, pAttrTemp[8].ulValueLen);
		ZeroMemory(pAttrTemp[9].pValue, pAttrTemp[9].ulValueLen);
		
		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 10);
		if(ckrv != CKR_OK)
		{
			delete[] pAttrTemp[0].pValue;
			delete[] pAttrTemp[1].pValue;
			delete[] pAttrTemp[2].pValue;
			delete[] pAttrTemp[3].pValue;
			delete[] pAttrTemp[4].pValue;
			delete[] pAttrTemp[5].pValue;
			delete[] pAttrTemp[6].pValue;
			delete[] pAttrTemp[7].pValue;
			delete[] pAttrTemp[8].pValue;
			delete[] pAttrTemp[9].pValue;
			break;
		}
		numObj++;
		
		CString strvale = (char*)pAttrTemp[2].pValue;
		CString strsubject=(char*)pAttrTemp[3].pValue;
		CString strckaid=(char*)pAttrTemp[4].pValue;
		
		ShowMsg(NEWLINE);
		StartOP();

		CString strShow;
		parse_object(pAttrTemp, 10, strShow);
		ShowMsg(strShow);

		delete[] pAttrTemp[0].pValue;
		delete[] pAttrTemp[1].pValue;
		delete[] pAttrTemp[2].pValue;
		delete[] pAttrTemp[3].pValue;
		delete[] pAttrTemp[4].pValue;
		delete[] pAttrTemp[5].pValue;
		delete[] pAttrTemp[6].pValue;
		delete[] pAttrTemp[7].pValue;
		delete[] pAttrTemp[8].pValue;
		delete[] pAttrTemp[9].pValue;

	}while(true);
	
	if(numObj==0)
	{
		StartOP();
		ShowMsg(NEWLINE"Can's find private key Obj"NEWLINE);
	}
	else
	{
		StartOP();
		CHAR strNumObj[4];
		wsprintf(strNumObj,"%d",numObj);
		ShowMsg(NEWLINE);
		ShowMsg(strNumObj);
		ShowMsg(" private key object(s) was found."NEWLINE);
	}
}

void CEnumObjDlg::OnBtnSecret() 
{
	// TODO: Add your control notification handler code here
	CK_OBJECT_CLASS dataClass = CKO_SECRET_KEY;
	BOOL IsToken=true;
	CK_ATTRIBUTE pTempl[] = 
	{
		{CKA_CLASS, &dataClass, sizeof(CKO_PUBLIC_KEY)},
		{CKA_TOKEN, &IsToken, sizeof(true)}
	};

	
	m_pPkcs11->C_FindObjectsInit(m_hSession, pTempl, 2);
	
	CK_OBJECT_HANDLE hCKObj;
	CK_ULONG ulRetCount = 0;
	CK_RV ckrv = 0;

	int numObj=0;

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
			{CKA_KEY_TYPE,NULL,0},
			{CKA_LABEL, NULL, 0},
			{CKA_DERIVE,NULL,0},
			{CKA_VALUE,NULL,0},
		};
		
		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 5);
		if(ckrv != CKR_OK)
		{
			break;
		}
		
		pAttrTemp[0].pValue = new char[pAttrTemp[0].ulValueLen];
		pAttrTemp[1].pValue = new char[pAttrTemp[1].ulValueLen];
		pAttrTemp[2].pValue = new char[pAttrTemp[2].ulValueLen+1];
		pAttrTemp[3].pValue = new char[pAttrTemp[3].ulValueLen];
		pAttrTemp[4].pValue = new char[pAttrTemp[4].ulValueLen];

		
		ZeroMemory(pAttrTemp[0].pValue, pAttrTemp[0].ulValueLen);
		ZeroMemory(pAttrTemp[1].pValue, pAttrTemp[1].ulValueLen);
		ZeroMemory(pAttrTemp[2].pValue, pAttrTemp[2].ulValueLen+1);	
		ZeroMemory(pAttrTemp[3].pValue, pAttrTemp[3].ulValueLen);
		ZeroMemory(pAttrTemp[4].pValue, pAttrTemp[4].ulValueLen);

		
		ckrv = m_pPkcs11->C_GetAttributeValue(m_hSession, hCKObj, pAttrTemp, 5);
		if(ckrv != CKR_OK)
		{
			delete[] pAttrTemp[0].pValue;
			delete[] pAttrTemp[1].pValue;
			delete[] pAttrTemp[2].pValue;
			delete[] pAttrTemp[3].pValue;
			delete[] pAttrTemp[4].pValue;
			break;
		}

		numObj++;

		ShowMsg(NEWLINE);
		StartOP();
		CString strShow;
		parse_object(pAttrTemp, 5, strShow);
		ShowMsg(strShow);

		delete[] pAttrTemp[0].pValue;
		delete[] pAttrTemp[1].pValue;
		delete[] pAttrTemp[2].pValue;
		delete[] pAttrTemp[3].pValue;
		delete[] pAttrTemp[4].pValue;
	}while(true);
	

	if(numObj==0)
	{
		StartOP();
		ShowMsg(NEWLINE"No secret key."NEWLINE);
	}
	else
	{
		StartOP();
		CHAR strNumObj[4];
		wsprintf(strNumObj,"%d",numObj);
		ShowMsg(NEWLINE);
		ShowMsg(strNumObj);
		ShowMsg(" secret key object(s) was found."NEWLINE);
	}
}
