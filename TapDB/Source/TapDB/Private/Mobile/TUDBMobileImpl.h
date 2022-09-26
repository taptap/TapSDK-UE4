#pragma once
#include "TUDBImpl.h"

class TUDBMobileImpl: public TUDBImpl {
public:
	TUDBMobileImpl();
	
	virtual void Init(FTUDBConfig InitConfig) override;
	virtual void SetUserWithLoginType(const FString& UserId, const FString& LoginType) override;
	virtual void ClearUser() override;
	virtual void SetName(const FString& Name) override;
	virtual void SetLevel(int Level) override;
	virtual void SetServer(const FString& Server) override;
	virtual void OnCharge(const FString& OrderId, const FString& Product, int Amount, const FString& CurrencyType,
		const FString& Payment, TSharedPtr<FJsonObject> Properties) override;
	virtual void TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties) override;
	virtual void RegisterStaticProperties(TSharedPtr<FJsonObject> Properties) override;
	virtual void UnregisterStaticProperty(const FString& Key) override;
	virtual void ClearStaticProperties() override;
	virtual void RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> PropertiesBlock) override;
	virtual void DeviceInitialize(TSharedPtr<FJsonObject> Properties) override;
	virtual void DeviceUpdate(TSharedPtr<FJsonObject> Properties) override;
	virtual void DeviceAdd(TSharedPtr<FJsonObject> Properties) override;
	virtual void UserInitialize(TSharedPtr<FJsonObject> Properties) override;
	virtual void UserUpdate(TSharedPtr<FJsonObject> Properties) override;
	virtual void UserAdd(TSharedPtr<FJsonObject> Properties) override;
	virtual void AdvertiserIDCollectionEnabled(bool Enable) override;
	virtual void EnableLog(bool Enable) override;
};
