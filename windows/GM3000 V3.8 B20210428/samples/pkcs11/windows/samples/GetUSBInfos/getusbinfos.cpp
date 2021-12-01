/*
[]=========================================================================[]

FILE:
	getusbinfos.cpp

DESC:

[]=========================================================================[]
*/

#include "stdafx.h"
#include "getinfos.h"

int main(int argc, char* argv[])
{
	CGetInfos inf; 
	if(!inf.LoadLib())
		return 0;

	unsigned long rv;

	CK_INFO cryptinfo;
	rv = inf.GetCryptokiInfos(&cryptinfo);
	if(inf.CheckRV("Get cryptoki library information", rv))
		inf.ShowCryptokiInfos(&cryptinfo);

	CK_SLOT_INFO slotInfo;
	rv = inf.GetSlotInfos(&slotInfo);
	if(inf.CheckRV("Get slot information", rv))
		inf.ShowSlotInfo(&slotInfo);

	CK_TOKEN_INFO tokeninfo;
	rv = inf.GetTokenInfos(&tokeninfo);
	if(inf.CheckRV("Get token information", rv))
		inf.ShowTokenInfo(&tokeninfo);

	printf("Press any key to continue...\n");
	_getch();

	return 0;
}





















