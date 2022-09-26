#include "TULoginNet.h"
#include "TUDeviceInfo.h"
#include "TUJsonHelper.h"
#include "TUCrypto.h"
#include "TUHelper.h"
#include "TUHttpManager.h"
#include "TULoginRegionConfig.h"
#include "URLParser.h"
#include "TULoginImpl.h"

TULoginNet::TULoginNet()
{
}

FTULoginError TULoginNet::GenerateErrorInfo(const TSharedPtr<TUHttpResponse>& Response)
{
	FTULoginError Error = FTULoginError();
	if (Response->state == TUHttpResponse::clientError)
	{
		Error.code = TUHttpResponse::clientError;
		Error.msg = "request fail";
	} else if (Response->state == TUHttpResponse::serverError)
	{
		Error.code = TUHttpResponse::serverError;
		Error.msg = "server error";
	} else if (Response->state == TUHttpResponse::networkError)
	{
		Error.code = TUHttpResponse::networkError;
		Error.msg = "network connection error";
	}
	return Error;
}

template <typename StructType>
void PerfromWrapperResponseCallBack(const TSharedPtr<TUHttpResponse>& Response, TFunction<void(TSharedPtr<StructType> Model, FTULoginError Error)> Callback)
{
	if (Callback == nullptr)
	{
		return;
	}
	FTULoginError Error = TULoginNet::GenerateErrorInfo(Response);

	auto JsonObject = TUJsonHelper::GetJsonObject(Response->contentString);
	bool Success = false;
	const TSharedPtr<FJsonObject>* DataJsonObject = nullptr;
	if (JsonObject.IsValid() && JsonObject->TryGetBoolField("success", Success) && JsonObject->TryGetObjectField("data", DataJsonObject))
	{
		if (Success)
		{
			auto Model = TUJsonHelper::GetUStruct<StructType>(*DataJsonObject);
			if (Model.IsValid())
			{
				Callback(Model, Error);
				return;
			}
		} else
		{
			auto Model = TUJsonHelper::GetUStruct<FTULoginError>(*DataJsonObject);
			if (Model.IsValid())
			{
				Error = *Model.Get();
			}
		}
	}

	Callback(nullptr, Error);
}

void TULoginNet::RequestLoginQrCode(const TArray<FString> Permissions,
	TFunction<void(TSharedPtr<FTUQrCodeModel> Model, FTULoginError Error)> callback)
{
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new TULoginNet());
	request->Type = Post;
	request->URL = TULoginRegionConfig::Get()->CodeUrl();
	request->Parameters->SetStringField("client_id", TULoginImpl::Get()->Config.ClientID);
	request->Parameters->SetStringField("response_type", "device_code");
	request->Parameters->SetStringField("scope", FString::Join(Permissions, TEXT(",")));
	request->Parameters->SetStringField("version", TULoginRegionConfig::Get()->TapTapSDKVersion);
	request->Parameters->SetStringField("platform", "ue");
	request->Parameters->SetStringField("info", FString::Printf(TEXT("{\"device_id\":\"%s\"}"), *TUDeviceInfo::GetLoginId()));

	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void TULoginNet::RequestAccessToken(const FString& DeviceCode, TFunction<void(TSharedPtr<FTUAccessToken> Model, FTULoginError Error)> callback)
{
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new TULoginNet());
	request->Type = Post;
	request->URL = TULoginRegionConfig::Get()->TokenUrl();
	request->Parameters->SetStringField("grant_type", "device_token");
	request->Parameters->SetStringField("client_id", TULoginImpl::Get()->Config.ClientID);
	request->Parameters->SetStringField("secret_type", "hmac-sha-1");
	request->Parameters->SetStringField("code", DeviceCode);
	request->Parameters->SetStringField("version", "1.0");
	request->Parameters->SetStringField("platform", "ue");
	request->Parameters->SetStringField("info", FString::Printf(TEXT("{\"device_id\":\"%s\"}"), *TUDeviceInfo::GetLoginId()));

	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void TULoginNet::RequestProfile(const FTUAccessToken& AccessToken,
	TFunction<void(TSharedPtr<FTULoginProfileModel> Model, FTULoginError Error)> callback)
{
	const TSharedPtr<TULoginNet> request = MakeShareable(new TULoginNet());
	request->URL = TULoginRegionConfig::Get()->ProfileUrl();
	request->Parameters->SetStringField("client_id", TULoginImpl::Get()->Config.ClientID);
	request->AccessToken = MakeShareable(new FTUAccessToken(AccessToken));
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void TULoginNet::RequestAccessTokenFromWeb(const TSharedPtr<FJsonObject>& Paras,
	TFunction<void(TSharedPtr<FTUAccessToken> Model, FTULoginError Error)> callback) {
	const TSharedPtr<TULoginNet> request = MakeShareable(new TULoginNet());
	request->Type = Post;
	request->URL = TULoginRegionConfig::Get()->TokenUrl();
	request->Parameters = Paras;
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void TULoginNet::RequestTestQualification(
	TFunction<void(TSharedPtr<FTUTestQualificationModel> Model, FTULoginError Error)> Callback) {
	const TSharedPtr<TULoginNet> request = MakeShareable(new TULoginNet());
	request->Type = Get;
	request->URL = TULoginRegionConfig::Get()->TestQualificationUrl();
	request->Parameters->SetStringField("client_id", TULoginImpl::Get()->Config.ClientID);
	request->AccessToken = FTUAccessToken::GetLocalModel();
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, Callback);
	});
	TUHttpManager::Get().request(request);
}

TMap<FString, FString> TULoginNet::CommonHeaders()
{
	return TUHttpRequest::CommonHeaders();
}

TSharedPtr<FJsonObject> TULoginNet::CommonParameters()
{
	return TUHttpRequest::CommonParameters();
}

bool TULoginNet::ResetHeadersBeforeRequest()
{
	if (AccessToken.IsValid())
	{
		this->Headers.Add("Authorization", GetMacToken());
		return true;
	} else
	{
		return TUHttpRequest::ResetHeadersBeforeRequest();
	}
	
}

FString TULoginNet::GetMacToken()
{
	auto tokenModel = AccessToken;
	FString authToken;
	if (tokenModel == nullptr)
	{
		return authToken;
	}
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(this->GetFinalUrl());
	FString timeStr = FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp());
	FString nonce = TUHelper::GetRandomStr(5);
	// TUDebuger::DisplayLog(FString::Printf(TEXT("random: %s"), *nonce));
	FString md = this->Type == Get ? "GET" : "POST";

	FString pathAndQuery = Parse.GetPath();
	if (Parse.GetQuery().Len() > 0)
	{
		pathAndQuery += "?" + Parse.GetQuery();
	}
	FString domain = Parse.GetHost().ToLower();
	FString port = Parse.GetPort();

	FString dataStr = timeStr + "\n" + nonce + "\n" + md + "\n" + pathAndQuery + "\n" + domain + "\n" + port + "\n\n";
	auto sha1 = TUCrypto::HmacSHA1(TUCrypto::UTF8Encode(dataStr), TUCrypto::UTF8Encode(tokenModel->mac_key));
	FString mac = TUCrypto::Base64Encode(sha1);
	
	authToken = FString::Printf(TEXT("MAC id=\"%s\",ts=\"%s\",nonce=\"%s\",mac=\"%s\""), *tokenModel->kid, *timeStr, *nonce, *mac);
	return authToken;
}


