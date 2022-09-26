#pragma once
#include "TUDBType.h"

class TAPDB_API TapUEDB {
public:

	static void Init(FTUDBConfig Config);
	
	static void SetUserWithLoginType(const FString& UserId, const FString& LoginType = "");

	static void ClearUser();

	static void SetName(const FString& Name);

	static void SetLevel(int Level);

	static void SetServer(const FString& Server);

	static void OnCharge(const FString& OrderId, const FString& Product, int Amount, const FString& CurrencyType, const FString& Payment, TSharedPtr<FJsonObject> Properties = nullptr);

	static void TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties);

	static void RegisterStaticProperties(TSharedPtr<FJsonObject> Properties);

	static void UnregisterStaticProperty(const FString& Key);

	static void ClearStaticProperties();

	static void RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> PropertiesBlock);

	static void DeviceInitialize(TSharedPtr<FJsonObject> Properties);

	static void DeviceUpdate(TSharedPtr<FJsonObject> Properties);

	static void DeviceAdd(TSharedPtr<FJsonObject> Properties);

	static void UserInitialize(TSharedPtr<FJsonObject> Properties);

	static void UserUpdate(TSharedPtr<FJsonObject> Properties);

	static void UserAdd(TSharedPtr<FJsonObject> Properties);

	// iOS 独有
	static void AdvertiserIDCollectionEnabled(bool Enable);

	static void EnableLog(bool Enable);
	
};
