#pragma once
#include "TUHttpRequest.h"

class TAPCOMMON_API TUHttpManager
{
public:
	static TUHttpManager& Get();

	void request(TSharedPtr<TUHttpRequest> tdsReq);

private:
	static TUHttpManager* Singleton;
};
