#pragma once

class TUDBRegionConfig
{
public:
	virtual ~TUDBRegionConfig() = default;
	virtual FString GetHost() = 0;

	FString GetEventUrl();
	
	static TSharedPtr<TUDBRegionConfig>& Get();

private:
	static TSharedPtr<TUDBRegionConfig> Instance;
	
};
