#pragma once
#include "PC/TUSupportPCImpl.h"

class TUSupportMobileImpl: public TUSupportPCImpl {
public:
	TUSupportMobileImpl();
	virtual void OpenFullUrl(const FString& Path) override;
	virtual void CloseSupportView() override;

};
