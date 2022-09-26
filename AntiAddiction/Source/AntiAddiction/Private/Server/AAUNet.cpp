#include "AAUNet.h"

#include "AAUImpl.h"
#include "AAURegionConfig.h"
#include "AntiAddiction.h"
#include "TUCrypto.h"
#include "TUDebuger.h"
#include "TUDeviceInfo.h"
#include "TUHttpManager.h"
#include "TUJsonHelper.h"
#include "TUOpenSSL.h"
#include "TUType.h"
#include "Model/AAUUser.h"


bool AAUNet::RealNameServerIsCrash = false;

FTUError GenerateErrorInfo(const TSharedPtr<TUHttpResponse>& Response)
{
	FTUError Error = FTUError();
	Error.code = Response->state;
	Error.error_description = TEXT("网络异常，请稍后重试");
	return Error;
}


void JudgeServerIsCrash(const TSharedPtr<TUHttpResponse>& Response) {
	if (Response->state == TUHttpResponse::serverError) {
		AAUNet::RealNameServerIsCrash = true;
	} else if (Response->contentString.IsEmpty() && Response->state != TUHttpResponse::networkError) {
		AAUNet::RealNameServerIsCrash = true;
	}
}

template <typename StructType>
void PerfromResponseCallBack(const TSharedPtr<TUHttpResponse>& Response,
							 TFunction<void(TSharedPtr<StructType> ModelPtr, const FTUError& Error)> Callback) {
	if (Callback == nullptr) {
		return;
	}
	FTUError Error = GenerateErrorInfo(Response);

	auto JsonObject = TUJsonHelper::GetUStruct<StructType>(Response->contentString);

	Callback(JsonObject, Error);
}

template <typename StructType>
void GenerateWrapperResponse(const TSharedPtr<TUHttpResponse>& Response, TSharedPtr<StructType>& ModelPtr, FTUError& Error)
{
	Error = GenerateErrorInfo(Response);

	if (Response->state == TUHttpResponse::serverError) {
		return;
	}

	const auto JsonObject = TUJsonHelper::GetJsonObject(Response->contentString);
	bool Success = false;
	const TSharedPtr<FJsonObject>* DataJsonObject = nullptr;
	if (JsonObject.IsValid() && JsonObject->TryGetBoolField("success", Success) && JsonObject->TryGetObjectField("data", DataJsonObject))
	{
		if (Success)
		{
			ModelPtr = TUJsonHelper::GetUStruct<StructType>(*DataJsonObject);
		} else
		{
			const auto ErrorPtr = TUJsonHelper::GetUStruct<FTUError>(*DataJsonObject);
			if (ErrorPtr.IsValid())
			{
				Error = *ErrorPtr.Get();
			}
		}
	}
}

template <typename StructType>
void PerfromWrapperResponseCallBack(const TSharedPtr<TUHttpResponse>& Response, TFunction<void(TSharedPtr<StructType> ModelPtr, const FTUError& Error)> Callback)
{
	if (Callback == nullptr)
	{
		return;
	}
	FTUError Error;
	TSharedPtr<StructType> ModelPtr = nullptr;
	GenerateWrapperResponse(Response, ModelPtr, Error);
	Callback(ModelPtr, Error);
}


AAUNet::AAUNet() {
	TimeoutSecs = 15;
	Form = Json;
}

/// 获取防沉迷静态配置，目前只有一个stand_alone_mode字段
void AAUNet::GetStaticSetting(TFunction<void(TSharedPtr<FAAUSettingsModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = AAURegionConfig::Get()->TDSPublicConfigUrl() / "/antiaddiction-settings.json";
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::GetServerTime(TFunction<void(TSharedPtr<FAAUServerTimeModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = AAURegionConfig::Get()->AntiAddictionUrl() / "server-time";
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}

template <typename ConfigType>
void CommonGetSDKConfig(TFunction<void(TSharedPtr<ConfigType> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = AAURegionConfig::Get()->AntiAddictionUrl() / "{region}/clients/{clients}/configuration" ;
	AAUNet::AddUriParas(request->URL, "", request->PathParameters);
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::GetSDKConfig(TFunction<void(TSharedPtr<FAAUChinaConfigModel> ModelPtr, const FTUError& Error)> CallBack) {
	CommonGetSDKConfig(CallBack);
}

void AAUNet::
GetSDKConfig(TFunction<void(TSharedPtr<FAAUVietnamConfigModel> ModelPtr, const FTUError& Error)> CallBack) {
	CommonGetSDKConfig(CallBack);
}

void AAUNet::SetPayment(int Amount, const FAAUUser& User,
                        TFunction<void(TSharedPtr<FAAUPaymentModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = AAURegionConfig::Get()->AntiAddictionUrl() / "{region}/clients/{clients}/users/{users}/payments";
	AddUriParas(request->URL, User.UserID, request->PathParameters);
	request->Type = Post;
	request->Headers.Add("Authorization", User.AccessToken);
	request->Parameters->SetNumberField("amount", Amount);
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::CheckPayment(int Amount, const FAAUUser& User,
	TFunction<void(TSharedPtr<FAAUPayableModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = AAURegionConfig::Get()->AntiAddictionUrl() / "{region}/clients/{clients}/users/{users}/payable";
	AddUriParas(request->URL, User.UserID, request->PathParameters);
	request->Type = Post;
	request->Headers.Add("Authorization", User.AccessToken);
	request->Parameters->SetNumberField("amount", Amount);
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::CheckPlayable(const FString& UserID, const FString& Token, TArray<TArray<int>> ServerTimes,
                           TArray<TArray<int>> LocalTimes,
                           TFunction<void(TSharedPtr<FAAUPlayableModel> ModelPtr, const FTUError& Error)> CallBack, bool IsLogin) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = AAURegionConfig::Get()->AntiAddictionUrl() / "{region}/clients/{clients}/users/{users}/playable";
	AddUriParas(request->URL, UserID, request->PathParameters);
	request->Type = Post;
	if (AAUImpl::Config.Region == EAAURegion::China) {
		request->RepeatCount = 3;
	}
	request->Headers.Add("Authorization", Token);

	FString JsonStr;
	TSharedRef <TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonStr);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue("game", AAUImpl::Get()->Config.ClientID);
	const FString Version = AntiAddictionUE_VERSION;
	JsonWriter->WriteValue("sdkVersion", Version);
	JsonWriter->WriteObjectStart("play_logs");
	JsonWriter->WriteArrayStart("server_times");
	for (auto ServerTime : ServerTimes) {
		JsonWriter->WriteArrayStart();
		for (auto Time : ServerTime) {
			JsonWriter->WriteValue(Time);
		}
		JsonWriter->WriteArrayEnd();
	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->WriteArrayStart("local_times");
	for (auto LocalTime : LocalTimes) {
		JsonWriter->WriteArrayStart();
		for (auto Time : LocalTime) {
			JsonWriter->WriteValue(Time);
		}
		JsonWriter->WriteArrayEnd();
	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->WriteObjectEnd();
	JsonWriter->WriteValue("is_login", IsLogin ? 1 : 0);
	JsonWriter->WriteObjectEnd(); 
	JsonWriter->Close();

	request->PostBodyString = JsonStr;
	auto object = TUJsonHelper::GetJsonObject(JsonStr);
	JsonStr = TUJsonHelper::GetJsonString(object);
	
	// TSharedPtr<FJsonObject> PlayLogsJsonObject = MakeShareable(new FJsonObject);
	// PlayLogsJsonObject->SetArrayField("server_times", ServerTimes);
	
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		JudgeServerIsCrash(response);
		PerfromWrapperResponseCallBack(response, CallBack);
	});
	TUHttpManager::Get().request(request);
}



void AAUNet::ChinaManualVerify(const FString& UserID, const FString& Name, const FString& CardID,
                          TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = AAURegionConfig::Get()->RealNameUrl() / "{region}/clients/{clients}/users/{users}/manual" ;
	AddUriParas(request->URL, UserID, request->PathParameters);
	request->Type = Post;
	if (AAUImpl::Config.Region == EAAURegion::China) {
		request->RepeatCount = 3;
	}
	
	TSharedPtr<FJsonObject> UserInfo = MakeShareable(new FJsonObject);
	UserInfo->SetStringField("name", Name);
	UserInfo->SetStringField("id_card", CardID);
	FString UserInfoStr = TUJsonHelper::GetJsonString(UserInfo);
	FString Key = TUDebuger::GetReplacedHost(AAURegionConfig::Get()->GetRSAPublicKey());

	auto EncryptData = TUOpenSSL::RSAEncryptPublic(TUCrypto::UTF8Encode(UserInfoStr), Key);
	request->Parameters->SetStringField("data", TUCrypto::Base64Encode(EncryptData));
	
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		JudgeServerIsCrash(response);
		FTUError Error;
		TSharedPtr<FAAURealNameResultModel> ModelPtr = nullptr;
		GenerateWrapperResponse(response, ModelPtr, Error);
		if (CallBack) {
			CallBack(ModelPtr, Error);
		}
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::VietnamManualVerify(const FString& UserID, const FString& Year, const FString& Mouth, const FString& Day,
	TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = AAURegionConfig::Get()->RealNameUrl() / "{region}/clients/{clients}/users/{users}/manual" ;
	AddUriParas(request->URL, UserID, request->PathParameters);
	request->Type = Post;
	
	TSharedPtr<FJsonObject> UserInfo = MakeShareable(new FJsonObject);
	UserInfo->SetStringField("birthDate", FString::Printf(TEXT("%s-%s-%s"), *Year, *Mouth, *Day));
	FString UserInfoStr = TUJsonHelper::GetJsonString(UserInfo);
	FString Key = TUDebuger::GetReplacedHost(AAURegionConfig::Get()->GetRSAPublicKey());

	auto EncryptData = TUOpenSSL::RSAEncryptPublic(TUCrypto::UTF8Encode(UserInfoStr), Key);
	request->Parameters->SetStringField("data", TUCrypto::Base64Encode(EncryptData));
	
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		JudgeServerIsCrash(response);
		FTUError Error;
		TSharedPtr<FAAURealNameResultModel> ModelPtr = nullptr;
		GenerateWrapperResponse(response, ModelPtr, Error);
		if (CallBack) {
			CallBack(ModelPtr, Error);
		}
	});
	TUHttpManager::Get().request(request);
}

void AAUNet::CheckRealNameState(const FString& UserID,
                                TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack) {
	const TSharedPtr<AAUNet> request = MakeShareable(new AAUNet());
	request->URL = AAURegionConfig::Get()->RealNameUrl() / "{region}/clients/{clients}/users/{users}";
	AddUriParas(request->URL, UserID, request->PathParameters);
	request->Type = Get;
	if (AAUImpl::Config.Region == EAAURegion::China) {
		request->RepeatCount = 3;
	}
	
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		JudgeServerIsCrash(response);
		FTUError Error;
		TSharedPtr<FAAURealNameResultModel> ModelPtr = nullptr;
		GenerateWrapperResponse(response, ModelPtr, Error);
		if (!ModelPtr.IsValid()) {
			if (response->state == TUHttpResponse::networkError) {
				Error.error_description = TEXT("网络异常，请稍后重试");
			}
			else {
				Error.error_description = TEXT("未查询到实名状态，防沉迷启动失败");
			}
		}
		if (CallBack) {
			CallBack(ModelPtr, Error);
		}
	});
	TUHttpManager::Get().request(request);
}

// "/v3/fcm/authorizations"  // POST



TMap<FString, FString> AAUNet::CommonHeaders() {
	TMap<FString, FString> HeadMap = TUHttpRequest::CommonHeaders();
	FString UAStr = FString::Printf(TEXT("Version=%s&Platform=%s&Unreal-SDK-Version=%s"), TEXT(AntiAddictionUE_VERSION), *TUDeviceInfo::GetPlatform(), *TUDeviceInfo::GetEngineVersion());
	HeadMap.Add("UA", UAStr);
	if (AAUImpl::Config.Region == EAAURegion::China) {
		HeadMap.Add("Accept-Language", "zh-CN");
	} else {
		HeadMap.Add("Accept-Language", "vi-VN");
	}
	return HeadMap;
	
	// [request setValue:[requestParams objectForKey:ANTI_AUTH_KEY] forHTTPHeaderField:ANTI_AUTH_KEY]; Authorization
}

TSharedPtr<FJsonObject> AAUNet::CommonParameters() {
	auto _Parameters = TUHttpRequest::CommonParameters();
	_Parameters->SetStringField("game", AAUImpl::Get()->Config.ClientID);
	_Parameters->SetStringField("sdkVersion", AntiAddictionUE_VERSION);
	return _Parameters;
}

bool AAUNet::ResetHeadersBeforeRequest() {
	return TUHttpRequest::ResetHeadersBeforeRequest();
}

void AAUNet::AddUriParas(const FString& Url, const FString& UserID, TMap<FString, FString>& PathParas) {
	if (Url.Contains("{clients}")) {
		PathParas.Add("{clients}", AAUImpl::Config.ClientID);
	}
	if (Url.Contains("{users}")) {
		PathParas.Add("{users}", UserID);
	}
	if (Url.Contains("{region}")) {
		FString Region = "";
		switch (AAUImpl::Config.Region) {
		case EAAURegion::China:
			Region = "china";
			break;
		case EAAURegion::Vietnam:
			Region = "vietnam";
			break;
		}
		PathParas.Add("{region}", Region);
	}
}



