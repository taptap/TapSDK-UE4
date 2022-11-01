#include "TUWebViewJavascriptBridge.h"
#include "TUWebViewJavascriptBridge_JS.h"
#include "TUDebuger.h"
#include "TUJsonHelper.h"

#define kOldProtocolScheme TEXT("wvjbscheme")
#define kNewProtocolScheme TEXT("https")
#define kQueueHasMessage   TEXT("__wvjb_queue_message__")
#define kBridgeLoaded      TEXT("__bridge_loaded__")

TUWebViewJavascriptBridge::TUWebViewJavascriptBridge() {
	StartupMessageQueue = MakeShareable(new TArray<TUJSBridgeMessage>());
	UniqueID = 0;
	EmbeddedCommunicationHandle = FEmbeddedDelegates::GetNativeToEmbeddedParamsDelegateForSubsystem(TEXT("browserProxy")).AddRaw(this, &TUWebViewJavascriptBridge::HandleEmbeddedCommunication);
}

TUWebViewJavascriptBridge::~TUWebViewJavascriptBridge() {
	FEmbeddedDelegates::GetNativeToEmbeddedParamsDelegateForSubsystem(TEXT("browserProxy")).Remove(EmbeddedCommunicationHandle);
}

void TUWebViewJavascriptBridge::RegisterHandler(const FString& HandlerName, TUJSBridgeHandler Handler) {
	MessageHandlers.Add(HandlerName, Handler);
}

void TUWebViewJavascriptBridge::RemoveHandler(const FString& HandlerName) {
	MessageHandlers.Remove(HandlerName);
}

bool TUWebViewJavascriptBridge::IsHanderRegistered(const FString& HandlerName) {
	return MessageHandlers.Find(HandlerName) != nullptr;
}

void TUWebViewJavascriptBridge::CallHandler(const FString& HandlerName, TUJSBridgeMessage Data,
	TUJSBridgeResponseCallback ResponseCallback) {
	SendData(HandlerName, Data, ResponseCallback);
}

void TUWebViewJavascriptBridge::Reset() {
	StartupMessageQueue = MakeShareable(new TArray<TUJSBridgeMessage>());
	ResponseCallbacks.Empty();
	UniqueID = 0;
}

void TUWebViewJavascriptBridge::DisableJavscriptAlertBoxSafetyTimeout() {
	SendData("_disableJavascriptAlertBoxSafetyTimeout", nullptr, nullptr);
}

void TUWebViewJavascriptBridge::EnableLogging() {
}

bool TUWebViewJavascriptBridge::ShouldPerformBridgeAction(const FString& Url) {
	if (IsWebViewJavascriptBridgeURL(Url)) {
		if (IsBridgeLoadedURL(Url)) {
			InjectJavascriptFile();
		} else if(IsQueueMessageURL(Url)) {
			EvaluateJavascript(WebViewJavascriptFetchQueyCommand());
		} else {
			TUDebuger::WarningLog("Unkown Message: "+ Url);
		}
		return true;
	}
	return false;
}

void TUWebViewJavascriptBridge::SendData(const FString& HandlerName, TUJSBridgeMessage Data,
                                         TUJSBridgeResponseCallback ResponseCallback) {
	TUJSBridgeMessage Message = MakeShareable(new FJsonObject);
	if (Data.IsValid()) {
		Message->SetObjectField("data", Data);
	}
	if (ResponseCallback) {
		FString CallBackID = FString::Printf(TEXT("objc_cb_%lld"), ++UniqueID);
		ResponseCallbacks.Add(CallBackID, ResponseCallback);
		Message->SetStringField("callbackId", CallBackID);
	}
	if (!HandlerName.IsEmpty()) {
		Message->SetStringField("handlerName", HandlerName);
	}
	QueueMessage(Message);
}

void TUWebViewJavascriptBridge::FlushMessageQueue(const FString& MessageQueueString) {
	TUDebuger::DisplayLog("FlushMessageQueue: " + MessageQueueString);
	if (MessageQueueString.IsEmpty()) {
		TUDebuger::WarningLog("WebViewJavascriptBridge: WARNING: ObjC got nil while fetching the message queue JSON from webview. This can happen if the WebViewJavascriptBridge JS is not currently present in the webview, e.g if the webview just loaded a new page.");
		return;
	}
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(MessageQueueString);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonArray))
	{
		TUDebuger::WarningLog("WebViewJavascriptBridge: WARNING: MessageQueueString Parse Error.");
		return ;
	}
	for (auto JsonValue : JsonArray) {
		const TSharedPtr<FJsonObject>* JsonObject= nullptr;
		if (!JsonValue->TryGetObject(JsonObject)) {
			TUDebuger::WarningLog("WebViewJavascriptBridge: WARNING: Invalid Json Value.");
			continue;
		}
		TUJSBridgeMessage Message = *JsonObject;
		FString ResponseID;
		if (Message->TryGetStringField("responseId", ResponseID)) {
			// TODO：这里要不要加奔溃判断
			ResponseCallbacks[ResponseID](Message->GetObjectField("responseData"));
			ResponseCallbacks.Remove(ResponseID);
		} else {
			TUJSBridgeResponseCallback ResponseCallback = nullptr;
			FString CallbackID;
			if (Message->TryGetStringField("callbackId", CallbackID)) {
				ResponseCallback = [=](TUJSBridgeMessage ResponseData) {
					TUJSBridgeMessage MSG;
					MSG->SetStringField("responseId", CallbackID);
					MSG->SetObjectField("responseData", ResponseData);
					QueueMessage(MSG);
				};
			} else {
				ResponseCallback = [](TUJSBridgeMessage IgnoreResponseData) {
					// Do nothing
				};
			}
			auto HandlerPtr = MessageHandlers.Find(Message->GetStringField("handlerName"));
			// auto Handler = MessageHandlers[Message->GetStringField("handlerName")];
			if (!HandlerPtr) {
				TUDebuger::WarningLog("WVJBNoHandlerException, No handler for message");
				continue;
			}
			if (auto Handler = *HandlerPtr) {
				Handler(Message->GetObjectField("data"), ResponseCallback);
			}
		}
	}
}

void TUWebViewJavascriptBridge::InjectJavascriptFile() {
	FString JS = TUWebViewJavascriptBridge_js();
	EvaluateJavascript(JS);
	if (StartupMessageQueue.IsValid()) {
		auto Queue = StartupMessageQueue;
		StartupMessageQueue = nullptr;
		for (auto JsonObject : *Queue.Get()) {
			DispatchMessage(JsonObject);
		}
	} 
}

bool TUWebViewJavascriptBridge::IsWebViewJavascriptBridgeURL(const FString& URL) {
	return IsBridgeLoadedURL(URL) || IsQueueMessageURL(URL);
}

bool TUWebViewJavascriptBridge::IsSchemeMatch(const FString& URL) {
	return URL.StartsWith(kNewProtocolScheme) || URL.StartsWith(kOldProtocolScheme);
}

bool TUWebViewJavascriptBridge::IsQueueMessageURL(const FString& URL) {
	FString Host = FString("://") + kQueueHasMessage;
	return IsSchemeMatch(URL) && URL.Contains(Host);
}

bool TUWebViewJavascriptBridge::IsBridgeLoadedURL(const FString& URL) {
	FString Host = FString("://") + kBridgeLoaded;
	return IsSchemeMatch(URL) && URL.Contains(Host);
}

FString TUWebViewJavascriptBridge::WebViewJavascriptCheckCommand() {
	return TEXT("typeof WebViewJavascriptBridge == \'object\';");
}

FString TUWebViewJavascriptBridge::WebViewJavascriptFetchQueyCommand() {
	return TEXT("WebViewJavascriptBridge._fetchQueue();");
}

void TUWebViewJavascriptBridge::EvaluateJavascript(const FString& JavascriptCommand) {
	OnEvaluateJavascript.ExecuteIfBound(JavascriptCommand, [=](const FString& Result) {
		if (JavascriptCommand == WebViewJavascriptFetchQueyCommand()) {
			FlushMessageQueue(Result);
		}
	});
}

void TUWebViewJavascriptBridge::QueueMessage(const TUJSBridgeMessage& Message) {
	if (StartupMessageQueue.IsValid()) {
		StartupMessageQueue->Add(Message);
	} else {
		DispatchMessage(Message);
	}
}

void TUWebViewJavascriptBridge::DispatchMessage(const TUJSBridgeMessage& Message) {
	FString MessageJson = TUJsonHelper::GetJsonString(Message);
	Log("SEND", Message);
	FString JavascriptCommand = FString::Printf(TEXT("WebViewJavascriptBridge._handleMessageFromObjC('%s');"), *MessageJson);
	EvaluateJavascript(JavascriptCommand);
}

void TUWebViewJavascriptBridge::Log(const FString& Action, const TUJSBridgeMessage& Message) {
	FString DebugInfo = "WVJB " + Action + ": " + TUJsonHelper::GetJsonString(Message);
	TUDebuger::DisplayLog(DebugInfo);
}

void TUWebViewJavascriptBridge::HandleEmbeddedCommunication(const FEmbeddedCallParamsHelper& Params) {
	FString Error;
	if (Params.Command == "handlejs")
	{
		FString Message = Params.Parameters.FindRef(TEXT("script"));
		if (!Message.IsEmpty())
		{
			//OnJsMessageReceived(Message)
			TUDebuger::DisplayLog("HandleEmbeddedCommunication: " + Message);
			FlushMessageQueue(Message);
		}
	}
	// else if (Params.Command == "pageload")
	// {
	// 	Scripting->PageLoaded();
	// }

	Params.OnCompleteDelegate(FEmbeddedCommunicationMap(), Error);
}
