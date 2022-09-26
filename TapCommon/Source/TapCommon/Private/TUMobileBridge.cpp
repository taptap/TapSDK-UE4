#include "TUMobileBridge.h"
#include "TapBridge.h"
#include "TapResult.h"
#include "TUDebuger.h"
#include "TUJsonHelper.h"

TUMobileBridge * TUMobileBridge::Instance = nullptr;

FString TUMobileBridge::Perform(const FString& ServiceName, const FString& Method, const FString& Args) {
	// check(IsInGameThread());
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("service"), ServiceName);
	Writer->WriteValue(TEXT("method"), Method);
	Writer->WriteValue(TEXT("args"), Args);
	Writer->WriteValue(TEXT("callback"), true);
	FString CallbackId = FString::Printf(TEXT("%lld"), Get().PerformCount++);
	Writer->WriteValue(TEXT("callbackId"), CallbackId);
	Writer->WriteValue(TEXT("onceTime"), true);
	Writer->WriteObjectEnd();
	Writer->Close();
	FString ResultStr;
	Get().CallBackMap.Add(CallbackId, [=, &ResultStr](const FString& ResultJsonStr) {
		ResultStr = ResultJsonStr;
		// TUDebuger::DisplayLog(FString::Printf(TEXT("PerformResultStr-%s"), *ResultStr));
		if (Get().WaitEvent) {
			Get().WaitEvent->Trigger();
		}
	});
	// TUDebuger::DisplayLog(JsonOutString);
	GetBridge()->CallHandler(JsonOutString);
	Get().WaitEvent->Wait();
	// TUDebuger::DisplayLog(FString::Printf(TEXT("PerformResultStr-%s"), *ResultStr));
	return ResultStr;
}

void TUMobileBridge::AsyncPerform(const FString& ServiceName, const FString& Method, const FString& Args,
	TFunction<void(const FString& ResultJsonStr)> CallBack, bool OnceTime) {
	// check(IsInGameThread());
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("service"), ServiceName);
	Writer->WriteValue(TEXT("method"), Method);
	Writer->WriteValue(TEXT("args"), Args);
	Writer->WriteValue(TEXT("callback"), CallBack != nullptr);
	FString CallbackId = FString::Printf(TEXT("%lld"), Get().PerformCount++);
	Writer->WriteValue(TEXT("callbackId"), CallbackId);
	Writer->WriteValue(TEXT("onceTime"), OnceTime);
	Writer->WriteObjectEnd();
	Writer->Close();

	if (CallBack != nullptr) {
		Get().CallBackMap.Add(CallbackId, [=](const FString& ResultJsonStr) {
			AsyncTask(ENamedThreads::GameThread, [=]() {
				CallBack(ResultJsonStr);
			});
		});
	}
	// TUDebuger::DisplayLog(JsonOutString);
	GetBridge()->CallHandler(JsonOutString);
}

void TUMobileBridge::DoCallBack(const FString& JsonStr) {
	TUDebuger::DisplayLog(JsonStr);
	auto ResultPtr = TUJsonHelper::GetUStruct<FTapResult>(JsonStr);
	if (!ResultPtr.IsValid()) {
		TUDebuger::ErrorLog("Mobile bridge call back error");
		return;
	}
	// if (ResultPtr->code != 0) {
	// 	TUDebuger::ErrorLog("Mobile bridge call back fail");
	// 	return;
	// }
	
	auto CallBackPtr = Get().CallBackMap.Find(ResultPtr->callbackId);
	if (CallBackPtr == nullptr) {
		return;
	}
	(*CallBackPtr)(ResultPtr->content);
	if (ResultPtr->onceTime) {
		AsyncTask(ENamedThreads::GameThread, [=]() {
			Get().CallBackMap.Remove(ResultPtr->callbackId);
		});
	}
}

void TUMobileBridge::Register(const FString& ServiceClz, const FString& ServiceImpl) {
	GetBridge()->Register(ServiceClz,ServiceImpl);
}

TUMobileBridge::TUMobileBridge() {
	WaitEvent = FPlatformProcess::GetSynchEventFromPool(false);
}

TUMobileBridge::~TUMobileBridge() {
	if (WaitEvent) {
		FPlatformProcess::ReturnSynchEventToPool(WaitEvent);
		WaitEvent = nullptr;
	}
}

TUMobileBridge& TUMobileBridge::Get() {
	if (Instance == nullptr) {
		Instance = new TUMobileBridge();
	}
	return *Instance;
}
