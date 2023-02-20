#include "TUSupportNet.h"

#include "TUHttpManager.h"
#include "TUSupportImpl.h"

TUSupportNet::TUSupportNet() {
	
}

void TUSupportNet::FetchUnReadStatus(ESupportLoginType LoginType, const FString& UserID,
	TFunction<void(const FString& Model, const FTUError& Error)> Callback) {
	const TSharedPtr<TUSupportNet> request = MakeShareable(new TUSupportNet());
	request->Type = Get;
	request->URL = TUSupportImpl::Get()->Config.ServerUrl / "/api/2/unread";
	switch (LoginType)
	{
	case ESupportLoginType::None:
		return;
	case ESupportLoginType::Anonymous: 
		request->Headers.Add("X-Anonymous-ID", UserID);
	break;
	case ESupportLoginType::Custom: 
		request->Headers.Add("X-Credential", UserID);
	break;
	case ESupportLoginType::TDS: 
		request->Headers.Add("X-TDS-Credential", UserID);
	break;
	default: ;
	}
	request->Parameters->SetStringField("product", TUSupportImpl::Get()->Config.ProductID);
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> Response) {
		if (!Callback) {
			return;
		}
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->contentString);
		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			FTUError Error = FTUError();
			JsonObject->TryGetNumberField(TEXT("numCode"), Error.code);
			JsonObject->TryGetStringField(TEXT("message"), Error.msg);
			JsonObject->TryGetStringField(TEXT("code"), Error.error_description);
			Callback(TEXT(""), Error);
		}
		else
		{
			Callback(Response->contentString, FTUError());
		}
	});
	TUHttpManager::Get().request(request);
}
