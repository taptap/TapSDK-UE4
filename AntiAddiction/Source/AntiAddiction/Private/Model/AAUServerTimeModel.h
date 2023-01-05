#pragma once
#include "CoreMinimal.h"
#include "AAUServerTimeModel.generated.h"


USTRUCT()
struct FAAUServerTimeModel
{
	GENERATED_BODY()
	
	UPROPERTY()
	int64 timestamp = 0;

};

