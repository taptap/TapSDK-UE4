#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TapDBSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig)
class TAPDB_API UTapDBSettings : public UObject
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, config, Category = "TapDB")
    bool IsCN;

    UPROPERTY(EditAnywhere, config, Category = "TapDB")
    FString GameVersion;

    UPROPERTY(EditAnywhere, config, Category = "TapDB")
    FString GameChannel;
    
};
