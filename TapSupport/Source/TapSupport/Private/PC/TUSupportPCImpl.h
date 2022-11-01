#pragma once
#include "TUSupportImpl.h"

class TUSupportPCImpl: public TUSupportImpl {
public:
	TUSupportPCImpl();
	virtual ~TUSupportPCImpl() override;
	virtual void Init(FTapSupportConfig _Config) override;
	virtual void SetDefaultMetaData(TSharedPtr<FJsonObject> MetaData) override;
	virtual void SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData) override;
	virtual void LoginAnonymously(const FString& UserID) override;
	virtual FString GetSupportWebUrl(const FString& Path, TSharedPtr<FJsonObject> MetaData,
		TSharedPtr<FJsonObject> FieldsData) override;
	virtual void OpenSupportView(const FString& Path, TSharedPtr<FJsonObject> MetaData,
		TSharedPtr<FJsonObject> FieldsData) override;
	virtual void CloseSupportView() override;
	virtual void Resume() override;
	virtual void Pause() override;
	virtual void Logout() override;
	virtual void FetchUnReadStatus() override;

private:
	TSharedPtr<FJsonObject> DefaultMetaData;
	TSharedPtr<FJsonObject> DefaultFieldsData;
	FString AnonymousID;
	FString UnreadStatus;

	bool StopNotification = true;

	void StartNext();

	FTimerHandle TimerHandle;
	int32 NextDelay = 10;

	void CancelTimer();
	FString GetAnonymousID();
	// FString EncodeJsonObject(TSharedPtr<FJsonObject> JsonObject);
};
