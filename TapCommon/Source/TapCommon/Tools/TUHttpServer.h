#pragma once
#include "IHttpRouter.h"


class TAPCOMMON_API TUHttpServer {
public:

	
	static FString RegisterNewRoute(const FString& RoutePath, TFunction<bool(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)> CallBack);

private:

	static const TSharedPtr<TUHttpServer> Get();

	static TSharedPtr<TUHttpServer> InstancePtr;

	int32 CurrentPort = 0;

	TMap<FString, FHttpRouteHandle> RegisteredRoutes;

	TSharedPtr<IHttpRouter> HttpRouter = nullptr;
	// FHttpRouteHandle AuthHandle = nullptr;
	bool StartHttpServer();
	void StopHttpServer();
	
};

