#include "TUHttpRequest.h"
#include "Http.h"


TUHttpRequest::TUHttpRequest()
{
	Parameters = MakeShareable(new FJsonObject);
	PostUrlParameters = MakeShareable(new FJsonObject);
}

FString TUHttpRequest::GetFinalUrl()
{
	return FinalURL;
}

TMap<FString, FString> TUHttpRequest::CommonHeaders()
{
	TMap<FString, FString> CommonHeaders;
	if (Form == Default)
	{
		CommonHeaders.Add("Content-Type", "application/x-www-form-urlencoded");
	} else if (Form == Json)
	{
		CommonHeaders.Add("Content-Type", "application/json");
	}
	return CommonHeaders;
}

TSharedPtr<FJsonObject> TUHttpRequest::CommonParameters()
{
	return MakeShareable(new FJsonObject);
}

// TDUHttpRequest::~TDUHttpRequest()
// {
// 	UE_LOG(LogTemp, Display, TEXT("TDUHttpRequest销毁了----"));
// }




