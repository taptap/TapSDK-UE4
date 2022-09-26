#pragma once
#include "TUDBType.h"

class TUDBImpl {
public:

	virtual ~TUDBImpl() = default;

	static TSharedPtr<TUDBImpl>& Get();

	FTUDBConfig Config;
	
	virtual void Init(FTUDBConfig InitConfig);
	
	virtual void SetUserWithLoginType(const FString& UserId, const FString& LoginType = "");

	virtual void ClearUser();

	virtual void SetName(const FString& Name);

	virtual void SetLevel(int Level);

	virtual void SetServer(const FString& Server);

	virtual void OnCharge(const FString& OrderId, const FString& Product, int Amount, const FString& CurrencyType, const FString& Payment, TSharedPtr<FJsonObject> Properties = nullptr);

	virtual void TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties);

	virtual void RegisterStaticProperties(TSharedPtr<FJsonObject> Properties);

	virtual void UnregisterStaticProperty(const FString& Key);

	virtual void ClearStaticProperties();

	virtual void RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> PropertiesBlock);

	virtual void DeviceInitialize(TSharedPtr<FJsonObject> Properties);

	virtual void DeviceUpdate(TSharedPtr<FJsonObject> Properties);

	virtual void DeviceAdd(TSharedPtr<FJsonObject> Properties);

	virtual void UserInitialize(TSharedPtr<FJsonObject> Properties);

	virtual void UserUpdate(TSharedPtr<FJsonObject> Properties);

	virtual void UserAdd(TSharedPtr<FJsonObject> Properties);

	// iOS 独有
	virtual void AdvertiserIDCollectionEnabled(bool Enable);

	// 移动端 独有
	virtual void EnableLog(bool Enable);

private:
	static TSharedPtr<TUDBImpl> Instance;
};
