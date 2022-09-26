#pragma once

class TAPCOMMON_API TUMobileBridge {
public:

	// 同步返回原生过来的result，会卡游戏主线程，如果不需要及时返回result，那么请调用AsyncPerform
	static FString Perform(
		const FString& ServiceName,
		const FString& Method,
		const FString& Args);
	
	static void AsyncPerform(
		const FString& ServiceName,
		const FString& Method,
		const FString& Args,
		TFunction<void(const FString& ResultJsonStr)> CallBack = nullptr,
		bool OnceTime = true);

	static void DoCallBack(const FString& JsonStr);
	
	static void Register(const FString& ServiceClz, const FString& ServiceImpl);

	
private:
	TUMobileBridge();
	~TUMobileBridge();
	static TUMobileBridge &Get();
	static TUMobileBridge *Instance;

	TMap<FString, TFunction<void(const FString& ResultJsonStr)>> CallBackMap;
	uint64 PerformCount = 0;//也作为唯一标识
	FEvent* WaitEvent = nullptr;
};
