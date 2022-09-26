#include "TUDBPCImpl.h"

#include "TUDBNet.h"
#include "TUDBStorage.h"
#include "TUHelper.h"

FString TUDBPCImpl::First_login_type = "";
FString TUDBPCImpl::First_open_id = "";

void TUDBPCImpl::Init(FTUDBConfig InitConfig) {
	Config = InitConfig;
	TapDBEventPtr = MakeShareable(new TUDBEvent(Config.ClientId));
	
	StartTime = MakeShareable(new FDateTime(FDateTime::Now()));
	RegisterCoreDelegate();

	SendDeviceLogin();

	// 上次时长没有发送成功，再次发送
	double PlayTime = TUDataStorage<FTUDBStorage>::LoadNumber(FTUDBStorage::TapDBPlayTime);
	if (PlayTime > 0) {
		SendPlayTime(PlayTime);
		TUDataStorage<FTUDBStorage>::Remove(FTUDBStorage::TapDBPlayTime);
	}
}

void TUDBPCImpl::SetUserWithLoginType(const FString& UserId, const FString& LoginType) {
	if (!CheckStringParam(UserId, "UserId")) {
		return;
	}
	TapDBEventPtr->SetUser(UserId);

	FString OpenID;
	if (LoginType == TUDBType::LoginType::TapTap) {
		OpenID = TUHelper::InvokeFunction<FString>("TULoginReflection", "GetOpenID");
		if (!OpenID.IsEmpty()) {
			TapDBEventPtr->SetTapTapID(OpenID);
		}
	}
	TSharedPtr<FJsonObject> UserLoginProperties = nullptr;
	if (!LoginType.IsEmpty()) {
		UserLoginProperties = MakeShareable(new FJsonObject);
		UserLoginProperties->SetStringField(TUDBEvent::Key::LoginType, LoginType);
	}
	TapDBEventPtr->TrackEvent("user_login", UserLoginProperties);

	TSharedPtr<FJsonObject> DeviceInitialProperties = MakeShareable(new FJsonObject);
	if (First_login_type.IsEmpty()) {
		First_login_type = LoginType;
		DeviceInitialProperties->SetStringField("first_login_type", LoginType);
	}
	if (!OpenID.IsEmpty() && First_open_id.IsEmpty()) {
		First_open_id = OpenID;
		DeviceInitialProperties->SetStringField("first_open_id", OpenID);
	}
	if (DeviceInitialProperties->Values.Num() > 0) {
		TapDBEventPtr->DeviceInitialize(DeviceInitialProperties);
	}

	TSharedPtr<FJsonObject> DeviceUpdateProperties = MakeShareable(new FJsonObject);
	DeviceUpdateProperties->SetBoolField("has_user", true);
	if (!LoginType.IsEmpty()) {
		DeviceUpdateProperties->SetStringField("current_login_type", LoginType);
	}
	if (!OpenID.IsEmpty()) {
		DeviceUpdateProperties->SetStringField("current_open_id", OpenID);
	}
	TapDBEventPtr->DeviceUpdate(DeviceUpdateProperties);
	
}

void TUDBPCImpl::ClearUser() {
	TapDBEventPtr->ClearUser();
}

void TUDBPCImpl::SetName(const FString& Name) {
	if (!CheckStringParam(Name, "Name")) {
		return;
	}
	TSharedPtr<FJsonObject> NameProperties = MakeShareable(new FJsonObject);
	NameProperties->SetStringField("user_name", Name);
	TapDBEventPtr->UserUpdate(NameProperties);
}

void TUDBPCImpl::SetLevel(int Level) {
	if (Level < 0) {
		TUDebuger::ErrorLog("level is illegal, should >= 0");
		return;
	}
	TSharedPtr<FJsonObject> LevelProperties = MakeShareable(new FJsonObject);
	LevelProperties->SetNumberField("level", Level);
	TapDBEventPtr->UserUpdate(LevelProperties);
}

void TUDBPCImpl::SetServer(const FString& Server) {
	if (!CheckStringParam(Server, "Server")) {
		return;
	}

	TSharedPtr<FJsonObject> ServerInitialiseProperties = MakeShareable(new FJsonObject);
	ServerInitialiseProperties->SetStringField("first_server", Server);
	TapDBEventPtr->UserInitialize(ServerInitialiseProperties);

	TSharedPtr<FJsonObject> ServerUpdateProperties = MakeShareable(new FJsonObject);
	ServerUpdateProperties->SetStringField("current_server", Server);
	TapDBEventPtr->UserUpdate(ServerUpdateProperties);
}

void TUDBPCImpl::OnCharge(const FString& OrderId, const FString& Product, int Amount, const FString& CurrencyType,
                          const FString& Payment, TSharedPtr<FJsonObject> Properties) {
	if (Amount <= 0 || Amount > 100000000000) {
		TUDebuger::ErrorLog("amount is illegal, shoud > 0 and <= 100000000000");
		return;
	}
	TSharedPtr<FJsonObject> EventDataDic = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(EventDataDic, Properties);
	
	EventDataDic->SetNumberField(TUDBEvent::Key::Amount, Amount);
	if (!OrderId.IsEmpty()) {
		EventDataDic->SetStringField(TUDBEvent::Key::OrderID, OrderId);
	}
	if (!Product.IsEmpty()) {
		EventDataDic->SetStringField(TUDBEvent::Key::Product, Product);
	}
	if (!Payment.IsEmpty()) {
		EventDataDic->SetStringField(TUDBEvent::Key::Payment, Payment);
	}
	if (!CurrencyType.IsEmpty()) {
		EventDataDic->SetStringField(TUDBEvent::Key::CurrencyType, CurrencyType);
	}
	TapDBEventPtr->TrackEvent("charge", EventDataDic);
}

void TUDBPCImpl::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties) {
	if (EventName.IsEmpty()) {
		TUDebuger::ErrorLog("eventName is empty, will do nothing.");
		return;
	}
	TapDBEventPtr->TrackEvent(EventName, Properties);
}

void TUDBPCImpl::RegisterStaticProperties(TSharedPtr<FJsonObject> Properties) {
	TapDBEventPtr->CustomStaticProperties = Properties;
}

void TUDBPCImpl::UnregisterStaticProperty(const FString& Key) {
	if (TapDBEventPtr->CustomStaticProperties.IsValid()) {
		TapDBEventPtr->CustomStaticProperties->RemoveField(Key);
	}
}

void TUDBPCImpl::ClearStaticProperties() {
	RegisterStaticProperties(nullptr);
}

void TUDBPCImpl::RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> PropertiesBlock) {
	TapDBEventPtr->CustomDynamicPropertiesCaculator = PropertiesBlock;
}

void TUDBPCImpl::DeviceInitialize(TSharedPtr<FJsonObject> Properties) {
	TapDBEventPtr->DeviceInitialize(Properties);
}

void TUDBPCImpl::DeviceUpdate(TSharedPtr<FJsonObject> Properties) {
	TapDBEventPtr->DeviceUpdate(Properties);
}

void TUDBPCImpl::DeviceAdd(TSharedPtr<FJsonObject> Properties) {
	TapDBEventPtr->DeviceAdd(Properties);
}

void TUDBPCImpl::UserInitialize(TSharedPtr<FJsonObject> Properties) {
	TapDBEventPtr->UserInitialize(Properties);
}

void TUDBPCImpl::UserUpdate(TSharedPtr<FJsonObject> Properties) {
	TapDBEventPtr->UserUpdate(Properties);
}

void TUDBPCImpl::UserAdd(TSharedPtr<FJsonObject> Properties) {
	TapDBEventPtr->UserAdd(Properties);
}

void TUDBPCImpl::SendPlayTime(int Interval, TFunction<void()> SuccessBlock) {
	TSharedPtr<FJsonObject> TimeProperties = MakeShareable(new FJsonObject);
	TimeProperties->SetNumberField(TUDBEvent::Key::Duration, Interval);
	TapDBEventPtr->TrackEvent("play_game", TimeProperties, SuccessBlock);
}

void TUDBPCImpl::SendDeviceLogin() const {
	TapDBEventPtr->TrackEvent("device_login", nullptr);
}

bool TUDBPCImpl::CheckStringParam(const FString& Para, const FString& ParaName) {
	if (Para.IsEmpty() || Para.Len() > 256) {
		TUDebuger::ErrorLog(FString::Printf(TEXT("%s is illegal, length should > 0 and <= 256"), *ParaName));
		return false;
	}
	return true;
}

void TUDBPCImpl::RegisterCoreDelegate() {
	FCoreDelegates::ApplicationWillDeactivateDelegate.AddLambda([=]() {
		TUDebuger::DisplayLog("Enter background.");
		PauseTime = MakeShareable(new FDateTime(FDateTime::Now()));
	});

	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddLambda([=]() {
		FDateTime Now = FDateTime::Now();
		if (StartTime.IsValid()) {
			StartTime = MakeShareable(new FDateTime(Now));
			PauseTime = nullptr;
		}
		else if (PauseTime.IsValid()) {
			if ((Now - *PauseTime.Get()).GetTotalSeconds() > 30) {
				//算作一次新的游戏
				TUDebuger::DisplayLog("Start with a new session.");
				auto PlayTime = (*PauseTime.Get() - *StartTime.Get()).GetTotalSeconds();
				if (PlayTime > 0) {
					SendPlayTime(PlayTime);
				}
				StartTime = MakeShareable(new FDateTime(Now));
				PauseTime = nullptr;
			}
			else {
				PauseTime = nullptr;
			}
		}
	});

	FCoreDelegates::ApplicationWillTerminateDelegate.AddLambda([=]() {
		if (!StartTime.IsValid()) {
			return;
		}
		FDateTime Now = FDateTime::Now();
		// 先把游戏事件缓存到本地，然后发送给服务端，如果发送成功，那么删除，不然下次激活的时候发送
		auto PlayTime = (Now - *StartTime.Get()).GetTotalSeconds();
		if (PlayTime > 0) {
			// 如果网络请求数为0，说明前面的请求都发送成功了，直接发送时长就行了，如果不是，说明网络情况可能不好，留着下次启动的时候发送。
			// 以上情况并不是绝对，只是大概率的情况。
			if (TUDBNet::CacheCount == 0) {
				SendPlayTime(PlayTime);
			} else {
				TUDataStorage<FTUDBStorage>::SaveNumber(FTUDBStorage::TapDBPlayTime, PlayTime);
			}
		}
	});
}
