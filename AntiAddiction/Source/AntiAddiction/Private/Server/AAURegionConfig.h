#pragma once

class AAURegionConfig {
public:
	virtual ~AAURegionConfig() = default;
	virtual FString BaseHost() = 0;
	virtual FString GetRSAPublicKey();

	FString RealNameUrl();
	FString AntiAddictionUrl();
	FString TDSPublicConfigUrl();
	
	
	static TSharedPtr<AAURegionConfig>& Get();

private:
	static TSharedPtr<AAURegionConfig> Instance;
};
