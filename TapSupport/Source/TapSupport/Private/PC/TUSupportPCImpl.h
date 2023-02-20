#pragma once
#include "TUSupportImpl.h"
#include "Http.h"


class TUSupportPCImpl: public TUSupportImpl {
public:
	TUSupportPCImpl();
	virtual ~TUSupportPCImpl() override;
	virtual void Init(FTapSupportConfig _Config) override;
	virtual void SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData) override;
	virtual void UpdateDefaultField(const FString& Key, const TSharedPtr<FJsonValue>& NewValue) override;
	virtual void LoginAnonymously(const FString& UserID) override;
	virtual void LoginWithCustomCredential(const FString& Credential) override;
	virtual void LoginWithTDSCredential(const FString& Credential, const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed) override;
	virtual FString GetSupportWebUrl(const FString& Path, TSharedPtr<FJsonObject> FieldsData) override;
	virtual void OpenSupportView(const FString& Path, TSharedPtr<FJsonObject> FieldsData) override;
	virtual void CloseSupportView() override;
	virtual void Resume() override;
	virtual void Pause() override;
	virtual void Logout() override;
	virtual void FetchUnReadStatus() override;
	virtual void OpenFullUrl(const FString& Path);


private:
	void NetTDSLoginCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FSimpleDelegate OnSuccess, FTapFailed OnFailed);

	void FetchLoop();
	
	TSharedPtr<FJsonObject> DefaultFieldsData;
	FString CachedId;
	ESupportLoginType LoginType = ESupportLoginType::None;
	FString UnreadStatus;

	FTimerHandle TimerHandle;
	float NextDelay = 10.f;
	bool bUnregister = false;
	bool bWebOpen = false;
};
