#pragma once
#include "TUError.h"
#include "Model/AAURealNameResultModel.h"

class AAUVietnamRealName {
public:

	static void ManualVerify(const FString& UserID, const FString& Year, const FString& Mouth, const FString& Day,
	TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack);

	static void CheckRealNameState(const FString& UserID, TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack);
	
};
