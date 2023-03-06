#pragma once

#include "TUDBEnum.h"
#include "TUHttpRequest.h"



class TUDBNet: public TUHttpRequest
{
	
public:

	static void SendEvent(TSharedPtr<FJsonObject> Paras, TFunction<void()> SuccessBlock = nullptr, TFunction<void()> FailBlock = nullptr);

	static int CacheCount;
	virtual ~TUDBNet() override;
private:

	static void SendEvent(const FString& Url, TSharedPtr<FJsonObject> Paras, TFunction<void()> SuccessBlock, TFunction<void()> FailBlock);

	static void PerformRequest(const TSharedPtr<TUDBNet>& Request);

	static TQueue<TSharedPtr<TUDBNet>> RequestQueue;

	static FTimerHandle RetryTimerHandle;


};
