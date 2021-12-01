#include "generic.h"
#include <iostream>
#include <iomanip>

using namespace std;

void ShowData(unsigned char * pData, unsigned long ulDataLen)
{
	if(pData == NULL)
	{
		cout << "Data to be showed is NULL!" << endl;
		return;
	}
	else
	{
		cout << "Data (" << ulDataLen << " bytes) in HEX:";
		cout.setf(std::ios::uppercase);
		cout << std::setfill('0') << std::setw(2);
		for(unsigned long i = 0; i < ulDataLen; ++i)
		{
			if(0 == i % 16)
				cout << endl;

			cout << std::setw(2) << hex << (DWORD)pData[i] << ' ';
		}
	}

	cout << dec << endl;

	return ;
}

void BeginAction(const string& strOperation)
{
	cout << strOperation;
}

#ifndef NERR_BASE
#	define NERR_BASE		2100
#	define MAX_NERR			NERR_BASE + 899
#endif

void ActionFailed(DWORD dwLastError)
{
	HMODULE hModule = NULL; // default to system source
	LPSTR MessageBuffer = NULL;
	DWORD dwBufferLength = 0;

	DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_FROM_SYSTEM ;

	if(dwLastError >= NERR_BASE && dwLastError <= MAX_NERR)
	{
		hModule = LoadLibraryEx("netmsg.dll", NULL, LOAD_LIBRARY_AS_DATAFILE);

		if(hModule != NULL)
		{
			dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
		}
	}

	if(dwBufferLength = FormatMessageA(dwFormatFlags, hModule,
		dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR) &MessageBuffer, 0, NULL))
	{
		cout.setf(std::ios::uppercase);
		//cout<< "ERROR: " << strInfo << endl
		cout<< "......[FAILED]." << endl
			<< "    [0x" << std::setfill('0') << std::setw(8) << hex
			<< dwLastError << "] "
			<< MessageBuffer << dec
			<< endl;

		LocalFree(MessageBuffer);
	}

	if(hModule != NULL)
	{
		FreeLibrary(hModule);
	}
}

void ActionSuccess(void)
{
	cout << "......[OK]" << endl;
}

void ShowSysError(const string& strInfo, DWORD dwLastError)
{
	HMODULE hModule = NULL; // default to system source
	LPSTR MessageBuffer = NULL;
	DWORD dwBufferLength = 0;

	DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_FROM_SYSTEM ;

	if(dwLastError >= NERR_BASE && dwLastError <= MAX_NERR)
	{
		hModule = LoadLibraryEx("netmsg.dll", NULL, LOAD_LIBRARY_AS_DATAFILE);

		if(hModule != NULL)
		{
			dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
		}
	}

	if(dwBufferLength = FormatMessageA(dwFormatFlags, hModule,
		dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR) &MessageBuffer, 0, NULL))
	{
		cout.setf(std::ios::uppercase);
		cout<< strInfo << "......[FAILED]." << endl
			<< "    [0x" << std::setfill('0') << std::setw(8) << hex
			<< dwLastError << "] "
			<< MessageBuffer << dec
			<< endl;

		LocalFree(MessageBuffer);
	}

	if(hModule != NULL)
	{
		FreeLibrary(hModule);
	}
}
