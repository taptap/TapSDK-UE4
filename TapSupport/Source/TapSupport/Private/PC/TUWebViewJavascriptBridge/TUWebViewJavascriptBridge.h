#pragma once
#include "Misc/EmbeddedCommunication.h"

typedef TSharedPtr<FJsonObject> TUJSBridgeMessage;
typedef TFunction<void(TUJSBridgeMessage ResponseData)> TUJSBridgeResponseCallback;
typedef TFunction<void(TUJSBridgeMessage Data, TUJSBridgeResponseCallback ResponseCallback)> TUJSBridgeHandler;


class TUWebViewJavascriptBridge {
public:
	DECLARE_DELEGATE_TwoParams(FEvaluateJavascript , const FString&, TFunction<void(const FString&)>);

	FEvaluateJavascript OnEvaluateJavascript;
	
	TUWebViewJavascriptBridge();
	~TUWebViewJavascriptBridge();
	void RegisterHandler(const FString& HandlerName, TUJSBridgeHandler Handler);
	void RemoveHandler(const FString& HandlerName);
	bool IsHanderRegistered(const FString& HandlerName);
	void CallHandler(const FString& HandlerName,
		TUJSBridgeMessage Data = nullptr,
		TUJSBridgeResponseCallback ResponseCallback = nullptr);
	void Reset();
	void DisableJavscriptAlertBoxSafetyTimeout();
	void EnableLogging();

	// 如果为true，那么需要拦截跳转
	bool ShouldPerformBridgeAction(const FString& Url);

private:
	TMap<FString, TUJSBridgeHandler> MessageHandlers;
	TMap<FString, TUJSBridgeResponseCallback> ResponseCallbacks;
	TSharedPtr<TArray<TUJSBridgeMessage>> StartupMessageQueue;
	TUJSBridgeHandler MessageHandler;
	FDelegateHandle EmbeddedCommunicationHandle;
	int64 UniqueID = 0;
	

	void SendData(const FString& HandlerName, TUJSBridgeMessage Data, TUJSBridgeResponseCallback ResponseCallback);
	void FlushMessageQueue(const FString& MessageQueueString);
	void InjectJavascriptFile();
	bool IsWebViewJavascriptBridgeURL(const FString& URL);
	bool IsSchemeMatch(const FString& URL);
	bool IsQueueMessageURL(const FString& URL);
	bool IsBridgeLoadedURL(const FString& URL);
	FString WebViewJavascriptCheckCommand();
	FString WebViewJavascriptFetchQueyCommand();

	void EvaluateJavascript(const FString& JavascriptCommand);
	void QueueMessage(const TUJSBridgeMessage& Message);
	void DispatchMessage(const TUJSBridgeMessage& Message);

	void Log(const FString& Action, const TUJSBridgeMessage& Message);

	void HandleEmbeddedCommunication(const FEmbeddedCallParamsHelper& Params);
};
