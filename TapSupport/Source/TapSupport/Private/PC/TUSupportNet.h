#pragma once
#include "TUError.h"
#include "TUHttpRequest.h"

enum class ESupportLoginType : uint8;

class TUSupportNet: public TUHttpRequest {
public:

	TUSupportNet();
	
	static void FetchUnReadStatus(ESupportLoginType LoginType, const FString& UserID, TFunction<void(const FString& Model, const FTUError& Error)> Callback);
	
};
