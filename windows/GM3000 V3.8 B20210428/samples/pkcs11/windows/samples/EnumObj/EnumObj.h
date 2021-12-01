// EnumObj.h : main header file for the ENUMOBJ application
//

#if !defined(AFX_ENUMOBJ_H__6294CE6A_B83A_4515_9D71_BAAD6B042AEB__INCLUDED_)
#define AFX_ENUMOBJ_H__6294CE6A_B83A_4515_9D71_BAAD6B042AEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "pkcs11lib.h"///////////

/////////////////////////////////////////////////////////////////////////////
// CEnumObjApp:
// See EnumObj.cpp for the implementation of this class
//

class CEnumObjApp : public CWinApp
{
public:
	CEnumObjApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnumObjApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEnumObjApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	private:
	
	PKCS11Wraper*  m_pPkcs11;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENUMOBJ_H__6294CE6A_B83A_4515_9D71_BAAD6B042AEB__INCLUDED_)
