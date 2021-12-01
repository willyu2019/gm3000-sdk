// SKFFingerDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <stdafx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


#include "skfapi.h"
#include "skfapi_finger.h"


#pragma comment(lib, "mtoken_gm3000.lib")



void display()
{
	fflush(stdin);
	fflush(stdout);
	printf("*******************************************************************\n");
	printf("                  Finger  ʾ��                                 \n");
	printf("*******************************************************************\n");
	printf("  1. SKF_VerifyPIN PIN���¼   2. ָ��¼��                         \n");
	printf("  3. ��ָ֤��                  4. ɾ��ָ��						   \n");
	printf("  5. ָ��ѧϰ                  6. ����ָ��						   \n");
	printf("  7. ��ȡָ����Ϣ              8. SKF_VerifyPINָ�Ƶ�¼            \n");
	printf("  0. EXIT                      \n");
	printf("\n Select -->");
}

long EnunDevFun(HANDLE *hApplication)
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

	BOOL pbEnable = false;				//�Ƿ�����ʹ�ã�������
	ULONG pulVerifyLevel = 0;			//[OUT] ��֤����
	ULONG	pulRetryTimes = 0;			//[OUT] ������Դ���
	ULONG	pulLeftTimes = 0;			//[OUT] ʣ�����Դ���
	BYTE   pbFingerIDs[128] = { 0 };	//[OUT] ��ǰ�û���ָ��ID�б����Ϊ0���ü�¼��ָ��
	ULONG	pulIDLen = 128;				//[OUT] ��ǰ�û���ָ��ID������Ŀǰ�̶�Ϊ10

	ulRslt = SKF_GetFingerInfo(hApplication, USER_TYPE, &pbEnable, &pulVerifyLevel, &pulRetryTimes, &pulLeftTimes, pbFingerIDs, &pulIDLen);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to Get Finger Info . \n");
		return 1;
	}

	ULONG ulFeatureId = 0;
	int i = 0;
	//ѭ���ҵ�δ��ռ�õ�ָ�Ʊ�ǣ����Ϊ1�ѱ�ռ�ø���0δ��ռ��
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

		Sleep(200);
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

	ULONG ulFeatureId = 0;
	ULONG ulRetry = 0;

	ULONG ulRslt = SAR_ACTION_WAITING;

	ulRslt = SKF_VerifyFingerInit(hApplication, USER_TYPE);
	if (ulRslt != SAR_OK)
	{
		printf("Failed to Verify Finger Init . \n");
		return 1;
	}

	ulRslt = SAR_ACTION_WAITING;
	while (ulRslt == SAR_ACTION_WAITING)
	{
		ulRslt = SKF_VerifyFinger(hApplication, USER_TYPE, &ulFeatureId, &ulRetry);
		Sleep(200);
	}

	if (ulRslt == SAR_OK)
	{
		printf("Verify Finger Successfully. \n");

	}
	else
	{
		printf("Failed to Verify Finger . \n");

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
			ulRslt = SKF_DeleteFinger(hApplication, USER_TYPE, i + 1);
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
	//�˺�������ָ��ѧϰ��ָ��ѧϰ����ָ֤�Ƹ�׼ȷ������ָ��ѧϰ5�����ϣ�ͬһ��ָ�ƴӲ�ͬ��ָ�ƽǶ�ѧϰ
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
			Sleep(200);
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
	//���ڽ����û�ָ�ƻ����ָ�ƣ�����������Ӧ��Ȩ��
	//ʾ���ǽ����û�ָ��
	//ADMIN_TYPE ����Աָ������

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

	char *userPin = "";//����Ϊ��ʱ��ָ֤��
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


int main(int argc, char* argv[])
{

	long ulRslt = 0;
	int chg = -1;
	HANDLE hApplication = NULL;

	ulRslt = EnunDevFun(&hApplication);
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
		case 0:
			break;
		}
	} while (chg != 0);

}

