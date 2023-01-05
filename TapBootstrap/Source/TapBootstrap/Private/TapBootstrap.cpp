#include "TapBootstrap.h"
#include "TapBootstrapImpl.h"

void FTapBootstrap::Init(const FTUConfig& InConfig) {
	FTapBootstrapImpl::Get()->Init(InConfig);
}
