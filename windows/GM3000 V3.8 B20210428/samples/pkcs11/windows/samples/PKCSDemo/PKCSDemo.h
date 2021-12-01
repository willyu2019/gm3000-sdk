// PKCSDemo.h : main header file for the PKCSDEMO application
//

#if !defined(AFX_PKCSDEMO_H__0C11771C_402A_418C_B34F_43A019B8DDAD__INCLUDED_)
#define AFX_PKCSDEMO_H__0C11771C_402A_418C_B34F_43A019B8DDAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPKCSDemoApp:
// See PKCSDemo.cpp for the implementation of this class
//
#include "pkcs11lib.h"///////////////////////////////////////////////////////////////////////////////////////////////////

class CPKCSDemoApp : public CWinApp
{
public:
	CPKCSDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPKCSDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPKCSDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
	PKCS11Wraper*  m_pPkcs11;//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PKCSDEMO_H__0C11771C_402A_418C_B34F_43A019B8DDAD__INCLUDED_)
