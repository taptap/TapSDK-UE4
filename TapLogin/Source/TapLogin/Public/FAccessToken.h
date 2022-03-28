#pragma once

#include "CoreMinimal.h"
#include "FAccessToken.generated.h"

USTRUCT(BlueprintType)
struct FAccessToken
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString kid;

	UPROPERTY()
	FString access_token;

	UPROPERTY()
	FString mac_algorithm;

	UPROPERTY()
	FString token_type;

	UPROPERTY()
	FString mac_key;

	UPROPERTY()
	uint64 expireIn;

};