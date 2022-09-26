#include "TUDBEvent.h"

#include "TapDB.h"
#include "TUDeviceInfo.h"
#include "TUDBImpl.h"
#include "TUHelper.h"
#include "TUDebuger.h"
#include "TUDBNet.h"
#include "TUDBStorage.h"


FString const TUDBEvent::Key::IPV6					= "ip_v6";
FString const TUDBEvent::Key::UserID				= "user_id";
FString const TUDBEvent::Key::DeviceID				= "device_id";
FString const TUDBEvent::Key::OpenID				= "open_id";
FString const TUDBEvent::Key::Name					= "name";
FString const TUDBEvent::Key::Type					= "type";
FString const TUDBEvent::Key::ClientID				= "client_id";
FString const TUDBEvent::Key::Properties			= "properties";
FString const TUDBEvent::Key::OS					= "os";
FString const TUDBEvent::Key::DeviceModel			= "device_model";
FString const TUDBEvent::Key::DeviceID1				= "device_id1";
FString const TUDBEvent::Key::DeviceID2				= "device_id2";
FString const TUDBEvent::Key::DeviceID3				= "device_id3";
FString const TUDBEvent::Key::DeviceID4				= "device_id4";
FString const TUDBEvent::Key::InstallID				= "install_uuid";
FString const TUDBEvent::Key::PersistID				= "persist_uuid";
FString const TUDBEvent::Key::Channel				= "channel";
FString const TUDBEvent::Key::Width					= "width";
FString const TUDBEvent::Key::Height				= "height";
FString const TUDBEvent::Key::OSVersion				= "os_version";
FString const TUDBEvent::Key::Provider				= "provider";
FString const TUDBEvent::Key::Network				= "network";
FString const TUDBEvent::Key::AppVersion			= "app_version";
FString const TUDBEvent::Key::SDKVersion			= "sdk_version";
FString const TUDBEvent::Key::LoginType				= "login_type";
FString const TUDBEvent::Key::OrderID				= "order_id";
FString const TUDBEvent::Key::Product				= "product";
FString const TUDBEvent::Key::Amount				= "amount";
FString const TUDBEvent::Key::VirtualCurrencyAmount = "virtual_currency_amount";
FString const TUDBEvent::Key::CurrencyType			= "currency_type";
FString const TUDBEvent::Key::Payment				= "payment";
FString const TUDBEvent::Key::Duration				= "duration";


// FString TUDBEventUser::GetOpenId()
// {
// 	return TUHelper::InvokeFunction<FString>("TULoginReflection", "GetOpenID");
// }


TUDBEvent::TUDBEvent(const FString& _ClientID) {
	ClientID = _ClientID;
	if (ClientID.IsEmpty()) {
		TUDebuger::ErrorLog("TapDB ClientID is Empty");
	}
	DeviceID = TUDeviceInfo::GetLoginId();
	if (DeviceID.IsEmpty()) {
		TUDebuger::ErrorLog("TapDB DeviceID is Empty");
	}
	GenerateSysProperties();
	GenerateCommonProperties();
	// UserID = TUDataStorage<FTUDBStorage>::LoadString(FString::Printf(TEXT("%s_%s"), *FTUDBStorage::LoginUserIDKey, *ClientID));
	// if (!UserID.IsEmpty()) {
	// 	SysProperties->SetStringField(Key::UserID, UserID);
	// }
	// FString OpenID = TUDataStorage<FTUDBStorage>::LoadString(FString::Printf(TEXT("%s_%s"), *FTUDBStorage::LoginOpenIDKey, *ClientID));
	// if (!OpenID.IsEmpty()) {
	// 	SysProperties->SetStringField(Key::OpenID, OpenID);
	// }
}

void TUDBEvent::SetUser(const FString& _UserID) {
	if (_UserID.IsEmpty()) {
		TUDebuger::ErrorLog(FString::Printf(TEXT("%s Error"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	UserID = _UserID;
	SysProperties->SetStringField(Key::UserID, _UserID);
	// TUDataStorage<FTUDBStorage>::SaveString(FString::Printf(TEXT("%s_%s"), *FTUDBStorage::LoginUserIDKey, *ClientID), _UserID);
}

void TUDBEvent::SetTapTapID(const FString& TapTapID) {
	if (TapTapID.IsEmpty()) {
		TUDebuger::ErrorLog(FString::Printf(TEXT("%s Error"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	SysProperties->SetStringField(Key::OpenID, TapTapID);
	// TUDataStorage<FTUDBStorage>::SaveString(FString::Printf(TEXT("%s_%s"), *FTUDBStorage::LoginOpenIDKey, *ClientID), TapTapID);
}

void TUDBEvent::ClearUser() {
	UserID = "";
	SysProperties->RemoveField(Key::UserID);
	SysProperties->RemoveField(Key::OpenID);
	// TUDataStorage<FTUDBStorage>::Remove(FString::Printf(TEXT("%s_%s"), *FTUDBStorage::LoginUserIDKey, *ClientID));
	// TUDataStorage<FTUDBStorage>::Remove(FString::Printf(TEXT("%s_%s"), *FTUDBStorage::LoginOpenIDKey, *ClientID));
}

void TUDBEvent::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties, TFunction<void()> SuccessBlock) {
	TSharedPtr<FJsonObject> AllProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(AllProperties, SysProperties);
	AllProperties->SetStringField(Key::Type, "track");
	AllProperties->SetStringField(Key::Name, EventName);

	TSharedPtr<FJsonObject> SubProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(SubProperties, CommonProperties);
	TUHelper::JsonObjectAppend(SubProperties, CustomStaticProperties);
	if (CustomDynamicPropertiesCaculator) {
		TUHelper::JsonObjectAppend(SubProperties, CustomDynamicPropertiesCaculator());
	}
	TUHelper::JsonObjectAppend(SubProperties, Properties);

	AllProperties->SetObjectField(Key::Properties, SubProperties);

	TUDBNet::SendEvent(AllProperties, SuccessBlock);
}

void TUDBEvent::DeviceInitialize(TSharedPtr<FJsonObject> Properties) {
	TSharedPtr<FJsonObject> AllProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(AllProperties, SysProperties);
	AllProperties->RemoveField(Key::UserID);
	AllProperties->SetStringField(Key::Type, "initialise");

	TSharedPtr<FJsonObject> SubProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(SubProperties, Properties);

	AllProperties->SetObjectField(Key::Properties, SubProperties);

	TUDBNet::SendEvent(AllProperties);
}

void TUDBEvent::DeviceUpdate(TSharedPtr<FJsonObject> Properties) {
	TSharedPtr<FJsonObject> AllProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(AllProperties, SysProperties);
	AllProperties->RemoveField(Key::UserID);
	AllProperties->SetStringField(Key::Type, "update");

	TSharedPtr<FJsonObject> SubProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(SubProperties, Properties);

	AllProperties->SetObjectField(Key::Properties, SubProperties);

	TUDBNet::SendEvent(AllProperties);
}

void TUDBEvent::DeviceAdd(TSharedPtr<FJsonObject> Properties) {
	TSharedPtr<FJsonObject> AllProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(AllProperties, SysProperties);
	AllProperties->RemoveField(Key::UserID);
	AllProperties->SetStringField(Key::Type, "add");

	TSharedPtr<FJsonObject> SubProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(SubProperties, Properties);

	AllProperties->SetObjectField(Key::Properties, SubProperties);

	TUDBNet::SendEvent(AllProperties);
}

void TUDBEvent::UserInitialize(TSharedPtr<FJsonObject> Properties) {
	if (UserID.IsEmpty()) {
		TUDebuger::ErrorLog(FString::Printf(TEXT("Please Set User ID Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TSharedPtr<FJsonObject> AllProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(AllProperties, SysProperties);
	AllProperties->RemoveField(Key::DeviceID);
	AllProperties->SetStringField(Key::Type, "initialise");

	TSharedPtr<FJsonObject> SubProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(SubProperties, Properties);

	AllProperties->SetObjectField(Key::Properties, SubProperties);

	TUDBNet::SendEvent(AllProperties);
}

void TUDBEvent::UserUpdate(TSharedPtr<FJsonObject> Properties) {
	if (UserID.IsEmpty()) {
		TUDebuger::ErrorLog(FString::Printf(TEXT("Please Set User ID Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TSharedPtr<FJsonObject> AllProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(AllProperties, SysProperties);
	AllProperties->RemoveField(Key::DeviceID);
	AllProperties->SetStringField(Key::Type, "update");

	TSharedPtr<FJsonObject> SubProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(SubProperties, Properties);

	AllProperties->SetObjectField(Key::Properties, SubProperties);

	TUDBNet::SendEvent(AllProperties);
}

void TUDBEvent::UserAdd(TSharedPtr<FJsonObject> Properties) {
	if (UserID.IsEmpty()) {
		TUDebuger::ErrorLog(FString::Printf(TEXT("Please Set User ID Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TSharedPtr<FJsonObject> AllProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(AllProperties, SysProperties);
	AllProperties->RemoveField(Key::DeviceID);
	AllProperties->SetStringField(Key::Type, "add");

	TSharedPtr<FJsonObject> SubProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(SubProperties, Properties);

	AllProperties->SetObjectField(Key::Properties, SubProperties);

	TUDBNet::SendEvent(AllProperties);
}

void TUDBEvent::GenerateSysProperties() {
	SysProperties = MakeShareable(new FJsonObject);
	SysProperties->SetStringField(Key::ClientID, ClientID);
	SysProperties->SetStringField(Key::DeviceID, DeviceID);
	FString IPV6 = TUDeviceInfo::GetIpv6();
	if (!IPV6.IsEmpty()) {
		SysProperties->SetStringField(Key::IPV6, IPV6);
	}
}

void TUDBEvent::GenerateCommonProperties() {
	CommonProperties = MakeShareable(new FJsonObject);
	FString OS;
#if PLATFORM_IOS
	OS = "iOS";
#elif PLATFORM_ANDROID
	OS = "Android";
#elif PLATFORM_MAC
	OS = "Mac";
#elif PLATFORM_WINDOWS
	OS = "Windows";
#endif
	CommonProperties->SetStringField(Key::OS, OS);
	CommonProperties->SetNumberField(Key::Width, TUDeviceInfo::GetScreenWidth());
	CommonProperties->SetNumberField(Key::Height, TUDeviceInfo::GetScreenHeight());
	CommonProperties->SetStringField(Key::OSVersion, TUDeviceInfo::GetOSVersion());
	CommonProperties->SetStringField(Key::InstallID, TUDeviceInfo::GetInstallId());
	CommonProperties->SetStringField(Key::PersistID, DeviceID);
	CommonProperties->SetStringField(Key::DeviceID1, DeviceID);
	CommonProperties->SetStringField(Key::SDKVersion, TapUEDB_VERSION);
	FString GameVersion = TUDBImpl::Get()->Config.GameVersion;
	if (!GameVersion.IsEmpty()) {
		CommonProperties->SetStringField(Key::AppVersion, GameVersion);
	}
	FString Channel = TUDBImpl::Get()->Config.Channel;
	if (!Channel.IsEmpty()) {
		CommonProperties->SetStringField(Key::Channel, Channel);
	}
}