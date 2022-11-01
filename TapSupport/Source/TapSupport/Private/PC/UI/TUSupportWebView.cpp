#include "TUSupportWebView.h"

#include "TUDebuger.h"
#include "TUSettings.h"

void UTUSupportWebView::Show(const FString& Url) {
	if (UClass* MyWidgetClass = LoadClass<UTUSupportWebView>(
		nullptr, TEXT("WidgetBlueprint'/TapSupport/TapSupportWebViewBP.TapSupportWebViewBP_C'"))) {
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UTUSupportWebView>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->OriginUrl = Url;
			widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UTUSupportWebView::NativeOnInitialized() {
	Super::NativeOnInitialized();
	InitJSBridge();
}

void UTUSupportWebView::NativeConstruct() {
	Super::NativeConstruct();
	LoadURL(OriginUrl);
}

bool UTUSupportWebView::OnBeforeNavigation(const FString& URL, const FWebNavigationRequest& Request) {
	TUDebuger::DisplayShow("MY: " + URL);
	
	//if (URL == "https://www.taobao.com/") {
		//return false;
	//} else {
	//	return true;
	//}
	if (Bridge->ShouldPerformBridgeAction(URL)) {
		return true;
	}
	return false;
}

void UTUSupportWebView::InitJSBridge() {
	Bridge = MakeShareable(new TUWebViewJavascriptBridge);
	Bridge->OnEvaluateJavascript.BindUObject(this, &UTUSupportWebView::OnEvaluateJavascript);
	Bridge->RegisterHandler("_closePage", [](TUJSBridgeMessage Data, TUJSBridgeResponseCallback ResponseCallback) {
		TUDebuger::DisplayLog("RegisterHandler：_closePage");
	});
	Bridge->RegisterHandler("loadComplete", [](TUJSBridgeMessage Data, TUJSBridgeResponseCallback ResponseCallback) {
		TUDebuger::DisplayLog("RegisterHandler：loadComplete");
	});
	Bridge->RegisterHandler("showCloseButton", [](TUJSBridgeMessage Data, TUJSBridgeResponseCallback ResponseCallback) {
		TUDebuger::DisplayLog("RegisterHandler：showCloseButton");
	});
	Bridge->RegisterHandler("hideCloseButton", [](TUJSBridgeMessage Data, TUJSBridgeResponseCallback ResponseCallback) {
		TUDebuger::DisplayLog("RegisterHandler：hideCloseButton");
	});
	Bridge->RegisterHandler("openBrowser", [](TUJSBridgeMessage Data, TUJSBridgeResponseCallback ResponseCallback) {
		TUDebuger::DisplayLog("RegisterHandler：openBrowser");
	});
	Bridge->RegisterHandler("getDeviceInfo", [](TUJSBridgeMessage Data, TUJSBridgeResponseCallback ResponseCallback) {
		TUDebuger::DisplayLog("RegisterHandler：getDeviceInfo");
	});
	Bridge->RegisterHandler("_hasNativeMethod", [](TUJSBridgeMessage Data, TUJSBridgeResponseCallback ResponseCallback) {
		TUDebuger::DisplayLog("RegisterHandler：_hasNativeMethod");
	});
}

void UTUSupportWebView::OnEvaluateJavascript(const FString& JSCommand, TFunction<void(const FString&)> Ignore) {
	TUDebuger::DisplayLog("ExecuteJavascript: " + JSCommand);
	ExecuteJavascript(JSCommand);
}
