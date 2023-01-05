#pragma once
#include "CoreMinimal.h"
#include "AAUChinaConfigModel.generated.h"

//accout_type:0,实名类型，0-未实名游客 1：8岁以下 ，2：8-15岁，  3：16-17岁， 4：18+ 5：未实名正式用户
//type:type:1-宵禁剩余时间提示 2-时长剩余提示 3-单笔消费限制 4-月消费限制 5-已经处于宵禁提示 6-时长耗尽提示 7-非宵禁时段第一次登陆 8-非宵禁时段非第一次登陆,剩余时长大于0，9-非宵禁时段非第一次登陆,剩余时长小于等于0，10-宵禁时段，登陆成功 11-非第一次登录，且剩余时长＞0，且＜20min时 12-气泡剩余时长 13-气泡距离宵禁时间

/*
*        "child_protected_config": {
			"share_time_switch": 0,             // 分游戏计算时长，1:统一计算时长
			"use_time_switch": 1,               // 0:客户端时间 1:服务端时间
			"no_identify_time": 3600,           // 未实名用户娱乐时间，单位秒
			"charge_amount_switch": 1,          // 0:分游戏计算金额 1:统一计算金额
			"child_common_time": 5400,          // 娱乐时间，单位秒
			"child_holiday_time": 10800,        // 节假日娱乐时间，单位秒
			"night_strict_start": "22:00",      // 宵禁开始时间 | 格式:hh:mm | eg 20:00
			"night_strict_end": "08:00",        // 宵禁结束时间 | 格式:hh:mm | eg 08:00
			"night_strict_warn": 2400,          // 宵禁提前预警时间，单位秒
			"remain_time_warn": 1200,           // 剩余时间提前预警时间，单位秒
			"upload_all_data":0                 // 是否需要上传所有类型用户的数据，默认成年人不上传不计时
		},
 */

enum AAURealNameWordType {
	AAURealNameWordTypeQuickVerify = 0,
	AAURealNameWordTypeQuickVerifySuccess,
	AAURealNameWordTypeQuickVerifyFail,
	AAURealNameWordTypeVerify,
	AAURealNameWordTypeVerifying,
	AAURealNameWordTypeVerifyFail,
};
 
enum AAURealNameAccountType {
	AAURealNameAccountTypeUnrealNameVisitor = 0,	// 未实名游客
	AAURealNameAccountTypeChild,					// 8岁以下
	AAURealNameAccountTypeTeen,						// 8-15岁
	AAURealNameAccountTypeYoung,					// 16-17岁
	AAURealNameAccountTypeAdult,					// 18+
	AAURealNameAccountTypeUnrealNameUser,			// 未实名正式用户
};

enum AAUHealthTipType {
	AAUHealthTipTypeInCurfew = 5,				// 已经处于宵禁提示
	AAUHealthTipTypeTimeout = 6,				// 时长耗尽提示
	AAUHealthTipTypeFirstLogin = 7,				// 非宵禁时段第一次登陆(未成名，不过现在每次登录都会提示)
};

USTRUCT()
struct FAAUChinaChildProtectedConfig
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool share_time_switch = false;

	UPROPERTY()
	bool use_time_switch = false;

	UPROPERTY()
	int no_identify_time = 0;

	UPROPERTY()
	bool charge_amount_switch = false;

	UPROPERTY()
	int child_common_time = 0;

	UPROPERTY()
	int child_holiday_time = 0;

	UPROPERTY()
	FString night_strict_start;

	UPROPERTY()
	FString night_strict_end;

	UPROPERTY()
	int night_strict_warn = 0;

	UPROPERTY()
	int remain_time_warn = 0;
	
};

USTRUCT()
struct FAAUChinaPayLimitWord
{
	GENERATED_BODY()

	UPROPERTY()
	FString single_title;

	UPROPERTY()
	FString single_description;

	UPROPERTY()
	int single_limit = 0;

	UPROPERTY()
	int account_type = 0;

	UPROPERTY()
	FString month_title;

	UPROPERTY()
	FString month_description;

	UPROPERTY()
	int month_limit = 0;
	
};

USTRUCT()
struct FAAUChinaHealthReminderWordTip
{
	GENERATED_BODY()

	UPROPERTY()
	int type = 0;

	UPROPERTY()
	FString title;

	UPROPERTY()
	FString description;
	
	
};

USTRUCT()
struct FAAUChinaHealthReminderWord
{
	GENERATED_BODY()
	
	UPROPERTY()
	int account_type = 0;

	UPROPERTY()
	TArray<FAAUChinaHealthReminderWordTip> tips;
	
};



USTRUCT()
struct FAAUChinaAuthIdentifyWord
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString type;

	UPROPERTY()
	FString title;

	UPROPERTY()
	FString description;

	UPROPERTY()
	FString negative_button;

	UPROPERTY()
	FString positive_button;
	
};

USTRUCT()
struct FAAUChinaUIConfig
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FAAUChinaPayLimitWord> pay_limit_words;

	UPROPERTY()
	TArray<FAAUChinaHealthReminderWord> health_reminder_words;

	UPROPERTY()
	TArray<FAAUChinaAuthIdentifyWord> auth_identify_words;  // 见AAURealNameWordType 
	
};

USTRUCT()
struct FAAUChinaConfigModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString name;
	
	UPROPERTY()
	bool upload_user_action = false;

	UPROPERTY()
	FAAUChinaChildProtectedConfig child_protected_config;

	UPROPERTY()
	FAAUChinaUIConfig ui_config;

	UPROPERTY()
	TArray<FString> holiday;
	
	FAAUChinaHealthReminderWordTip GetNonageHealthTip(AAUHealthTipType Type);

	static void SaveToLocal(TSharedPtr<FAAUChinaConfigModel> Model);
	// 理论上一定是有值的，可以不用判空。
	static TSharedPtr<FAAUChinaConfigModel> GetLocalModel();

private:
	static TSharedPtr<FAAUChinaConfigModel> CurrentModel;
};
