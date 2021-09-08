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
    UTapBootstrapSettings(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer), clientID(""), clientToken(""), serverUrl(""), bIsCN(true), bTapDBEnable(false), gameVersion(""), gameChannel("")
    {
    }

    UPROPERTY(EditAnywhere, config, meta = (DisplayName = "TapTap clientID"), Category = "Bootstrap Config")
    FString clientID;

    UPROPERTY(EditAnywhere, config, meta = (DisplayName = "TapTap clientToken"), Category = "Bootstrap Config")
    FString clientToken;

    UPROPERTY(EditAnywhere, config, meta = (DisplayName = "TapTap serverUrl"), Category = "Bootstrap Config")
    FString serverUrl;

    UPROPERTY(EditAnywhere, config, meta = (DisplayName = "China Mainland"), Category = "Bootstrap Config")
    bool bIsCN;

    UPROPERTY(EditAnywhere, config, meta = (DisplayName = "Open TapDB"), Category = "TapDB Config")
    bool bTapDBEnable;

    UPROPERTY(EditAnywhere, config, meta = (DisplayName = "iOS 14 Open IDFA",EditCondition = "bTapDBEnable"), Category = "TapDB Config")
    bool bAdvertiserIDCollectionEnabled;

    UPROPERTY(EditAnywhere, config, meta = (DisplayName = "Game Version",EditCondition = "bTapDBEnable"), Category = "TapDB Config" )
    FString gameVersion;

    UPROPERTY(EditAnywhere, config, meta = (DisplayName = "Game Channel",EditCondition = "bTapDBEnable"), Category = "TapDB Config")
    FString gameChannel;
};
