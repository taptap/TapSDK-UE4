#pragma once
#include "TUSupportType.h"

class TUSupportImpl {
public:

	virtual ~TUSupportImpl() = default;

	static TSharedPtr<TUSupportImpl>& Get();

	FTapSupportConfig Config;
	
	virtual void Init(FTapSupportConfig _Config);

	virtual void SetDefaultMetaData(TSharedPtr<FJsonObject> MetaData);
	
	virtual void SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData);

	virtual void LoginAnonymously(const FString& UserID);

	virtual void Logout();

	virtual FString GetSupportWebUrl(const FString& Path, TSharedPtr<FJsonObject> MetaData, TSharedPtr<FJsonObject> FieldsData);

	virtual void OpenSupportView(const FString& Path, TSharedPtr<FJsonObject> MetaData, TSharedPtr<FJsonObject> FieldsData);

	virtual void CloseSupportView();

	virtual void Resume();

	virtual void Pause();

	virtual void FetchUnReadStatus();

private:
	static TSharedPtr<TUSupportImpl> Instance;
	
};
