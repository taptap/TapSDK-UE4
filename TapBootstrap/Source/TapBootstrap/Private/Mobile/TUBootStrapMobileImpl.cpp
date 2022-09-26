#include "TUBootStrapMobileImpl.h"

#include "TapBridgeWrapper.h"
#include "TUJsonHelper.h"
#include "TUMobileBridge.h"

#define TAP_BOOTSTRAP_CLZ "com.tapsdk.bootstrap.wrapper.TapBootstrapService"
#define TAP_BOOTSTRAP_IMPL "com.tapsdk.bootstrap.wrapper.TapBootstrapServiceImpl"
#define TAP_BOOTSTRAP_SERVICE "TapBootstrapService"


TUBootStrapMobileImpl::TUBootStrapMobileImpl() {
	TUMobileBridge::Register(TEXT(TAP_BOOTSTRAP_CLZ),TEXT(TAP_BOOTSTRAP_IMPL));
}

void TUBootStrapMobileImpl::Init(const FTUConfig& Config) {
	FString ConfigJSON;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ConfigWriter = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&ConfigJSON);
	ConfigWriter->WriteObjectStart();
	ConfigWriter->WriteValue("clientID", Config.ClientID);
	ConfigWriter->WriteValue("clientToken", Config.ClientToken);
	ConfigWriter->WriteValue("serverUrl", Config.ServerURL);
	ConfigWriter->WriteValue("isCN", Config.RegionType == ERegionType::CN);
	if (Config.DBConfig.Enable)
	{
		ConfigWriter->WriteObjectStart("dbConfig");
		ConfigWriter->WriteValue("enable", Config.DBConfig.Enable);
#if PLATFORM_IOS
		ConfigWriter->WriteValue("advertiserIDCollectionEnabled",Config.DBConfig.AdvertiserIDCollectionEnabled);
#endif
		ConfigWriter->WriteValue("gameVersion", Config.DBConfig.GameVersion);
		ConfigWriter->WriteValue("channel", Config.DBConfig.Channel);
		ConfigWriter->WriteObjectEnd();
	}
	ConfigWriter->WriteObjectEnd();
	ConfigWriter->Close();

	FString BootstrapConfigJSON;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> BootstrapWriter = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&BootstrapConfigJSON);
	BootstrapWriter->WriteObjectStart();
	BootstrapWriter->WriteValue("initWithConfig", ConfigJSON);
	BootstrapWriter->WriteObjectEnd();
	BootstrapWriter->Close();

	TUMobileBridge::AsyncPerform(TAP_BOOTSTRAP_SERVICE, "initWithConfig", BootstrapConfigJSON);

}

void TUBootStrapMobileImpl::Login(TArray<FString> Permissions, TFunction<void(const FTDSUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	
#if PLATFORM_IOS
	dispatch_async(dispatch_get_main_queue(), ^{
		[TDSUser loginByTapTapWithPermissions: IOSHelper::convertArray(Permissions) callback:^(TDSUser *_Nullable user, NSError *_Nullable error) {
			DealLoginCallBack(user, error, SuccessBlock, FailBlock);
	  }];
	});
#else
	
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("permissions"), Permissions);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAP_BOOTSTRAP_SERVICE, "login", JsonOutString, [=](const FString& ResultJsonStr) {
		DealLoginCallBack(ResultJsonStr, SuccessBlock, FailBlock);
	});

#endif
}

void TUBootStrapMobileImpl::AnonymouslyLogin(TFunction<void(const FTDSUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
#if PLATFORM_IOS
	dispatch_async(dispatch_get_main_queue(), ^{
		[TDSUser loginAnonymously:^(TDSUser *_Nullable user, NSError *_Nullable error) {
			DealLoginCallBack(user, error, SuccessBlock, FailBlock);	
	    }];
	});
#else
	TUMobileBridge::AsyncPerform(TAP_BOOTSTRAP_SERVICE, "anonymouslyLogin", "", [=](const FString& ResultJsonStr) {
		DealLoginCallBack(ResultJsonStr, SuccessBlock, FailBlock);
	});

#endif
}

void TUBootStrapMobileImpl::Logout() {
	TUMobileBridge::AsyncPerform(TAP_BOOTSTRAP_SERVICE, "logout", "");
}

TSharedPtr<FTDSUser> TUBootStrapMobileImpl::GetUser() {
#if PLATFORM_IOS
	TDSUser *iOSUser = [TDSUser currentUser];
	if (iOSUser) {
		TSharedPtr<FTDSUser> User = MakeShareable(new FTDSUser((NSObject *)iOSUser));
		return User;
	} else {
		return nullptr;
	}
#else
	FString ResultStr = TUMobileBridge::Perform(TAP_BOOTSTRAP_SERVICE, "getUser", "");
	auto ResultJsonObject = TUJsonHelper::GetJsonObject(ResultStr);
	if (ResultJsonObject.IsValid() && ResultJsonObject->GetIntegerField("getUserInfoCode") == 0) {
		FString WrapStr;
		if (ResultJsonObject->TryGetStringField("wrapper", WrapStr)) {
			return TUJsonHelper::GetUStruct<FTDSUser>(WrapStr);
		}
	}
	return nullptr;

#endif
	
}

void TUBootStrapMobileImpl::SetPreferLanguage(ELanguageType LangType) {
	TSharedPtr<FJsonObject> Args = MakeShareable(new FJsonObject);
	Args->SetNumberField(TEXT("preferredLanguage"),(int)LangType);
	TUMobileBridge::AsyncPerform(TAP_BOOTSTRAP_SERVICE, "setPreferredLanguage", TUJsonHelper::GetJsonString(Args));
}

void TUBootStrapMobileImpl::DealLoginCallBack(const FString& ResultStr, TFunction<void(const FTDSUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	auto LoginWrapper = TUJsonHelper::GetUStruct<FTapLoginWrapper>(ResultStr);
	if (LoginWrapper.IsValid()) {
		if (LoginWrapper->loginCallbackCode == 0) {
			auto UserPtr = TUJsonHelper::GetUStruct<FTDSUser>(LoginWrapper->wrapper);
			if (UserPtr.IsValid() && SuccessBlock) {
				SuccessBlock(*UserPtr.Get());
				return;
			}
		} else {
			auto ErrorPtr = TUJsonHelper::GetUStruct<FTUError>(LoginWrapper->wrapper);
			if (ErrorPtr.IsValid() && FailBlock) {
				FailBlock(*ErrorPtr.Get());
				return;
			}
		}
	}
	if (FailBlock) {
		FailBlock(FTUError(FTUError::ERROR_CODE_BRIDGE_EXECUTE, "TapBootStrap Login Bridge Error"));
	}
}

#if PLATFORM_IOS
void TUBootStrapMobileImpl::DealLoginCallBack(TDSUser* user, NSError* error, TFunction<void(const FTDSUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	if (error) {
		if (FailBlock) {
			FTUError Error = IOSHelper::convertError(error);
			AsyncTask(ENamedThreads::GameThread, [=]() {
				FailBlock(Error);
			});
		}
	}
	else {
		if (SuccessBlock) {
			FTDSUser User = FTDSUser((NSObject *)user);
			AsyncTask(ENamedThreads::GameThread, [=]() {
				SuccessBlock(User);
			});
			
		}
	}
}
#endif
