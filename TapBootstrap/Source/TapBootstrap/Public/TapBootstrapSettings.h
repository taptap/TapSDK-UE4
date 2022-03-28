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
	UTapBootstrapSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), bIsAutoInit(false),
	                                                                     clientID(""), clientToken(""), serverUrl(""),
	                                                                     bIsCN(true), bTapDBEnable(false),
	                                                                     gameVersion(""), gameChannel("")
	{
	}

	UPROPERTY(EditAnywhere, Config, meta = (DisplayName = "TapTap Auto Init"), Category = "Bootstrap Config")
	bool bIsAutoInit;

	UPROPERTY(EditAnywhere, config, meta = (DisplayName = "TapTap clientID", EditCondition = "bIsAutoInit"),
		Category = "Bootstrap Config")
	FString clientID;

	UPROPERTY(EditAnywhere, config, meta = (DisplayName = "TapTap clientToken", EditCondition = "bIsAutoInit"),
		Category = "Bootstrap Config")
	FString clientToken;

	UPROPERTY(EditAnywhere, config, meta = (DisplayName = "TapTap serverUrl", EditCondition = "bIsAutoInit"),
		Category = "Bootstrap Config")
	FString serverUrl;

	UPROPERTY(EditAnywhere, config, meta = (DisplayName = "China Mainland", EditCondition = "bIsAutoInit"),
		Category = "Bootstrap Config")
	bool bIsCN;

	UPROPERTY(EditAnywhere, config, meta = (DisplayName = "Open TapDB", EditCondition = "bIsAutoInit"),
		Category = "TapDB Config")
	bool bTapDBEnable;

	UPROPERTY(EditAnywhere, config, meta = (DisplayName = "iOS 14 Open IDFA", EditCondition = "bTapDBEnable"),
		Category = "TapDB Config")
	bool bAdvertiserIDCollectionEnabled;

	UPROPERTY(EditAnywhere, config, meta = (DisplayName = "Game Version", EditCondition = "bTapDBEnable"),
		Category = "TapDB Config")
	FString gameVersion;

	UPROPERTY(EditAnywhere, config, meta = (DisplayName = "Game Channel", EditCondition = "bTapDBEnable"),
		Category = "TapDB Config")
	FString gameChannel;
};
