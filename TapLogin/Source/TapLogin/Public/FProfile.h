#pragma once

#include "CoreMinimal.h"
#include "FProfile.generated.h"

USTRUCT(BlueprintType)
struct FProfile
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString name;

	UPROPERTY()
	FString avatar;

	UPROPERTY()
	FString openid;

	UPROPERTY()
	FString unionid;

};
