#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TapBootstrapSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig)
class TAPBOOTSTRAP_API UTapBootstrapSettings : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, config, Category = "Bootstrap")
    FString ClientId;

    UPROPERTY(EditAnywhere, config, Category = "Bootstrap")
    bool IsCN;

};
