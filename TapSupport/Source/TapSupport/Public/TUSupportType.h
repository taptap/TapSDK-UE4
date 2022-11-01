#pragma once
#include "CoreMinimal.h"
#include "TUSupportType.generated.h"

USTRUCT(BlueprintType)
struct FTapSupportConfig {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ServerUrl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ProductID = "-";

};
