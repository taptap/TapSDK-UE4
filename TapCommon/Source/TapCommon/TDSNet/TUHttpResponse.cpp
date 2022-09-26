#include "TUHttpResponse.h"

#include "TUJsonHelper.h"
#include "TUHttpRequest.h"

// TDUHttpResponse::~TDUHttpResponse()
// {
// 	UE_LOG(LogTemp, Display, TEXT("TDUHttpResponse销毁了-----"));
// }

FString TUHttpResponse::GenerateDebugString()
{
	FString DebugString;
	if (request == nullptr)
	{
		return DebugString;
	}
	DebugString += "------------------------------------------\n";
	DebugString += (request->Type == TUHttpRequest::Post ? "POST: " : "GET: ") + request->GetFinalUrl() + "\n";
	if (request->Headers.Num() > 0)
	{
		DebugString += "Headers:\n";
		for (auto Header : request->Headers)
		{
			DebugString += "\t" + Header.Key + ": " + Header.Value + "\n";
		}
	}
	if (request->Type == TUHttpRequest::Post)
	{
		if (!request->PostBodyString.IsEmpty()) {
			DebugString += "Body:\n";
			DebugString += request->PostBodyString + "\n";
		} else if (request->Parameters->Values.Num() > 0) {
			FString body = TUJsonHelper::GetJsonString(request->Parameters);
			DebugString += "Body:\n";
			DebugString += body + "\n";
		}
	}

	if (state == networkError)
	{
		DebugString += "Network Error, please check network connection\n";
	} else
	{
		DebugString += "HttpCode: " + FString::FromInt(httpCode) + "\n";
		DebugString += "Response:\n" + contentString + "\n";
	}
	DebugString += "------------------------------------------";
	return DebugString;
}
