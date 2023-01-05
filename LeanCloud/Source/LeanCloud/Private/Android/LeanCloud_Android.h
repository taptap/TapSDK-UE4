#pragma once
#include "LeanCloudImpl.h"

class FLeanCloud_Android: public FLeanCloudImpl {
public:
	virtual ~FLeanCloud_Android() override;
	virtual void Init(const FLCConfig& InConfig) override;
};
