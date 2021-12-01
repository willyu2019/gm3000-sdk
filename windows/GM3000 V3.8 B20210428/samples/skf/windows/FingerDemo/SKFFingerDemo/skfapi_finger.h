#ifndef _SKFAPI_FINGER_H_
#define _SKFAPI_FINGER_H_

#include "skfapi.h"

#ifdef __cplusplus
extern "C" {
#endif

	#define SAR_FINGER_EXIST				0x0C000001	//ָ���Ѵ��ڣ�ͬ����ָ�Ʋ���¼������
	#define SAR_FINGER_NOT_EXIST			0x0C000002	//ָ�Ʋ����ڣ�ָ����֤ʱ����
	#define SAR_FINGER_RECORD_FULL			0x0C000003	//ָ�Ƽ�¼�������޷�¼���µ�ָ��


	/*
	*	ָ��ע���ʼ��, ��Ҫ�û�/����ԱȨ�ޣ�����Աָ��ֻ���ڹ���ԱȨ����¼�룬 �û�ָ�ƿ����û������ԱȨ����¼�� -- ¼��ָ��ʱʹ��
	*   ��ָ��¼��������ȵ��ô˺������г�ʼ��
	*	hApplication	[IN] Ӧ�þ��
	*	ulPinType		[IN] 1���û�   0������Ա
	*   ulFingerID		[IN] ָ�Ʊ�� ( 1- 254), ע�� 0 ����ʹ��
	*/
	ULONG DEVAPI SKF_EnrollFingerInit(HAPPLICATION hApplication, ULONG  ulPINType, ULONG ulFingerID);

	/*
	*	ָ��ע��, ��Ҫ�û�/����ԱȨ��, ע��ָ��ָ��ID -- ¼��ָ��ʱʹ��
	*	hApplication	[IN] Ӧ�þ��
	*	ulPinType		[IN] 1���û�   0������Ա
	*   pulEnrollTimes	[OUT] ¼��ɹ������� һ����Ҫ¼������
	*	ulFingerId		[IN] ָ��ID
	*/
	ULONG DEVAPI SKF_EnrollFinger(HAPPLICATION hApplication, ULONG  ulPINType, ULONG ulFingerId,  ULONG * pulEnrollTimes);
	

	/*
	*	ָ����֤��ʼ���� ��ָ����֤ǰ�������ȵ��ô˺������г�ʼ�� -- ��ָ֤��ʱʹ��
	*	hApplication	[IN] Ӧ�þ��
	*	ulPinType		[IN] 1���û�   0������Ա
	*/	
	ULONG DEVAPI SKF_VerifyFingerInit(HAPPLICATION hApplication, ULONG  ulPINType);

	/*
	*	ָ����֤		-- ��ָ֤��ʱʹ��
	*	hApplication	[IN] Ӧ�þ��
	*	ulPinType		[IN] 1���û�   0������Ա
	*	pulFingerId		[OUT] ָ��ID
	*   pulRetryCount	[OUT] ָ����֤ʣ�����
	*/
	ULONG DEVAPI SKF_VerifyFinger(HAPPLICATION hApplication, ULONG  ulPINType, ULONG * pulFingerId, ULONG *pulRetryCount);

	/*
	*	ָ�Ʋ��� - ���Ե�ǰָ��ID -- ����ָ�Ʋ��ԣ� �ȵ���SKF_VerifyFingerInit��������
	*   ��Ҫ�û�/����ԱȨ�ޣ�����Աָ��ֻ���ڹ���ԱȨ���²��ԣ� �û�ָ�ƿ����û������ԱȨ���²���
	*	hApplication	[IN] Ӧ�þ��
	*	ulPinType		[IN] 1���û�   0������Ա
	*	pulFingerId		[OUT] ָ��ID
	*/
	ULONG DEVAPI SKF_TestFinger(HAPPLICATION hApplication, ULONG  ulPINType, ULONG * pulFingerId);

	/*
	*	ɾ��ָ�ƣ� ��Ҫ�û�/����ԱȨ�ޣ�ɾ������ָ��ID������ָ�Ƽ�¼	-- ɾ��ָ��ָ��
	*	hApplication	[IN] Ӧ�þ��
	*	ulPinType		[IN] 1���û�   0������Ա
	*	ulFingerId		[IN] ָ��ID,   FF - ȫ��
	*/
	ULONG DEVAPI SKF_DeleteFinger(HAPPLICATION hApplication, ULONG  ulPINType, ULONG ulFingerId);

	/*
	*	��ȡָ����Ϣ, ����ҪȨ��	-- ��ȡ��ǰӦ�ô��ڵ�ָ��
	*	hApplication	[IN] Ӧ�þ��
	*	ulPinType		[IN] 1���û�   0������Ա
	*	pbEnable		[OUT] �Ƿ�����ʹ��
	*	pulVerifyLevel	[OUT] ��֤����
	*	pulRetryTimes	[OUT] ������Դ���
	*	pulLeftTimes	[OUT] ʣ�����Դ���	
	*   pbFingerIDs		[OUT] ��ǰ�û���ָ��ID�б������Ϊ0���ü�¼��ָ��
	*	pulIDLen		[OUT] ��ǰ�û���ָ��ID������Ŀǰ�̶�Ϊ10
	*/
	ULONG DEVAPI SKF_GetFingerInfo(HAPPLICATION hApplication, ULONG  ulPINType, BOOL * pbEnable,
				ULONG * pulVerifyLevel, ULONG * pulRetryTimes, ULONG * pulLeftTimes, BYTE * pbFingerIDs, ULONG * pulIDLen);

	/*
	*	ָ�ƽ����� ��Ҫ�û�/����ԱȨ�ޣ�����ԱȨ�޿ɽ����û��͹���Ա���û�ֻ�ܽ����û�
	*	��ָ����֤�δ����������������ʱ�� ָ����֤��������Ҫͨ����������������
	*	hApplication	[IN] Ӧ�þ��
	*	ulPINType		[IN] 1���û�   0������Ա
	*/
	ULONG DEVAPI SKF_UnblockFinger(HAPPLICATION hApplication, ULONG  ulPINType);
	

	/*
	*	ȡ��ָ��¼��
	*	
	*/
	ULONG DEVAPI SKF_CancelEnrollFinger(HAPPLICATION hApplication);



	//ȡ����֤
	ULONG DEVAPI SKF_CancelVerifyFinger(HAPPLICATION hApplication);

	
#ifdef __cplusplus
}
#endif


#endif