#include "AAUVietnamRealName.h"

#include "Server/AAUNet.h"

void AAUVietnamRealName::ManualVerify(const FString& UserID, const FString& Year, const FString& Mouth,
                                      const FString& Day, TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack) {
	AAUNet::VietnamManualVerify(UserID, Year, Mouth, Day, CallBack);
}

void AAUVietnamRealName::CheckRealNameState(const FString& UserID,
	TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack) {
	AAUNet::CheckRealNameState(UserID, CallBack);
}
