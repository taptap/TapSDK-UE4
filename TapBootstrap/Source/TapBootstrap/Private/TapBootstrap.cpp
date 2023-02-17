#include "TapBootstrap.h"
#include "TapBootstrapImpl.h"

FTapBootstrapInit FTapBootstrap::OnBootstrapInit;

void FTapBootstrap::Init(const FTUConfig& InConfig) {
	FTapBootstrapImpl::Get()->Init(InConfig);
	
	OnBootstrapInit.Broadcast(*FTUConfig::Get());
}

