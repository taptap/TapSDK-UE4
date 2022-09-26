#include "TUType.h"

TSharedPtr<FTUConfig> FTUConfig::Instance = nullptr;

void FTUConfig::Init(const FTUConfig& InitConfig) {
	Instance = MakeShareable(new FTUConfig(InitConfig));
}

TSharedPtr<FTUConfig>& FTUConfig::Get() {
	if (!Instance.IsValid()) {
		Instance = MakeShareable(new FTUConfig);
	}
	return Instance;
}

namespace TUType {
	FString PermissionScope::BasicInfo = "basic_info";
	FString PermissionScope::Profile = "public_profile";
	FString PermissionScope::Friend = "user_friends";
	
}


