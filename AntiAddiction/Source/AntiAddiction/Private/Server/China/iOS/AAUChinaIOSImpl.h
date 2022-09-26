#pragma once
#include "Server/AAUImpl.h"

class AAUChinaIOSImpl: public AAUImpl {
public:
	AAUChinaIOSImpl();
	virtual ~AAUChinaIOSImpl() override;
	virtual void InitImpl(const FAAUConfig& _Config) override;
	virtual void Startup(const FString& UserID) override;
	virtual void Exit() override;
	virtual void EnterGame() override;
	virtual void LeaveGame() override;
	virtual EAAUAgeLimit GetAgeRange() override;
	virtual int GetRemainingTime() override;
	virtual void CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler) override;
	virtual void SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler) override;
	virtual FString CurrentToken() override;

};
