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

FTUConfig::FTUConfig(const FTUConfig& Other)
{
	*this = Other;
	if (Other.BillboardConfig)
	{
		if (BillboardConfig)
		{
			*BillboardConfig = *Other.BillboardConfig;
		}
		else
		{
			BillboardConfig = MakeShared<FBillboardConfig>(*Other.BillboardConfig);
		}
	}
	else
	{
		BillboardConfig.Reset();
	}
}

namespace TUType {
	FString PermissionScope::BasicInfo = "basic_info";
	FString PermissionScope::Profile = "public_profile";
	FString PermissionScope::Friend = "user_friends";
	FString PermissionScope::Email = "email";	
}


