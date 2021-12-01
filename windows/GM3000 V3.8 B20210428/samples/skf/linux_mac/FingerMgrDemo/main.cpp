// FingerDemo.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "skfapi.h"
#include "skfapi_finger.h"
//#include "verifyfinger_ui.h"
#include <dlfcn.h>
#include <pthread.h>

#define SGD_FINGER_STATUS			0x80080000

void display()
{
	//system("cls");
	fflush(stdin);
	fflush(stdout);
	printf("*******************************************************************\n");
	printf("                  Finger  示例                                     \n");
	printf("*******************************************************************\n");
	printf("  1. PIN码登录[SKF_VerifyPIN]	                                   \n");
	printf("  2. 指纹录入[SKF_EnrollFingerInit,SKF_EnrollFinger]               \n");
	printf("  3. 指纹验证[SKF_VerifyFinger带UI]		       	                   \n");
	printf("  4. 删除指纹[SKF_DeleteFinger]		                	           \n");	
	printf("  5. 指纹测试[SKF_TestFinger]			       	                   \n");
	printf("  6. 解锁指纹[SKF_UnblockFinger]			                       \n");	
	printf("  7. 获取指纹信息[SKF_GetFingerInfo]	                           \n");
	printf("  8. 指纹验证[SKF_VerifyPIN,UserPIN=""]                            \n");	
	printf("  9. 获取设备类型[SKF_GetDevInfo]			                       \n");
	printf("  10. 获指纹取录入个数[SKF_HasFinger,SKF_GetFingerCount]           \n");	
	printf("  0. EXIT                                                          \n");
	printf("\n Select -->");
}

long EnunDevFun(HANDLE *hDev, HANDLE *hApplication)
{
	ULONG ulRslt = 0;
	char szDevName[256];
	ULONG ulNamelen = sizeof(szDevName);

	memset(szDevName, 0, sizeof(szDevName));
	ulRslt = SKF_EnumDev(true, szDevName, &ulNamelen);
	if (ulRslt != SAR_OK || ulNamelen == 0)
	{
		printf("Failed to Enum Dev . \n");
		return 1;
	}

	HANDLE hDevHandle;
	ulRslt = SKF_ConnectDev(szDevName, &hDevHandle);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to Connect Dev . \n");
		return 2;
	}

	*hDev = hDevHandle;
	
	char szAppName[256] = { 0 };
	ULONG namesLen = sizeof(szAppName);
	ulRslt = SKF_EnumApplication(hDevHandle, szAppName, &namesLen);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to Enum Application . \n");
		return 3;
	}

	HANDLE hApp;
	ulRslt = SKF_OpenApplication(hDevHandle, szAppName, &hApp);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to Open Application . \n");
		return 3;
	}

	*hApplication = hApp;

	return 0;
};

long VerifyPIN(HAPPLICATION hApplication)
{
	ULONG ulRslt = 0;

	char *userPin = "12345678";
	ULONG retry = 0;
	ulRslt = SKF_VerifyPIN(hApplication, USER_TYPE, userPin, &retry);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to Verify PIN . \n");
		return 3;
	}

	printf("VerifyFun Successfully. \n");

	fflush(stdin);
	getchar();
	return ulRslt;
};


long EnrollFinger(HAPPLICATION hApplication)
{
	ULONG ulRslt = 0;

	BOOL pbEnable = false;				//是否允许使用（保留）
	ULONG pulVerifyLevel = 0;			//[OUT] 验证级别
	ULONG	pulRetryTimes = 0;			//[OUT] 最大重试次数
	ULONG	pulLeftTimes = 0;			//[OUT] 剩余重试次数
	BYTE   pbFingerIDs[128] = { 0 };	//[OUT] 当前用户的指纹ID列表，如果为0，该记录无指纹
	ULONG	pulIDLen = 128;				//[OUT] 当前用户的指纹ID个数，目前固定为10

	ulRslt = SKF_GetFingerInfo(hApplication, USER_TYPE, &pbEnable, &pulVerifyLevel, &pulRetryTimes, &pulLeftTimes, pbFingerIDs, &pulIDLen);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to Get Finger Info . \n");
		return 1;
	}

	ULONG ulFeatureId = 0;
	int i = 0;
	//循环找到未被占用的指纹标记，如果为1已被占用个，0未被占用
	for (i = 0; i<128; i++)
	{
		if (pbFingerIDs[i] == 0)
		{
			ulFeatureId = i + 1;

			break;
		}
	}

	ulRslt = SAR_ACTION_WAITING;
	ULONG ulEnrollTimes = 0;

	ulRslt = SKF_EnrollFingerInit(hApplication, USER_TYPE, ulFeatureId);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to Enroll Finger Init . \n");
		return 1;
	}


	printf("please Look at the fingerprint device, Input fingerprint when the light flickers. \n");
	ulRslt = SAR_ACTION_WAITING;
	while (ulRslt == SAR_ACTION_WAITING)
	{
		ulRslt = SKF_EnrollFinger(hApplication, USER_TYPE, ulFeatureId, &ulEnrollTimes);

		usleep(200);
	}

	if (ulRslt == SAR_OK)
	{
		printf("Enroll Finger Successfuly. \n");
	}
	else
	{
		printf("Failed to Enroll Finger. \n");
	}


	ulRslt = SKF_GetFingerInfo(hApplication, USER_TYPE, &pbEnable, &pulVerifyLevel, &pulRetryTimes, &pulLeftTimes, pbFingerIDs, &pulIDLen);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to Get Finger Info . \n");
		return 1;
	}

	fflush(stdin);
	getchar();
	return ulRslt;
};



long VerifyFinger(HAPPLICATION hApplication)
{
	ULONG ulRslt = 0;
 	ULONG ulRetryCount = 3;
	ULONG ulPopFingerUI = 1;
	
	ulRslt = SKF_VerifyFinger(hApplication, USER_TYPE, ulRetryCount, ulPopFingerUI);
	if (ulRslt ==  SAR_OK)
	{
		printf("Verify Finger Successfully. \n");
	}
	else
	{
		printf("Failed to Verify Finger ,Error code = %08x . \n", ulRslt);
	}
	fflush(stdin);
	getchar();
	return ulRslt;
}

long Deletefinger(HAPPLICATION hApplication)
{
	ULONG ulRslt = 0;

	BOOL bEnable;
	ULONG ulVerifyLevel;
	ULONG ulRetryTimes;
	ULONG ulLeftTimes;
	BYTE bFeatureIDs[128] = { 0 };
	ULONG ulIDLen = 128;
	ULONG FingerID = 0;

	ulRslt = SKF_GetFingerInfo(hApplication, USER_TYPE, &bEnable, &ulVerifyLevel, &ulRetryTimes, &ulLeftTimes, bFeatureIDs, &ulIDLen);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to get finger info. \n");
		return 1;
	}

	for (int i = 0; i < ulIDLen; i++)
	{
		if (bFeatureIDs[i] != 0)
		{
			FingerID = i +1;
			if (bFeatureIDs[i]>1)
			{
				FingerID = bFeatureIDs[i];
			}
			ulRslt = SKF_DeleteFinger(hApplication, USER_TYPE, FingerID);
			if (ulRslt != SAR_OK)
			{
				printf("Failed to Delete Finger, FeatureID = %d \n", i);

				break;
			}
			else
			{
				printf("Delete finger successfully, FeatureID = %d. \n", i);
			}
		}
	}

	fflush(stdin);
	getchar();
	return ulRslt;
}

long TestFinger(HAPPLICATION hApplication)
{
	//此函数用于指纹学习，指纹学习后验证指纹更准确，建议指纹学习5次以上，同一个指纹从不同的指纹角度学习
	for (int i = 0; i<5; i++)
	{
		printf("Prepare for fingerprint learning . \n\n");

		ULONG ulRslt = 0;
		ulRslt = SKF_VerifyFingerInit(hApplication, USER_TYPE);
		if (ulRslt != SAR_OK)
		{
			printf("Failed to Verify Finger Init . \n");
			return 1;
		}

		ULONG pulFingerId = 0;

		ulRslt = SAR_ACTION_WAITING;
		while (ulRslt == SAR_ACTION_WAITING)
		{
			ulRslt = SKF_TestFinger(hApplication, USER_TYPE, &pulFingerId);
			usleep(200);
		}

		if (ulRslt == SAR_OK)
		{
			printf("TestFinger Successfully . FingerId = %d. \n", pulFingerId);

		}
		else if (ulRslt == SAR_ACTION_CANCELED)
		{
			continue;
		}
		else
		{
			printf("Failed to Test Finger, FingerId = %d. \n", pulFingerId);

			break;
		}
	}

	fflush(stdin);
	getchar();
	return 0;
};

long UnblockFinger(HAPPLICATION hApplication)
{
	//用于解锁用户指纹或管理指纹，但必须有相应的权限
	//示例是解锁用户指纹
	//ADMIN_TYPE 管理员指纹类型

	ULONG ulRslt = 0;
	ulRslt = SKF_UnblockFinger(hApplication, USER_TYPE);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to Unblock Finger. \n");
	}
	else
	{
		printf("Unblock Finger successfully. \n");
	}

	fflush(stdin);
	getchar();
	return ulRslt;
}

long GetFingerInfo(HAPPLICATION hApplication)
{
	ULONG ulRslt = 0;
	BOOL bEnable;
	ULONG ulVerifyLevel;
	ULONG ulRetryTimes;
	ULONG ulLeftTimes;
	BYTE bFeatureIDs[128] = { 0 };
	ULONG ulIDLen = 128;

	ulRslt = SKF_GetFingerInfo(hApplication, USER_TYPE, &bEnable, &ulVerifyLevel, &ulRetryTimes, &ulLeftTimes, bFeatureIDs, &ulIDLen);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to get finger info. \n");
	}
	else
	{
		for (int i = 0; i<10; i++)
		{
			printf("FeatureIDs[%d] = [%d]  \n", i, bFeatureIDs[i]);
		}

	}
	fflush(stdin);
	getchar();
	return ulRslt;
}

long VerifyPINForFinger(HAPPLICATION hApplication)
{
	ULONG ulRslt = 0;

	char *userPin = "";//密码为空时验证指纹
	ULONG retry = 0;
	ulRslt = SKF_VerifyPIN(hApplication, USER_TYPE, userPin, &retry);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to Verify PIN . \n");
		return 3;
	}

	printf("VerifyFun Successfully. \n");

	fflush(stdin);
	getchar();
	return ulRslt;
};
long GetDevInfo(HANDLE hDev)
{
	long nRet = 0;
	long bFinger = 0;
	DEVINFO devInfo = {0};
	
	nRet = SKF_GetDevInfo (hDev, &devInfo);
	if (nRet != SAR_OK)
	{
		printf("Failed to SKF_GetDevInfo . \n");
		return 1;
	}
	printf("SKF_GetDevInfo Successfully. \n");
	
	bFinger = devInfo.AlgHashCap & SGD_FINGER_STATUS;
	if(bFinger == SGD_FINGER_STATUS)
	{
		printf("This is a Fingerprint device. \n");
	}
	else
	{
		printf("This is a common device. \n");
	}
}

long GetFingerCount(HAPPLICATION hApplication)
{
	long nRet = 0;
	ULONG FingerCount = 0;
	ULONG  PINType = 1;  //1用户指纹  0管理员指纹
	BOOL bRet = false;
	bRet = SKF_HasFinger(hApplication,PINType);
	if (!bRet)
	{
		printf("SKF_HasFinger False \n");
		return 1;		
	}
	
	nRet = SKF_GetFingerCount(hApplication, PINType, &FingerCount);
	if (nRet != SAR_OK)
	{
		printf("Failed to SKF_GetFingerCount ,ErrorCode= %0x. \n", nRet);
		return 1;
	}

	printf("Get FingerCount = %d. \n", FingerCount);
	return 0;
}

int main(int argc, char* argv[])
{

	long ulRslt = 0;
	int chg = -1;
	HANDLE hDev = NULL;
	HANDLE hApplication = NULL;

	ulRslt = EnunDevFun(&hDev, &hApplication);
	if (ulRslt != 0)
	{
		printf("Failed to EnunDevFun. \n");
		return 1;
	}

	do
	{
		display();
		scanf("%d", &chg);

		switch (chg)
		{
		case 1: VerifyPIN(hApplication);
			break;
		case 2: EnrollFinger(hApplication);
			break;
		case 3: VerifyFinger(hApplication);
			break;
		case 4: Deletefinger(hApplication);
			break;
		case 5: TestFinger(hApplication);
			break;
		case 6: UnblockFinger(hApplication);
			break;
		case 7: GetFingerInfo(hApplication);
			break;
		case 8:VerifyPINForFinger(hApplication);
			break;
		case 9:
			GetDevInfo(hDev);
			break;
		case 10:
			GetFingerCount(hApplication);
			break;

		case 0:
			break;
		}
	} while (chg != 0);

}

