#include "TULoginRegionConfig.h"
#include "TULoginImpl.h"

class TULoginRegionConfigCN: public TULoginRegionConfig
{
public:
	virtual FString WebHost() {
		return "https://www.taptap.com";
	}
	
	virtual FString ApiHost() {
		return "https://openapi.taptap.com";
	}
	
	virtual FString AccountHost() {
		return "https://accounts.taptap.com";
	}
	
};

class TULoginRegionConfigIO: public TULoginRegionConfig
{
public:
	virtual FString WebHost() {
		return "https://www.taptap.io";
	}
	
	virtual FString ApiHost() {
		return "https://openapi.tap.io";
	}
	
	virtual FString AccountHost() {
		return "https://accounts.taptap.io";
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
	return AccountHost() + "/authorize";
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