#include "TULoginRegionConfig.h"

#include "TUDebuger.h"
#include "TULoginImpl.h"

class TULoginRegionConfigCN: public TULoginRegionConfig
{
public:
	virtual FString WebHost() {
		return TUDebuger::GetReplacedHost("https://accounts.tapapis.cn");
	}
	
	virtual FString ApiHost() {
		return TUDebuger::GetReplacedHost("https://open.tapapis.cn");
	}
	
};

class TULoginRegionConfigIO: public TULoginRegionConfig
{
public:
	virtual FString WebHost() {
		return TUDebuger::GetReplacedHost("https://accounts.tapapis.com");
	}
	
	virtual FString ApiHost() {
		return TUDebuger::GetReplacedHost("https://open.tapapis.com");
	}
	
};

FString TULoginRegionConfig::TapTapSDKVersion = "1.2.0";
TSharedPtr<TULoginRegionConfig> TULoginRegionConfig::Instance = nullptr;

FString TULoginRegionConfig::CodeUrl()
{
	return WebHost() + "/oauth2/v1/device/code";
}

FString TULoginRegionConfig::TokenUrl()
{
	return WebHost() + "/oauth2/v1/token";
}

FString TULoginRegionConfig::ProfileUrl()
{
	return ApiHost() + "/account/profile/v1";
}

FString TULoginRegionConfig::AccountUrl()
{
	return WebHost() + "/oauth2/v1/authorize";
}

FString TULoginRegionConfig::TestQualificationUrl()
{
	return ApiHost() + "/test/user-status/v1";
}

TSharedPtr<TULoginRegionConfig>& TULoginRegionConfig::Get() {
	if (!Instance.IsValid()) {
		if (TULoginImpl::Get()->Config.RegionType == ERegionType::CN) {
			Instance = MakeShareable(new TULoginRegionConfigCN);
		} else {
			Instance = MakeShareable(new TULoginRegionConfigIO);
		}
	}
	return Instance;
}