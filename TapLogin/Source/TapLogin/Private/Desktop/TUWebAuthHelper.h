#pragma once
#include "IHttpRouter.h"

class TUWebAuthHelper: public TSharedFromThis<TUWebAuthHelper> {
public:
	TUWebAuthHelper();
	~TUWebAuthHelper();

	bool ProcessWebAuth(const TArray<FString>& NeedPermissions, TFunction<void(FString WebCode)> CallBack);

	void StopProcess();

private:

	int32 CurrentPort = 0;

	FString State;

	TArray<FString> Permissions;

	FString CodeVerifier;

public:
	FString GetCodeVerifier() const;
	FString RedirectUri;
private:

	TFunction<void(FString WebCode)> CallBackBlock;
	
	TSharedPtr<IHttpRouter> HttpRouter = nullptr;

	FHttpRouteHandle AuthHandle = nullptr;

	void ProcessWebAuthRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);

	FString GenerateWebAuthUrl();

};
