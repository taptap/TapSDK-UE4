#pragma once

#include "CoreMinimal.h"
#include "TapError.generated.h"

USTRUCT()
struct FTapError
{
    GENERATED_BODY()
	
	UPROPERTY()
	int code;

	UPROPERTY()
	FString error_description;

};