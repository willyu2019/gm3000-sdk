#pragma once

#include <windows.h>

#define ID_EDITUSER WM_USER+1001
#define ID_EDITPWD WM_USER+1002
#define ID_BUTTIONSET WM_USER+103
#define ID_BUTTIONCANCLE WM_USER+104
#define ID_STATIC_PWD	WM_USER+105
#define WM_GETPIN		WM_USER+106

class CWin32Dlg
{
public:
	CWin32Dlg(void);
	~CWin32Dlg(void);

	int RegisterClass();

	HRESULT CreateInstance();

	void CenterWindow(HWND hwnd);

	HRESULT ProcessNextMessage();

	char *GetPin(){return m_szPin;}

private:
	char m_szPin[64];
};
