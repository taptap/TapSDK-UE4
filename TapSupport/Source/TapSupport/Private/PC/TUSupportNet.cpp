#include "TUSupportNet.h"

#include "TUHttpManager.h"
#include "TUSupportImpl.h"

TUSupportNet::TUSupportNet() {
	
}

void TUSupportNet::FetchUnReadStatus(const FString& UserID,
	TFunction<void(const FString& Model, const FTUError& Error)> Callback) {
	const TSharedPtr<TUSupportNet> request = MakeShareable(new TUSupportNet());
	request->Type = Get;
	request->URL = TUSupportImpl::Get()->Config.ServerUrl / "/api/2/unread";
	request->Headers.Add("X-Anonymous-ID", UserID);
	request->Parameters->SetStringField("product", TUSupportImpl::Get()->Config.ProductID);
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> Response) {
		if (!Callback) {
			return;
		}
		FTUError Error = FTUError();
		if (Response->state == TUHttpResponse::clientError) {
			Error.code = TUHttpResponse::clientError;
			Error.msg = "request fail";
		}
		else if (Response->state == TUHttpResponse::serverError) {
			Error.code = TUHttpResponse::serverError;
			Error.msg = "server error";
		}
		else if (Response->state == TUHttpResponse::networkError) {
			Error.code = TUHttpResponse::networkError;
			Error.msg = "network connection error";
		}
		if (Error.msg.IsEmpty()) {
			Callback(Response->contentString, Error);
		} else {
			Callback("", Error);
		}
	});
	TUHttpManager::Get().request(request);
}
