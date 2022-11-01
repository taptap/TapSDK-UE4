#pragma once
#include "TUSupportImpl.h"

class TUSupportMobileImpl: public TUSupportImpl {
public:
	TUSupportMobileImpl();
	virtual void Init(FTapSupportConfig _Config) override;
	virtual void SetDefaultMetaData(TSharedPtr<FJsonObject> MetaData) override;
	virtual void SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData) override;
	virtual void LoginAnonymously(const FString& UserID) override;
	virtual void Logout() override;
	virtual FString GetSupportWebUrl(const FString& Path, TSharedPtr<FJsonObject> MetaData,
		TSharedPtr<FJsonObject> FieldsData) override;
	virtual void OpenSupportView(const FString& Path, TSharedPtr<FJsonObject> MetaData,
		TSharedPtr<FJsonObject> FieldsData) override;
	virtual void CloseSupportView() override;
	virtual void Resume() override;
	virtual void Pause() override;
	virtual void FetchUnReadStatus() override;
};
