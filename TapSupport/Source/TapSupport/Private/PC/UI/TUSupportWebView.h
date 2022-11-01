#pragma once

#include "TapWebBrowser.h"
#include "PC/TUWebViewJavascriptBridge/TUWebViewJavascriptBridge.h"
// #include "Blueprint/UserWidget.h"
#include "TUSupportWebView.generated.h"

UCLASS()
class UTUSupportWebView : public UTapWebBrowser
{
	GENERATED_BODY()

public:

	static void Show(const FString& Url);

protected:

	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual bool OnBeforeNavigation(const FString& URL, const FWebNavigationRequest& Request) override;

private:
	TSharedPtr<TUWebViewJavascriptBridge> Bridge;

	FString OriginUrl;

	void InitJSBridge();

	void OnEvaluateJavascript(const FString& JSCommand, TFunction<void(const FString&)> Ignore);
	
};
