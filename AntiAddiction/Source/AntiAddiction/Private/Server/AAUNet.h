#pragma once
#include "TUError.h"
#include "TUHttpRequest.h"
#include "Model/AAUPayableModel.h"
#include "Model/AAUPlayableModel.h"
#include "Model/AAURealNameResultModel.h"
#include "Model/AAUServerTimeModel.h"
#include "Model/AAUSettingsModel.h"
#include "Model/AAUUser.h"
#include "Model/China/AAUChinaConfigModel.h"
#include "Model/Vietnam/AAUVietnamConfigModel.h"


class AAUNet: public TUHttpRequest
{
public:

	AAUNet();

	static void GetStaticSetting(TFunction<void(TSharedPtr<FAAUSettingsModel> ModelPtr, const FTUError& Error)> CallBack);
	static void GetServerTime(TFunction<void(TSharedPtr<FAAUServerTimeModel> ModelPtr, const FTUError& Error)> CallBack);
	
	static void GetSDKConfig(TFunction<void(TSharedPtr<FAAUChinaConfigModel> ModelPtr, const FTUError& Error)> CallBack);
	static void GetSDKConfig(TFunction<void(TSharedPtr<FAAUVietnamConfigModel> ModelPtr, const FTUError& Error)> CallBack);

	/// 提交已付费金额
	static void SetPayment(int Amount, const FAAUUser& User, TFunction<void(TSharedPtr<FAAUPaymentModel> ModelPtr, const FTUError& Error)> CallBack);
	/// 检查付费限制
	static void CheckPayment(int Amount, const FAAUUser& User, TFunction<void(TSharedPtr<FAAUPayableModel> ModelPtr, const FTUError& Error)> CallBack);

	

	// "is_login":0,   // 1:登录时调用  0：游戏中调用
	static void CheckPlayable(const FString& UserID, const FString& Token,
		TArray<TArray<int>> ServerTimes, TArray<TArray<int>> LocalTimes,
		TFunction<void(TSharedPtr<FAAUPlayableModel> ModelPtr, const FTUError& Error)> CallBack, bool IsLogin = false);

	// MARK: - 实名认证部分
	static void ChinaManualVerify(const FString& UserID, const FString& Name, const FString& CardID,
		TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack);

	static void VietnamManualVerify(const FString& UserID, const FString& Year, const FString& Mouth, const FString& Day,
		TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack);

	static void CheckRealNameState(const FString& UserID, TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack);

	static bool RealNameServerIsCrash;

	static void AddUriParas(const FString& Url, const FString& UserID, TMap<FString, FString>& PathParas);
	

private:
	virtual TMap<FString, FString> CommonHeaders() override;
	virtual TSharedPtr<FJsonObject> CommonParameters() override;
	virtual bool ResetHeadersBeforeRequest() override;
	
};
