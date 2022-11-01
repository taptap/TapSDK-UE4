#pragma once
#include "TUError.h"
#include "TUHttpRequest.h"

class TUSupportNet: public TUHttpRequest {
public:

	TUSupportNet();
	
	static void FetchUnReadStatus(const FString& UserID, TFunction<void(const FString& Model, const FTUError& Error)> Callback);
	
};
