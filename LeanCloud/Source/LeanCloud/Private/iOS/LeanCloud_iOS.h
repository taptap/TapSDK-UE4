#pragma once
#include "LeanCloudImpl.h"

class FLeanCloud_iOS: public FLeanCloudImpl {
public:
	virtual ~FLeanCloud_iOS() override;
	virtual void Init(const FLCConfig& InConfig) override;
};
