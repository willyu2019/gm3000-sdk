#ifndef _SKFAPI_FINGER_H_
#define _SKFAPI_FINGER_H_

#include "skfapi.h"

#ifdef __cplusplus
extern "C" {
#endif

	#define SAR_FINGER_EXIST				0x0C000001	//指纹已存在，同样的指纹不能录入两次
	#define SAR_FINGER_NOT_EXIST			0x0C000002	//指纹不存在，指纹认证时返回
	#define SAR_FINGER_RECORD_FULL			0x0C000003	//指纹记录已满，无法录入新的指纹

	/*
	*	清除所有指纹，如果存在指纹记录将被删除， 需要设备认证权限, 用于初始化指纹或重新初始化指纹 -- 设备格式化时使用
	*	hDevHandle	[IN] 设备句柄
	*/
	ULONG DEVAPI SKF_CleanAllFingers(DEVHANDLE hDevHandle);

	/*
	*	初始化应用指纹记录 需要管理员权限，在创建应用后应通过该API初始化指纹记录 -- 创建应用时使用
	*	hApplication		[IN] 应用句柄
	*	ulUserRetryTimes	[IN] 
	*	ulAdminRetryTimes	[IN] 
	*	ulUserVerifyLevel	[IN]
	*	ulAdminVerifyLevel  [IN]
	*/
	ULONG DEVAPI SKF_InitializeFinger(HAPPLICATION hApplication, ULONG ulUserRetryTimes, ULONG ulAdminRetryTimes, ULONG ulUserVerifyLevel, ULONG ulAdminVerifyLevel);

	/*
	*	指纹注册初始化, 需要用户/管理员权限，管理员指纹只能在管理员权限下录入， 用户指纹可在用户或管理员权限下录入 -- 录入指纹时使用
	*   在指纹录入必须首先调用此函数进行初始化
	*	hApplication	[IN] 应用句柄
	*	ulPinType		[IN] 1：用户   0：管理员
	*   ulFingerID		[IN] 指纹编号 ( 1- 254), 注意 0 不能使用
	*/
	ULONG DEVAPI SKF_EnrollFingerInit(HAPPLICATION hApplication, ULONG  ulPINType, ULONG ulFingerID);

	/*
	*	指纹注册, 需要用户/管理员权限, 注册指定指纹ID -- 录入指纹时使用
	*	hApplication	[IN] 应用句柄
	*	ulPinType		[IN] 1：用户   0：管理员
	*   pulEnrollTimes	[OUT] 录入成功次数， 一般需要录入三次
	*	ulFingerId		[IN] 指纹ID
	*/
	ULONG DEVAPI SKF_EnrollFinger(HAPPLICATION hApplication, ULONG  ulPINType, ULONG ulFingerId,  ULONG * pulEnrollTimes);
	

	/*
	*	指纹验证初始化， 在指纹验证前必须首先调用此函数进行初始化 -- 验证指纹时使用
	*	hApplication	[IN] 应用句柄
	*	ulPinType		[IN] 1：用户   0：管理员
	*/	
	ULONG DEVAPI SKF_VerifyFingerInit(HAPPLICATION hApplication, ULONG  ulPINType);

	/*
	*	指纹验证		-- 验证指纹时使用
	*	hApplication	[IN] 应用句柄
	*	ulPinType		[IN] 1：用户   0：管理员
	*	pulFingerId		[OUT] 指纹ID
	*   pulRetryCount	[OUT] 指纹认证剩余次数
	*/
	ULONG DEVAPI SKF_VerifyFinger(HAPPLICATION hApplication, ULONG  ulPINType, ULONG * pulFingerId, ULONG *pulRetryCount);

	/*
	*	指纹测试 - 测试当前指纹ID -- 用于指纹测试， 先调用SKF_VerifyFingerInit启动测试
	*   需要用户/管理员权限，管理员指纹只能在管理员权限下测试， 用户指纹可在用户或管理员权限下测试
	*	hApplication	[IN] 应用句柄
	*	ulPinType		[IN] 1：用户   0：管理员
	*	pulFingerId		[OUT] 指纹ID
	*/
	ULONG DEVAPI SKF_TestFinger(HAPPLICATION hApplication, ULONG  ulPINType, ULONG * pulFingerId);

	/*
	*	删除指纹， 需要用户/管理员权限，删除的是指纹ID，并非指纹记录	-- 删除指定指纹
	*	hApplication	[IN] 应用句柄
	*	ulPinType		[IN] 1：用户   0：管理员
	*	ulFingerId		[IN] 指纹ID,   FF - 全部
	*/
	ULONG DEVAPI SKF_DeleteFinger(HAPPLICATION hApplication, ULONG  ulPINType, ULONG ulFingerId);

	/*
	*	获取指纹信息, 不需要权限	-- 获取当前应用存在的指纹
	*	hApplication	[IN] 应用句柄
	*	ulPinType		[IN] 1：用户   0：管理员
	*	pbEnable		[OUT] 是否允许使用
	*	pulVerifyLevel	[OUT] 验证级别
	*	pulRetryTimes	[OUT] 最大重试次数
	*	pulLeftTimes	[OUT] 剩余重试次数	
	*   pbFingerIDs		[OUT] 当前用户的指纹ID列表，如果为0，该记录无指纹
	*	pulIDLen		[OUT] 当前用户的指纹ID个数，目前固定为10
	*/
	ULONG DEVAPI SKF_GetFingerInfo(HAPPLICATION hApplication, ULONG  ulPINType, BOOL * pbEnable,
				ULONG * pulVerifyLevel, ULONG * pulRetryTimes, ULONG * pulLeftTimes, BYTE * pbFingerIDs, ULONG * pulIDLen);

	/*
	*	指纹解锁， 需要用户/管理员权限，管理员权限可解锁用户和管理员，用户只能解锁用户
	*	当指纹认证次错误次数超过最大次数时， 指纹认证锁定，需要通过解锁命令来解锁
	*	hApplication	[IN] 应用句柄
	*	ulPINType		[IN] 1：用户   0：管理员
	*/
	ULONG DEVAPI SKF_UnblockFinger(HAPPLICATION hApplication, ULONG  ulPINType);
	
	/*
	*	取消指纹录入
	*	
	*/
	ULONG DEVAPI SKF_CancelEnrollFinger(HAPPLICATION hApplication);

	/**
	*	指纹认证至结束
	*
	*/
	ULONG DEVAPI MKF_VerifyFinger(HAPPLICATION hApplication, ULONG  ulPINType, ULONG *pulRetryCount);

	//取消认证
	ULONG DEVAPI SKF_CancelVerifyFinger(HAPPLICATION hApplication);

	
	/*
	功能：该应用下指定用户是否有指纹录入
	参数：
			hApplication:应用句柄
			ulPINType: USER_TYPE, ADMIN_TYPE 
	返回值：
			TRUE: KEY中有指纹
			FALSE: 不支持指纹或未录入指纹
	*/
	BOOL DEVAPI MKF_HasFinger(HAPPLICATION hApplication, ULONG ulPINType);


#ifdef __cplusplus
}
#endif


#endif