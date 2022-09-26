#pragma once
#include "Model/AAULoginResult.h"
#include "Model/AAUUser.h"
#include "Server/AAUServerDelegate.h"

class AAUChinaSever {
public:
	AAUServerDelegate *Delegate;
	
	void Login(const FAAUUser& User, TFunction<void(const AAULoginResult& Result)> CallBack);

	void EnterGame();

	void LeaveGame();

	void Logout();

	int GetRemainingTime();

	EAAUAgeLimit GetAgeRange();
	
	FString CurrentToken();
	
	void CheckPayLimit(int Amount,
	TFunction<void(bool Status, const FString& Title, const FString& Description)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler);

	void SubmitPayResult(int Amount,
		TFunction<void(bool Success)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler);

	

private:
	bool IsInGame = false;
	int PlayableServerCrashCount = 0;
	
	int64 TimeSpan = 0;   // 本地时间与服务器时间的差值

	double CostTime = 0; // 花费的时间
	double RemainTime = 0; // 剩余时间

	FDateTime TimeRecordBegin = 0; // 起始记录时间
	FDateTime TimeRecordEnd = 0; // 最后记录时间

	FTimerHandle CheckTimer;
	FTimerHandle CountDownTimer;

	TSharedPtr<FAAUUser> CurrentUser;

	TArray<TArray<int>> CostTimes;

	void ResetValues();

	FDateTime GetCurrentTime();
	int64 CalculateRemainTime(AAUTimeBoundary& Boundary);

	void LoginStandAlone(const FAAUUser& User, TFunction<void(const AAULoginResult& Result)> CallBack);
	
	void LoginSuccess(const FAAUUser& User, bool NeedLoadCostTime);

	void StartCheckTimer();
	void StartCountDownTimer();
	void StopTimer();
	void StartTimer();

	// IsPure 表示只是纯粹的上传操作，不做踢出判断
	void UploadTime(bool IsPure);

	void UpdateCostAndRemainTime();

	bool AccessTokenIsInvaild(const FString& AccessToken);

	void KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content, bool IsServer);

	
};


