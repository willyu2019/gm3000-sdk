// exportcert.h : main header file for the EXPORTCERT application
//

#if !defined(AFX_EXPORTCERT_H__4B09DD6F_3B2C_4B7A_8E63_C16BEB1C2BC2__INCLUDED_)
#define AFX_EXPORTCERT_H__4B09DD6F_3B2C_4B7A_8E63_C16BEB1C2BC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "pkcs11lib.h"///////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CExportcertApp:
// See exportcert.cpp for the implementation of this class
//

class CExportcertApp : public CWinApp
{
public:
	CExportcertApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportcertApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CExportcertApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	private:
	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTCERT_H__4B09DD6F_3B2C_4B7A_8E63_C16BEB1C2BC2__INCLUDED_)
