#include "TUHttpManager.h"

#include "Http.h"
#include "TUJsonHelper.h"
#include "TUHelper.h"

TUHttpManager* TUHttpManager::Singleton = nullptr;

TUHttpManager& TUHttpManager::Get()
{
	if (Singleton == nullptr)
	{
		check(IsInGameThread());
		Singleton = new TUHttpManager();
	}
	return *Singleton;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> GenerateRequest(TSharedPtr<TUHttpRequest> tdsReq)
{
	//This is the url on which to process the request
	
	TMap<FString, FString> headers;
	TSharedPtr<FJsonObject> parameters = MakeShareable(new FJsonObject);
	if (!tdsReq->isPure)
	{
		headers.Append(tdsReq->CommonHeaders());
		TSharedPtr<FJsonObject> CommonParameters = tdsReq->CommonParameters();
		if (CommonParameters != nullptr)
		{
			parameters->Values.Append(CommonParameters->Values);
		}
	}
	headers.Append(tdsReq->Headers);
	parameters->Values.Append(tdsReq->Parameters->Values);
	tdsReq->Headers = headers;
	tdsReq->Parameters = parameters;
	
	auto Request = FHttpModule::Get().CreateRequest();
	Request->SetTimeout(tdsReq->TimeoutSecs);
	for (auto header : tdsReq->Headers)
	{
		Request->SetHeader(header.Key, header.Value);
	}
	
	FString URL = TUDebuger::GetReplacedUrl(tdsReq->URL);
	for (auto PathParameter : tdsReq->PathParameters) {
		FString PathValue = FGenericPlatformHttp::UrlEncode(PathParameter.Value);
		URL.ReplaceInline(*PathParameter.Key, *PathValue, ESearchCase::CaseSensitive);
	}
	
	switch (tdsReq->Type)
	{
	case TUHttpRequest::Type::Get:
		{
			Request->SetVerb("GET");
			FString queryString = TUHelper::CombinParameters(tdsReq->Parameters);
			if (queryString.Len() > 0)
			{
				URL = URL + "?" + queryString;
			}
			Request->SetURL(URL);
		}
		break;
	case TUHttpRequest::Type::Post:
		{
			Request->SetVerb("POST");
			FString queryString = TUHelper::CombinParameters(tdsReq->PostUrlParameters);
			if (queryString.Len() > 0)
			{
				URL = URL + "?" + queryString;
			}
			Request->SetURL(URL);
			if (tdsReq->PostBodyString.IsEmpty()) {
				if (tdsReq->Form == TUHttpRequest::Form::Default)
				{
					FString body = TUHelper::CombinParameters(tdsReq->Parameters);
					Request->SetContentAsString(body);
				} else if (tdsReq->Form == TUHttpRequest::Form::Json)
				{
					FString body = TUJsonHelper::GetJsonString(tdsReq->Parameters);
					Request->SetContentAsString(body);
				}
			} else {
				Request->SetContentAsString(tdsReq->PostBodyString);
			}
		}
		break;
	}
	return Request;
}

void TUHttpManager::request(TSharedPtr<TUHttpRequest> tdsReq)
{
	auto Request = GenerateRequest(tdsReq);
	tdsReq->FinalURL = Request->GetURL();
	if (tdsReq->ResetHeadersBeforeRequest())
	{
		for (auto header : tdsReq->Headers)
		{
			Request->SetHeader(header.Key, header.Value);
		}
	}
	Request->OnProcessRequestComplete().BindLambda(
		[=](FHttpRequestPtr HttpRequest, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			tdsReq->tryCount++;
			if (bWasSuccessful == false || Response->GetResponseCode() >= 500)
			{
				if (tdsReq->tryCount < tdsReq->RepeatCount)
				{
					request(tdsReq);
					return;
				}
			}
			TSharedPtr<TUHttpResponse> tdsRes(new TUHttpResponse);
			tdsRes->request = tdsReq;
			if (bWasSuccessful)
			{
				tdsRes->httpCode = Response->GetResponseCode();
				tdsRes->contentString = Response->GetContentAsString();
				tdsRes->headers = Response->GetAllHeaders();
				if (EHttpResponseCodes::IsOk(tdsRes->httpCode))
				{
					tdsRes->state = TUHttpResponse::success;
				} else if (tdsRes->httpCode >= 500)
				{
					tdsRes->state = TUHttpResponse::serverError;
				} else
				{
					tdsRes->state = TUHttpResponse::clientError;
				}
			} else
			{
				tdsRes->state = TUHttpResponse::networkError;
			}
			UE_LOG(TDUHttpLog, Display, TEXT("%s"), *tdsRes->GenerateDebugString());
			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				tdsReq->onCompleted.ExecuteIfBound(tdsRes);
			});
		});
	Request->ProcessRequest();
}
