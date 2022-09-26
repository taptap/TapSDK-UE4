#pragma once
#include "AAUVietnamServer.h"
#include "Server/AAUImpl.h"
#include "Server/AAUServerDelegate.h"

class SAAURealNameVietnam;

class AAUVietnamImpl: public AAUImpl, AAUServerDelegate {
public:
	AAUVietnamImpl();
	virtual ~AAUVietnamImpl() override;
	virtual void InitImpl(const FAAUConfig& _Config) override;
	virtual void EnterGame() override;
	virtual void LeaveGame() override;
	virtual void Startup(const FString& UserID) override;
	virtual void Exit() override;
	virtual EAAUAgeLimit GetAgeRange() override;
	virtual int GetRemainingTime() override;
	virtual void CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler) override;
	virtual void SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler) override;

	virtual void KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content) override;
	virtual FString CurrentToken() override;
private:
	TSharedPtr<AAUVietnamServer> Server;

	TSharedPtr<SAAURealNameVietnam> RealName;

	bool IsFirstRealNameSuccess = false;
	
	FString CurrentUserID;
	
	void Login(const FString& AccessToken, EAAUAgeLimit AgeLimit);

	void ShowRealNameUI();

	void ShowTimeLimitUI(TFunction<void()> ComformBlock, bool IsExit);

	void TryAgainLogin(const FString& ErrMsg);
};
