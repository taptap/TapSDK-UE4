#pragma once
#include "CoreMinimal.h"
#include "TULoginProfileModel.generated.h"

USTRUCT()
struct FTULoginProfileModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString name;
	
	UPROPERTY()
	FString avatar;
	
	UPROPERTY()
	FString openid;
	
	UPROPERTY()
	FString unionid;

#if PLATFORM_MAC || PLATFORM_WINDOWS

	void SaveToLocal();
	
	static TSharedPtr<FTULoginProfileModel> GetLocalModel();

	static void ClearLocalModel();

#endif

	
};
