#pragma once

class TULoginRegionConfig
{
public:
	virtual ~TULoginRegionConfig() = default;
	virtual FString WebHost() = 0;
	virtual FString ApiHost() = 0;

	FString CodeUrl();
	FString TokenUrl();
	FString ProfileUrl();
	FString AccountUrl();
	FString TestQualificationUrl();

	static FString TapTapSDKVersion;

	static TSharedPtr<TULoginRegionConfig>& Get();

private:
	static TSharedPtr<TULoginRegionConfig> Instance;
	
};

