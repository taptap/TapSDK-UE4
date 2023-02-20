#pragma once
#include "TUError.h"
#include "TUSupportType.h"

enum class ESupportLoginType : uint8
{
	None,
	Anonymous,
	Custom,
	TDS
};

#define LOOP_INC_SECONDS 10.f
#define LOOP_MAX_SECONDS 300.f
#define UNREGISTER_SECONDS 3600.f

class TUSupportImpl {
public:

	virtual ~TUSupportImpl() = default;

	static TSharedPtr<TUSupportImpl>& Get();

	FTapSupportConfig Config;
	
	virtual void Init(FTapSupportConfig _Config);

	virtual void SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData);

	virtual void UpdateDefaultField(const FString& Key, const TSharedPtr<FJsonValue>& NewValue);

	virtual void LoginAnonymously(const FString& UserID);

	virtual void LoginWithCustomCredential(const FString& Credential) = 0;
	
	virtual void LoginWithTDSCredential(const FString& Credential, const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed) = 0;

	virtual void Logout();

	virtual FString GetSupportWebUrl(const FString& Path, TSharedPtr<FJsonObject> FieldsData);

	virtual void OpenSupportView(const FString& Path, TSharedPtr<FJsonObject> FieldsData);
	
	virtual void CloseSupportView();

	virtual void Resume();

	virtual void Pause();

	virtual void FetchUnReadStatus();

private:
	static TSharedPtr<TUSupportImpl> Instance;
	
};
