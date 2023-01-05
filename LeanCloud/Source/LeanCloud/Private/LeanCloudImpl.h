#pragma once
#include "LeanCloudType.h"

class FLeanCloudImpl {
public:
	static TSharedPtr<FLeanCloudImpl> Get();
	virtual ~FLeanCloudImpl() = default;

	virtual void Init(const FLCConfig& InConfig);
private:
	static TSharedPtr<FLeanCloudImpl> Instance;
};
