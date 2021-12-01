#include "getinfos.h"

int main(int argc, char* argv[])
{
	CGetInfos inf; 
	if(!inf.LoadLib()) return 0;

	CK_SLOT_INFO slotInfo;
	unsigned long rv;

	CK_INFO cryptinfo;
	printf("Get Cryptoki information:\n");
	rv = inf.GetCryptokiInfos(&cryptinfo);
	inf.CheckRV(rv);
	inf.ShowCryptokiInfos(&cryptinfo);

	printf("Get Slot information:\n");
	rv = inf.GetSlotInfos(&slotInfo);
	inf.CheckRV(rv);
	printf("Get Token information:\n");
	CK_TOKEN_INFO tokeninfo;
	rv = inf.GetTokenInfos(&tokeninfo);
	inf.CheckRV(rv);

	return 0;
}

