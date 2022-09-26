#include "AAUChinaRealName.h"
#include "AAUChinaImpl.h"
#include "Server/AAUHelper.h"
#include "Server/AAUNet.h"


void AAUChinaRealName::ManualVerify(const FString& UserID, const FString& Name, const FString& CardID,
                                    TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)>
                                    CallBack) {
	auto StandAloneVerifyBlock = [=]() {
		if (!CallBack) {
			return;
		}
		if (!AAUHelper::ValidateCardID(CardID) || Name.IsEmpty()) {
			CallBack(nullptr, FTUError(-1, TEXT("身份证号码非法")));
			return;
		}
		const int Age = AAUHelper::GetAge(CardID);
		TSharedPtr<FAAURealNameResultModel> ModelPtr = MakeShareable(new FAAURealNameResultModel);
		if (Age >= 0 && Age < 8) {
			ModelPtr->age_limit = (int)EAAUAgeLimit::Child;
		}
		else if (Age >= 8 && Age < 16) {
			ModelPtr->age_limit = (int)EAAUAgeLimit::Teen;
		}
		else if (Age >= 16 && Age < 18) {
			ModelPtr->age_limit = (int)EAAUAgeLimit::Young;
		}
		else if (Age >= 18 && Age < 200) {
			ModelPtr->age_limit = (int)EAAUAgeLimit::Adult;
		}
		else {
			CallBack(nullptr, FTUError(-1, TEXT("身份证号码非法")));
			return;
		}
		ModelPtr->anti_addiction_token = AAUChinaImpl::LocalTokenString;
		ModelPtr->status = FAAURealNameResultModel::Success;
		CallBack(ModelPtr, FTUError());
	};
	
	if (AAUImpl::Get()->IsNeedStandAlone()) {
		StandAloneVerifyBlock();
		return;
	}
	AAUNet::ChinaManualVerify(UserID, Name, CardID, [=](TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error) {
		if (!CallBack) {
			return;
		}
		if (ModelPtr.IsValid()) {
			CallBack(ModelPtr, Error);
		} else {
			AAUImpl::Get()->IsNeedStandAlone([=](bool IsNeed) {
				if (IsNeed) {
					StandAloneVerifyBlock();
				} else {
					CallBack(ModelPtr, Error);
				}
			});
		}
	});
}

void AAUChinaRealName::CheckRealNameState(const FString& UserID,
	TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack) {
	AAUNet::CheckRealNameState(UserID, CallBack);
}
