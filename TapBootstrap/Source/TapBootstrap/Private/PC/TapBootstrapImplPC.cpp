#include "TapBootstrapImplPC.h"

#include "HttpModule.h"
#include "TapUELogin.h"
#include "TUCrypto.h"
#include "TUHelper.h"
#include "TUJsonHelper.h"
#include "TULoginType.h"
#include "Interfaces/IHttpResponse.h"


FString GetLeanCloudSyncString()
{
	const int64 Ticks = FDateTime::UtcNow().ToUnixTimestamp() * 1000;
	const FString TimeStr = FString::Printf(TEXT("%010lld"), Ticks);
	return FMD5::HashAnsiString(*(TimeStr + FTUConfig::Get()->ClientToken)) + TEXT(",") + TimeStr;
}

FTapBootstrapImplPC::~FTapBootstrapImplPC() {
}

void FTapBootstrapImplPC::Init(const FTUConfig& InConfig) {
	FTUConfig::Init(InConfig);
	
	// 初始化 TapLogin
	FTULoginConfig LoginConfig;
	LoginConfig.ClientID = InConfig.ClientID;
	LoginConfig.RegionType = InConfig.RegionType;
	TapUELogin::Init(LoginConfig);

	// 初始化 TapDB
	if (InConfig.DBConfig.Enable)
	{
		TUHelper::InvokeNoReturnFunction("TUDBReflection", "Init");
	}
}

TSharedPtr<FTDSUser> FTapBootstrapImplPC::GetCurrentUser() {
	if (CachedUser)
	{
		return CachedUser;
	}

	FString Key;
	if (!GConfig->GetString(TEXT("/Script/TapCommon.TapSettings"), TEXT("SaveFileKey"), Key, GEngineIni))
	{
		Key = TEXT("tap_save_file_key");
	}
	const FString FilePath = FPaths::SandboxesDir() + TEXT("/DataStorage/TDSUser");
	TArray<uint8> Data;
	if (FPaths::FileExists(FilePath))
	{
		if (FFileHelper::LoadFileToArray(Data, *FilePath))
		{
			const FString JsonStr = TUCrypto::UTF8Encode(TUCrypto::AesDecode(Data, TUCrypto::UTF8Encode(Key)));

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(JsonStr);
			if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
			{
				return nullptr;
			}
			
			CachedUser = MakeShared<FTDSUser>(JsonObject);
			return CachedUser;
		}
	}

	return nullptr;
}

void FTapBootstrapImplPC::Logout() {
	CachedUser.Reset();
	const FString FilePath = FPaths::SandboxesDir() + TEXT("/DataStorage/TDSUser");
	IFileManager::Get().Delete(*FilePath);
	TapUELogin::Logout();
}

void FTapBootstrapImplPC::LoginAnonymously(FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("POST"));
	
	const FString Url = FTUConfig::Get()->ServerURL + TEXT("/1.1/users");
	HttpRequest->SetURL(Url);
	
	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLeanCloudSyncString());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString JsonString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);
	Writer->WriteObjectStart();
	Writer->WriteObjectStart(TEXT("authData"));
	Writer->WriteObjectStart(TEXT("anonymous"));
#if PLATFORM_IOS || PLATFORM_ANDROID
	FString ID = FPlatformMisc::GetDeviceId();
#else
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	FString ID = FPlatformMisc::GetMacAddressString();
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
#endif
	Writer->WriteValue(TEXT("id"), ID);
	Writer->WriteObjectEnd();
	Writer->WriteObjectEnd();
	Writer->WriteObjectEnd();
	Writer->Close();
	HttpRequest->SetContentAsString(JsonString);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FTapBootstrapImplPC::OnUserInfoCallback, OnSuccess, OnError);

	HttpRequest->ProcessRequest();
}

void FTapBootstrapImplPC::LoginWithTapTap(const TArray<FString>& Permissions, FTDSUser::FDelegate OnSuccess,
	FTUError::FDelegate OnError) {
	if (TapUELogin::GetAccessToken().IsValid() && TapUELogin::GetProfile().IsValid())
	{
		const TSharedPtr<FTUAccessToken> TapAccessToken = TapUELogin::GetAccessToken();
		const TSharedPtr<FTULoginProfileModel> TapProfile = TapUELogin::GetProfile();
		InternalLoginWithTapTap(TapAccessToken, TapProfile, OnSuccess, OnError);
	}
	else
	{
		auto TapLoginResult = [=](const TUAuthResult& Result)
		{
			switch (Result.GetType())
			{
			case TUAuthResult::Success:
			{
				const TSharedPtr<FTUAccessToken> TapAccessToken = TapUELogin::GetAccessToken();
				const TSharedPtr<FTULoginProfileModel> TapProfile = TapUELogin::GetProfile();
				ensure(TapAccessToken && TapProfile);
				
				InternalLoginWithTapTap(TapAccessToken, TapProfile, OnSuccess, OnError);
			}
				break;
			case TUAuthResult::Cancel:
			case TUAuthResult::Fail:
				if (TSharedPtr<FTUError> Error = Result.GetError())
				{
					OnError.ExecuteIfBound(*Error);
				}
				else
				{
					OnError.ExecuteIfBound(FTUError(-1, TEXT("TapTap login failed.")));
				}
				break;
			default:
				OnError.ExecuteIfBound(FTUError(-1, TEXT("Login taptap error")));
			}
		};
		TapUELogin::Login(Permissions, TapLoginResult);
	}
}

void FTapBootstrapImplPC::BecomeWithSessionToken(const FString& SessionToken, FTDSUser::FDelegate OnSuccess,
                                                 FTUError::FDelegate OnError) {
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("GET"));

	const FString Url = FTUConfig::Get()->ServerURL + TEXT("/1.1/users/me");
	HttpRequest->SetURL(Url);

	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLeanCloudSyncString());
	HttpRequest->SetHeader(TEXT("X-LC-Session"), SessionToken);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FTapBootstrapImplPC::OnUserInfoCallback, OnSuccess, OnError);

	HttpRequest->ProcessRequest();
}

void FTapBootstrapImplPC::LoginWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData,
                                            FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	if (Platform.IsEmpty())
	{
		OnError.ExecuteIfBound(FTUError(-1, TEXT("Platform expected.")));
		return;
	}
	if (!AuthData.IsValid() || AuthData->Values.Num() == 0)
	{
		OnError.ExecuteIfBound(FTUError(-1, TEXT("AuthData expected.")));
		return;
	}
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("POST"));

	const FString Url = FTUConfig::Get()->ServerURL + TEXT("/1.1/users");
	HttpRequest->SetURL(Url);

	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLeanCloudSyncString());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	TSharedPtr<FJsonObject> JsonObject1 = MakeShareable(new FJsonObject);
	JsonObject1->SetObjectField(Platform, AuthData);
	TSharedPtr<FJsonObject> JsonObject2 = MakeShareable(new FJsonObject);
	JsonObject2->SetObjectField("authData", JsonObject1);
	
	HttpRequest->SetContentAsString(TUJsonHelper::GetJsonString(JsonObject2));

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FTapBootstrapImplPC::OnUserInfoCallback, OnSuccess,
	                                                OnError);

	HttpRequest->ProcessRequest();
}

void FTapBootstrapImplPC::AssociateWithAuthData(const FTDSUser& InUser, const FString& Platform,
	TSharedPtr<FJsonObject> AuthData, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	if (Platform.IsEmpty())
	{
		OnError.ExecuteIfBound(FTUError(-1, TEXT("Platform expected.")));
		return;
	}
	if (!AuthData.IsValid() || AuthData->Values.Num() == 0)
	{
		OnError.ExecuteIfBound(FTUError(-1, TEXT("AuthData expected.")));
		return;
	}
	
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("PUT"));
	
	const FString Url = FTUConfig::Get()->ServerURL + TEXT("/1.1/users/") + InUser.GetObjectId();
	HttpRequest->SetURL(Url);
	
	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLeanCloudSyncString());
	HttpRequest->SetHeader(TEXT("X-LC-Session"), InUser.GetSessionToken());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	TSharedPtr<FJsonObject> JsonObject1 = MakeShareable(new FJsonObject);
	JsonObject1->SetObjectField(Platform, AuthData);
	TSharedPtr<FJsonObject> JsonObject2 = MakeShareable(new FJsonObject);
	JsonObject2->SetObjectField("authData", JsonObject1);
	HttpRequest->SetContentAsString(TUJsonHelper::GetJsonString(JsonObject2));
	
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FTapBootstrapImplPC::OnAssociateWithAuthDataCallback, Platform, AuthData, OnSuccess, OnError);

	HttpRequest->ProcessRequest();
}

void FTapBootstrapImplPC::DisassociateAuthData(const FTDSUser& InUser, const FString& Platform,
	FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	if (Platform.IsEmpty())
	{
		OnError.ExecuteIfBound(FTUError(-1, TEXT("Platform expected.")));
		return;
	}
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("PUT"));
	
	const FString Url = FTUConfig::Get()->ServerURL + TEXT("/1.1/users/") + InUser.GetObjectId();
	HttpRequest->SetURL(Url);
	
	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLeanCloudSyncString());
	HttpRequest->SetHeader(TEXT("X-LC-Session"), InUser.GetSessionToken());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString JsonString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);
	Writer->WriteObjectStart();
	Writer->WriteObjectStart(FString::Printf(TEXT("authData.%s"), *Platform));
	Writer->WriteValue(TEXT("__op"), TEXT("Delete"));
	Writer->WriteObjectEnd();
	Writer->WriteObjectEnd();
	Writer->Close();
	HttpRequest->SetContentAsString(JsonString);
	
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FTapBootstrapImplPC::OnDisassociateAuthDataCallback, Platform, OnSuccess, OnError);

	HttpRequest->ProcessRequest();
}

void FTapBootstrapImplPC::SaveUser(const FTDSUser& InUser) {
	auto ServerData = InUser.GetServerData();
	
	const FString FilePath = FPaths::SandboxesDir() + TEXT("/DataStorage/TDSUser");
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR> > JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonStr, 0);
	if (!FJsonSerializer::Serialize(ServerData, JsonWriter))
	{
		ensure(false);
		return;
	}
	JsonWriter->Close();
	
	FString Key;
	if (!GConfig->GetString(TEXT("/Script/TapCommon.TapSettings"), TEXT("SaveFileKey"), Key, GEngineIni))
	{
		Key = TEXT("tap_save_file_key");
	}
	TArray<uint8> Data = TUCrypto::AesEncode(TUCrypto::UTF8Encode(JsonStr), TUCrypto::UTF8Encode(Key));
	FFileHelper::SaveArrayToFile(Data, *FilePath);
}

void FTapBootstrapImplPC::OnUserInfoCallback(FHttpRequestPtr Request, FHttpResponsePtr Response,
                                             bool bConnectedSuccessfully, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (bConnectedSuccessfully && FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		int32 Code;
		FString ErrorMsg;
		if (JsonObject->TryGetStringField(TEXT("error"), ErrorMsg))
		{
			JsonObject->TryGetNumberField(TEXT("code"), Code);
			OnError.ExecuteIfBound(FTUError(Code, ErrorMsg));
		}
		else
		{
			CachedUser = MakeShared<FTDSUser>(JsonObject);
			auto UserPtr = CachedUser.Get();
			SaveUser(*UserPtr);
			OnSuccess.ExecuteIfBound(*UserPtr);
		}
	}
	else
	{
		OnError.ExecuteIfBound(FTUError(-1, "network error"));
	}
}

void FTapBootstrapImplPC::OnAssociateWithAuthDataCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, FString Platform, TSharedPtr<FJsonObject> AuthData, FTDSUser::FDelegate OnSuccess,
	FTUError::FDelegate OnError)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (bConnectedSuccessfully && FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		int32 Code;
		FString ErrorMsg;
		if (JsonObject->TryGetStringField(TEXT("error"), ErrorMsg))
		{
			JsonObject->TryGetNumberField(TEXT("code"), Code);
			OnError.ExecuteIfBound(FTUError(Code, ErrorMsg));
		}
		else
		{
			if (CachedUser)
			{
				if (TSharedPtr<FJsonObject> Data = CachedUser->GetServerData())
				{
					if (TSharedPtr<FJsonValue> AuthDataValue = Data->GetField<EJson::Object>(TEXT("authData")))
					{
						AuthDataValue->AsObject()->SetObjectField(Platform, AuthData);
						if (TSharedPtr<FJsonValue> UpdateAt = JsonObject->GetField<EJson::None>(TEXT("updatedAt")))
						{
							Data->SetField(TEXT("updatedAt"), UpdateAt);
						}
						if (TSharedPtr<FJsonValue> ObjectId = JsonObject->GetField<EJson::None>(TEXT("objectId")))
						{
							Data->SetField(TEXT("objectId"), ObjectId);
						}
						SaveUser(*CachedUser);
						OnSuccess.ExecuteIfBound(*CachedUser);
					}
				}
			}
		}
	}
	else
	{
		OnError.ExecuteIfBound(FTUError(-1, "network error"));
	}
}

void FTapBootstrapImplPC::OnDisassociateAuthDataCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, FString Platform, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (bConnectedSuccessfully && FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		int32 Code;
		FString ErrorMsg;
		if (JsonObject->TryGetStringField(TEXT("error"), ErrorMsg))
		{
			JsonObject->TryGetNumberField(TEXT("code"), Code);
			OnError.ExecuteIfBound(FTUError(Code, ErrorMsg));
		}
		else
		{
			if (CachedUser)
			{
				if (TSharedPtr<FJsonObject> Data = CachedUser->GetServerData())
				{
					if (TSharedPtr<FJsonValue> AuthDataValue = Data->GetField<EJson::Object>(TEXT("authData")))
					{
						AuthDataValue->AsObject()->RemoveField(Platform);
						if (TSharedPtr<FJsonValue> UpdateAt = JsonObject->GetField<EJson::None>(TEXT("updatedAt")))
						{
							Data->SetField(TEXT("updatedAt"), UpdateAt);
						}
						if (TSharedPtr<FJsonValue> ObjectId = JsonObject->GetField<EJson::None>(TEXT("objectId")))
						{
							Data->SetField(TEXT("objectId"), ObjectId);
						}
						SaveUser(*CachedUser);
						OnSuccess.ExecuteIfBound(*CachedUser);
					}
				}
			}
		}
	}
	else
	{
		OnError.ExecuteIfBound(FTUError(-1, "network error"));
	}
}

void FTapBootstrapImplPC::InternalLoginWithTapTap(const TSharedPtr<FTUAccessToken>& TapAccessToken,
                                                  const TSharedPtr<FTULoginProfileModel>& TapProfile, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	TSharedPtr<FJsonObject> AuthData = MakeShareable(new FJsonObject);
	AuthData->SetStringField(TEXT("access_token"), TapAccessToken->access_token);
	AuthData->SetStringField(TEXT("kid"), TapAccessToken->kid);
	AuthData->SetStringField(TEXT("mac_key"), TapAccessToken->mac_key);
	AuthData->SetStringField(TEXT("token_type"), TapAccessToken->token_type);
	AuthData->SetStringField(TEXT("mac_algorithm"), TapAccessToken->mac_algorithm);
	AuthData->SetStringField(TEXT("openid"), TapProfile->openid);
	AuthData->SetStringField(TEXT("unionid"), TapProfile->unionid);
	AuthData->SetStringField(TEXT("name"), TapProfile->name);
	AuthData->SetStringField(TEXT("avatar"), TapProfile->avatar);
	LoginWithAuthData("taptap", AuthData, OnSuccess, OnError);
}
	
