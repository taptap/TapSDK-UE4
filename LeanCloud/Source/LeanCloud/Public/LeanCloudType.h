#pragma once
#include "LeanCloudType.generated.h"


USTRUCT(BlueprintType)
struct LEANCLOUD_API FLCConfig {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientToken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ServerURL;

};
