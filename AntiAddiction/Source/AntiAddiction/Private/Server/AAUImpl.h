#pragma once
#include "AAUType.h"

class AAUImpl {
public:
	virtual ~AAUImpl() = default;
	static TSharedPtr<AAUImpl>& Get();

	static FAAUConfig Config;

	static FString LocalTokenString;

	bool IsNeedStandAlone(TFunction<void(bool IsNeed)> CallBack = nullptr);

	void Init(const FAAUConfig& _Config);

	virtual void InitImpl(const FAAUConfig& _Config);

	virtual void Startup(const FString& UserID);

	virtual void Exit();

	virtual void EnterGame();

	virtual void LeaveGame();

	virtual EAAUAgeLimit GetAgeRange();

	virtual int GetRemainingTime();

	virtual void CheckPayLimit(int Amount,TFunction<void(bool Status)> CallBack,TFunction<void(const FString& Msg)> FailureHandler);
	
	virtual void SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack, TFunction<void(const FString& Msg)> FailureHandler);

	virtual FString CurrentToken();
	
	static int AdultRemainTime;
	


private:
	static TSharedPtr<AAUImpl> Instance;
	
	TSharedPtr<bool> StandAloneEnable;
};
