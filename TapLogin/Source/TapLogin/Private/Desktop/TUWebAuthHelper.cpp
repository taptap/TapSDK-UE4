#include "TUWebAuthHelper.h"

#include "HttpServerModule.h"
#include "HttpServerResponse.h"
#include "TULoginImpl.h"
#include "TUCrypto.h"
#include "TUHelper.h"
#include "TUDebuger.h"
#include "TUHttpServer.h"
#include "Server/TULoginRegionConfig.h"
#include "TUOpenSSL.h"

static FString WebAuthPath = "authorize";

TUWebAuthHelper::TUWebAuthHelper() {
	State = FGuid::NewGuid().ToString();
	CodeVerifier = TUHelper::GetRandomStr(16);
}

TUWebAuthHelper::~TUWebAuthHelper() {
	StopProcess();
}

bool TUWebAuthHelper::ProcessWebAuth(const TArray<FString>& NeedPermissions, TFunction<void(FString WebCode)> CallBack)
{
	this->Permissions = NeedPermissions;
	this->CallBackBlock = CallBack;
	TWeakPtr<TUWebAuthHelper> WeakThisPtr(AsShared());
	auto Callback = [=](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
	{
		TUDebuger::DisplayLog("taptap web call back success");
		TSharedPtr<TUWebAuthHelper> SharedThis = WeakThisPtr.Pin();
		if (!SharedThis.IsValid()) { return false; }
		SharedThis->ProcessWebAuthRequest(Request, OnComplete);
		return true;
	};
	FString TempRedirectUri = TUHttpServer::RegisterNewRoute(WebAuthPath, Callback);
	if (TempRedirectUri.IsEmpty())
	{
		return false;
	}
	else
	{
		RedirectUri = TempRedirectUri;
		TUHelper::LaunchURL(*GenerateWebAuthUrl(), nullptr, nullptr);
		return true;
	}
}

void TUWebAuthHelper::StopProcess() {
	TUHttpServer::RegisterNewRoute(WebAuthPath, nullptr);
}

FString TUWebAuthHelper::GetCodeVerifier() const {
	return CodeVerifier;
}

void TUWebAuthHelper::ProcessWebAuthRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) {
	FString WebState = Request.QueryParams.FindRef("state");
	FString WebCode = Request.QueryParams.FindRef("code");
	FString WebMode = Request.QueryParams.FindRef("mode");
	TUniquePtr<FHttpServerResponse> ResponsePtr;
	if (WebState != State || WebCode.IsEmpty()) {
		ResponsePtr = FHttpServerResponse::Error(EHttpServerResponseCodes::NotFound);
	} else {
		//code..
		if (WebMode == "gif") {
			ResponsePtr = MakeUnique<FHttpServerResponse>();
			ResponsePtr->Code = EHttpServerResponseCodes::Ok;
			ResponsePtr->Headers.Add("Content-Type", {"image/gif"});
			ResponsePtr->Headers.Add("Access-Control-Allow-Origin", {"*"});
			FString GifBase64Str = "R0lGODlhAQABAIABAAAAAP///yH5BAEAAAEALAAAAAABAAEAAAICTAEAOw==";
			ResponsePtr->Body.Append(TUCrypto::Base64Decode(GifBase64Str));
		} else if (WebMode == "redirect") {
			ResponsePtr = MakeUnique<FHttpServerResponse>();
			ResponsePtr->Code = EHttpServerResponseCodes::Moved;
			ResponsePtr->Headers.Add("Location", {FString::Printf(TEXT("open-taptap-%s://authorize"), *TULoginImpl::Get()->Config.ClientID)});
		} else {
			ResponsePtr = MakeUnique<FHttpServerResponse>();
			ResponsePtr->Code = EHttpServerResponseCodes::Ok;
			ResponsePtr->Headers.Add("Content-Type", {"text/plain"});
			ResponsePtr->Headers.Add("Access-Control-Allow-Origin", {"*"});
			ResponsePtr->Body.Append(TUCrypto::UTF8Encode(FString("ok")));
		}
		if (CallBackBlock) {
			CallBackBlock(WebCode);
		}
#if PLATFORM_MAC || PLATFORM_WINDOWS
		TUHelper::ActivateItself();
#endif
	}
	OnComplete(MoveTemp(ResponsePtr));
}

FString TUWebAuthHelper::GenerateWebAuthUrl() {
	TSharedPtr<FJsonObject> Paras = MakeShareable(new FJsonObject);
	Paras->SetStringField("client_id", TULoginImpl::Get()->Config.ClientID);
	Paras->SetStringField("response_type", "code");
	Paras->SetStringField("redirect_uri", RedirectUri);
	Paras->SetStringField("state", State);
	Paras->SetStringField("code_challenge", TUCrypto::UrlBase64Encode(TUOpenSSL::Sha256(TUCrypto::UTF8Encode(CodeVerifier))));
	Paras->SetStringField("code_challenge_method", "S256");
	Paras->SetStringField("scope", FString::Join(Permissions, TEXT(",")));
	Paras->SetStringField("flow", "pc_localhost");
	FString ParaStr = TUHelper::CombinParameters(Paras);
	return TULoginRegionConfig::Get()->AccountUrl() + "?" + ParaStr;
}
