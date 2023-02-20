#include "LCUser.h"

#include "Tools/LCJsonHelper.h"

FString FLCUser::KeyUserName = "username";
FString FLCUser::KeyPassword = "password";
FString FLCUser::KeyEmail = "email";
FString FLCUser::KeyMobilePhone = "mobilePhoneNumber";
FString FLCUser::KeyMobilePhoneVerified = "mobilePhoneVerified";
FString FLCUser::KeyAuthData = "authData";
FString FLCUser::KeyAuthDataAnonymous = "anonymous";
FString FLCUser::KeySessionToken = "sessionToken";
FString FLCUser::CLASS_NAME = "_User";

FString GetSyncString()
{
	const int64 Ticks = FDateTime::UtcNow().ToUnixTimestamp() * 1000;
	const FString TimeStr = FString::Printf(TEXT("%010lld"), Ticks);
	return FMD5::HashAnsiString(*(TimeStr + FLCConfig::Get().ClientToken)) + TEXT(",") + TimeStr;
}

FLCUser::FLCUser(TSharedPtr<FJsonObject> InServerData): FLCObject(CLASS_NAME, InServerData) {
}

FString FLCUser::GetEmail() const {
	return LCJsonHelper::GetStringField(ServerData, KeyEmail);
}

FString FLCUser::GetUsername() const {
	return LCJsonHelper::GetStringField(ServerData, KeyUserName);
}

FString FLCUser::GetPassword() const {
	return LCJsonHelper::GetStringField(ServerData, KeyPassword);
}

FString FLCUser::GetMobilePhoneNumber() const {
	return LCJsonHelper::GetStringField(ServerData, KeyMobilePhone);
}

FString FLCUser::GetSessionToken() const {
	return LCJsonHelper::GetStringField(ServerData, KeySessionToken);
}

bool FLCUser::IsAnonymous() const {
	auto AuthDataPtr = GetAuthData();
	if (!AuthDataPtr.IsValid()) {
		return false;
	}
	return AuthDataPtr->Values.Contains(KeyAuthDataAnonymous);
}

bool FLCUser::IsMobilePhoneVerified() const {
	return LCJsonHelper::GetBoolField(ServerData, KeyMobilePhoneVerified);
}

bool FLCUser::IsAuthenticated() const {
	return !GetSessionToken().IsEmpty();
}

void FLCUser::RetrieveShortToken(const FStringSignature& OnSuccess, const FLCError::FDelegate& OnFailed)
{
	if (TSharedPtr<FLCError> Error = FLCConfig::Get().CheckConfig())
	{
		OnFailed.ExecuteIfBound(*Error);
		return;
	}
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("GET"));

	FString Url = FLCConfig::Get().ServerURL + TEXT("/storage/1.1/users/tap-support/identity");
	HttpRequest->SetURL(Url);

	HttpRequest->SetHeader(TEXT("X-LC-Id"), FLCConfig::Get().ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetSyncString());
	HttpRequest->SetHeader(TEXT("X-LC-Session"), GetSessionToken());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	HttpRequest->OnProcessRequestComplete().BindSP(this, &FLCUser::OnRetrieveShortTokenCallback, OnSuccess, OnFailed);

	if (!HttpRequest->ProcessRequest())
	{
		OnFailed.ExecuteIfBound(FLCError(-1, TEXT("Net Error. http send failed.")));
	}
}

void FLCUser::OnRetrieveShortTokenCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, FStringSignature OnSuccess, FLCError::FDelegate OnError)
{
	if (bConnectedSuccessfully && Response && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FString ContentString = Response->GetContentAsString();
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ContentString);
		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			FString Token;
			if (JsonObject->TryGetStringField(TEXT("identityToken"), Token))
			{
				OnSuccess.ExecuteIfBound(Token);
				return;
			}
		}
		OnError.ExecuteIfBound(FLCError(-1, FString::Printf(TEXT("Net Error, http response content:%s."), *ContentString)));
		return;
	}
	OnError.ExecuteIfBound(FLCError(-1, TEXT("Net Error, Http failed.")));
}

TSharedPtr<FJsonObject> FLCUser::GetAuthData() const {
	const TSharedPtr<FJsonObject> * TempPtr = nullptr;
	ServerData->TryGetObjectField(KeyAuthData, TempPtr);
	return *TempPtr;
}
