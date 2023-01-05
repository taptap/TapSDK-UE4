#include "LeanCloud.h"
#include "LeanCloudImpl.h"

void FLeanCloud::Init(const FLCConfig& Config) {
	FLeanCloudImpl::Get()->Init(Config);
}
