#include "TUDBNet.h"

#include "TUDBRegionConfig.h"
#include "TUDebuger.h"
#include "TUHttpManager.h"

TQueue<TSharedPtr<TUDBNet>> TUDBNet::RequestQueue;
FTimerHandle TUDBNet::RetryTimerHandle = FTimerHandle();
int TUDBNet::CacheCount = 0;

void TUDBNet::SendEvent(TSharedPtr<FJsonObject> Paras, TFunction<void()> SuccessBlock, TFunction<void()> FailBlock) {
	SendEvent(TUDBRegionConfig::Get()->GetEventUrl(), Paras, SuccessBlock, FailBlock);
}

TUDBNet::~TUDBNet() {
	TUDebuger::DisplayLog("request is release");
}

void TUDBNet::SendEvent(const FString& Url, TSharedPtr<FJsonObject> Paras, TFunction<void()> SuccessBlock,
                        TFunction<void()> FailBlock) {
	const TSharedPtr<TUDBNet> request = MakeShareable(new TUDBNet);
	request->URL = Url;
	request->Type = Post;
	request->Form = Json;
	request->Parameters = Paras;
	TUDBNet *RequestPtr = request.Get();
	// 查下有没有循环引用
	request->onCompleted.BindLambda([RequestPtr, SuccessBlock, FailBlock](TSharedPtr<TUHttpResponse> Response) {
		if (RequestPtr == nullptr) {
			return;
		}
		if (RequestQueue.IsEmpty()) {
			return;
		}
		TSharedPtr<TUDBNet> LastRequest = *RequestQueue.Peek();
		if (LastRequest.Get() != RequestPtr) {
			return;
		}
		if (Response->state == TUHttpResponse::success || Response->state == TUHttpResponse::clientError)
		{
			if (Response->state == TUHttpResponse::success && SuccessBlock) {
				SuccessBlock();
			} else if (Response->httpCode == 302 || Response->httpCode == 307) {
				FString Location;
				FString Prefix = "Location: ";
				for (auto Header : Response->headers) {
					if (Header.StartsWith(Prefix)) {
						Location = Header.LeftChop(Prefix.Len());
						break;
					}
				}
				if (!Location.IsEmpty()) {
					PerformRequest(LastRequest);
					return;
				}
			} else if (Response->state == TUHttpResponse::clientError && FailBlock) {
				FailBlock();
			}
			// 去掉队列的第一个，执行下一个请求

			RequestQueue.Pop();
			CacheCount--;
			
			if (!RequestQueue.IsEmpty())
			{
				PerformRequest(LastRequest);
			}
		} else
		{
			// 如果请求失败，等待2秒无限重试；
			if (GWorld)
			{
				GWorld->GetWorld()->GetTimerManager().SetTimer(RetryTimerHandle, [=]()
				{
					PerformRequest(LastRequest);
				}, 2, false);
			}
		}
	});
	PerformRequest(request);
}

void TUDBNet::PerformRequest(const TSharedPtr<TUDBNet>& Request)
{
	if (!Request.IsValid())
	{
		return;
	}
	// 如果缓存的请求超过500个，那么直接清空队列，执行新的请求。
	if (CacheCount >= 500)
	{
		if (GWorld)
		{
			GWorld->GetWorld()->GetTimerManager().ClearTimer(RetryTimerHandle);
		}
		RequestQueue.Empty();
		CacheCount = 0;
	}
	// 如果需要执行的请求不是队列的第一个，那么先加到队列里
	if (RequestQueue.IsEmpty() || Request != *RequestQueue.Peek())
	{
		RequestQueue.Enqueue(Request);
		CacheCount++;
	}
	// 如果加到队列里之后，队列的第一个请求还不是要执行的请求，说明有其他请求正在执行，等着执行就行
	if (Request != *RequestQueue.Peek())
	{
		return;
	}
	TUHttpManager::Get().request(Request);
	if (CacheCount >= 1)
	{
		TUDebuger::WarningLog(FString::Printf(TEXT("TapDB 缓存请求数量：%d"), CacheCount));
	}
}
