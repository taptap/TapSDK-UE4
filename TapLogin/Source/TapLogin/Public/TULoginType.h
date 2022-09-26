#pragma once
#include "TUType.h"
#include "TULoginType.generated.h"

USTRUCT(BlueprintType)
struct FTULoginConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,	BlueprintReadWrite)
	FString ClientID;

	UPROPERTY(EditAnywhere,	BlueprintReadWrite)
	ERegionType RegionType = ERegionType::CN;

	UPROPERTY(EditAnywhere,	BlueprintReadWrite)
	bool bRoundCorner = true;

};
