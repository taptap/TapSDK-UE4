#pragma once
#include "Server/AAUImpl.h"
#include "UI/AAUManualRealNameWidget.h"
#include "Server/AAUServerDelegate.h"

class AAUChinaAndroidImpl: public AAUImpl {
public:
	AAUChinaAndroidImpl();
	virtual ~AAUChinaAndroidImpl() override;
	virtual void Startup(const FString& UserID) override;
	virtual void Exit() override;
	virtual EAAUAgeLimit GetAgeRange() override;
	virtual int GetRemainingTime() override;
	virtual void InitImpl(const FAAUConfig& _Config) override;
	virtual void EnterGame() override;
	virtual void LeaveGame() override;
	virtual void CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler) override;
	virtual void SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler) override;
	virtual FString CurrentToken() override;
};
