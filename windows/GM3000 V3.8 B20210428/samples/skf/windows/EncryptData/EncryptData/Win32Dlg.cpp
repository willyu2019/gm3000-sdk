#include "Win32Dlg.h"

HWND _hWnd;	
HWND  _hWndButtonOK;
HWND  _hWndButtonCancle;
HWND  _hWndEditPIN;
HWND  _hWndStatic;

const TCHAR *g_wszClassName = TEXT("PinWindow");
LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CWin32Dlg::CWin32Dlg(void)
{
	memset(m_szPin, 0, sizeof(m_szPin));
}

CWin32Dlg::~CWin32Dlg(void)
{
}

int CWin32Dlg::RegisterClass()
{
	HRESULT hr = S_OK;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style            = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = _WndProc;
	wcex.cbClsExtra        = 0; 
	wcex.cbWndExtra        = 0;
	wcex.hIcon            = NULL;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName    = NULL;
	wcex.lpszClassName    = ::g_wszClassName;
	wcex.hIconSm        = NULL;
	HBRUSH   hBrush   =   CreateSolidBrush(RGB(240, 240, 240));
	wcex.hbrBackground =   hBrush;
	if (!RegisterClassEx(&wcex))
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
	}
	return hr;
}

void CWin32Dlg::CenterWindow(HWND hwnd)
{
	RECT    rect;
	LONG    dx, dy;
	LONG    dxParent, dyParent;
	LONG    Style;
	GetWindowRect(hwnd, &rect);

	dx = rect.right - rect.left;
	dy = rect.bottom - rect.top;
	Style = GetWindowLong(hwnd, GWL_STYLE);
	if ((Style & WS_CHILD) == 0) 
	{
		dxParent = GetSystemMetrics(SM_CXSCREEN);
		dyParent = GetSystemMetrics(SM_CYSCREEN);
	}
	else 
	{
		HWND    hwndParent;
		RECT    rectParent;

		hwndParent = GetParent(hwnd);
		if (hwndParent == NULL) 
		{
			hwndParent = GetDesktopWindow();
		}

		GetWindowRect(hwndParent, &rectParent);

		dxParent = rectParent.right - rectParent.left;
		dyParent = rectParent.bottom - rectParent.top;
	}
	rect.left = (dxParent - dx) / 2;
	rect.top  = (dyParent - dy) / 3;
	SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_NOSIZE);
	SetForegroundWindow(hwnd);
}

HRESULT CWin32Dlg::CreateInstance()
{
	HRESULT hr = S_OK;
	_hWnd = ::CreateWindowEx(WS_EX_TOPMOST, ::g_wszClassName, 0, 
		WS_DLGFRAME, 400, 400, 400, 210, NULL, NULL, NULL, NULL);
	if (_hWnd == NULL)
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
	}

	if (SUCCEEDED(hr))
	{

		_hWndStatic = ::CreateWindow("STATIC", "智能卡口令：", WS_CHILD | WS_VISIBLE |SS_LEFT, 
			10, 70, 100, 22,  _hWnd, (HMENU)ID_STATIC_PWD, NULL, NULL);
		if (_hWndStatic == NULL)
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}

		_hWndEditPIN = ::CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_PASSWORD | WS_BORDER, 
			115, 70, 220, 21, _hWnd, (HMENU)ID_EDITUSER, NULL, NULL);
		if (_hWndEditPIN == NULL)
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}

		_hWndButtonOK = ::CreateWindow("Button", "登录", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
			100, 140, 80, 22,  _hWnd, (HMENU)ID_BUTTIONSET, NULL, NULL);
		if (_hWndButtonOK == NULL)
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}

		_hWndButtonCancle = ::CreateWindow("Button", "取消", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
			200, 140, 80, 22,  _hWnd, (HMENU)ID_BUTTIONCANCLE, NULL, NULL);
		if (_hWndButtonCancle == NULL)
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}


		if (SUCCEEDED(hr))
		{
			CenterWindow(_hWnd);

			if (!::ShowWindow(_hWnd, SW_NORMAL))
			{
				hr = HRESULT_FROM_WIN32(::GetLastError());
			}

			if (SUCCEEDED(hr))
			{
				if (!::UpdateWindow(_hWnd))
				{
					hr = HRESULT_FROM_WIN32(::GetLastError());
				}
			}
		}
	}

	return hr;
}

HRESULT CWin32Dlg::ProcessNextMessage()
{
	MSG msg;
	(void) ::GetMessage(&(msg), _hWnd, 0, 0);
	(void) ::TranslateMessage(&(msg));
	(void) ::DispatchMessage(&(msg));

	switch (msg.message)
	{
	case 1: return FALSE;

	case WM_GETPIN:
		{
			memset(m_szPin, 0, sizeof(m_szPin));
			::SendMessage(_hWndEditPIN, WM_GETTEXT , 256, (WPARAM)m_szPin);
			::SendMessage(_hWnd, WM_CLOSE, 0, 0);
			if(strlen(m_szPin) > 0)
				return FALSE;
		}
		break;
	}
	return TRUE;
}

LRESULT CALLBACK _WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_DEVICECHANGE:
		::MessageBox(NULL, TEXT("Device change"), TEXT("Device change"), 0);
		break;

	case WM_COMMAND:
		if((HWND)lParam == _hWndButtonOK)
		{
			::PostMessage(_hWnd, WM_GETPIN, 0, 0);
		}
		if((HWND)lParam == _hWndButtonCancle)
		{
			exit(0);
		}
		break;
	case WM_CLOSE:
		::ShowWindow(hWnd, SW_HIDE);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}