#pragma once
#include "UI/AAUManualRealNameWidget.h"
#include "AAUChinaSever.h"
#include "Server/AAUImpl.h"
#include "Server/AAUServerDelegate.h"

// class AAUChinaSever;
class AAUChinaImpl: public AAUImpl, AAUServerDelegate {
public:
	AAUChinaImpl();
	virtual ~AAUChinaImpl() override;
	virtual void Startup(const FString& UserID) override;
	virtual void Exit() override;
	virtual EAAUAgeLimit GetAgeRange() override;
	virtual int GetRemainingTime() override;
	virtual void KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content) override;
	virtual void InitImpl(const FAAUConfig& _Config) override;
	virtual void EnterGame() override;
	virtual void LeaveGame() override;
	virtual void CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler) override;
	virtual void SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler) override;
	virtual FString CurrentToken() override;
private:
	TSharedPtr<AAUChinaSever> Server;
	
	FString CurrentUserID;
	
	void TryAgainLogin(const FString& ErrMsg);

	void Login(const FString& AccessToken, EAAUAgeLimit AgeLimit, bool IsFirst = false);

	void ShowRealNameUI(UAAUManualRealNameWidget *Widget, AAURealNameWordType Type);

	void ShowHealthTipUI(const FString& Title, const FString& Content, int RemainTime, AAUHealthTipType Type);
};
