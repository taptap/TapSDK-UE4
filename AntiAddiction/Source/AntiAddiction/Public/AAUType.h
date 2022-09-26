#pragma once
#include "CoreMinimal.h"
#include "AAUType.generated.h"

UENUM()
enum class EAAURegion : uint8 {
	China,			// 国内            
	Vietnam,		// 越南  
};


UENUM()
enum class EAAUAgeLimit : int8 {
	Unknown = -1,
	Child = 0,                   
	Teen = 8,                   
	Young = 16,                        
	Adult = 18
};

USTRUCT(BlueprintType)
struct FAAUConfig {
	GENERATED_BODY()
	
	// 注册获得的Client ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientID;

	// 是否使用TapTap的快速实名认证(暂不支持)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseTapLogin = false;

	// 是否支持切换账号
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShowSwitchAccount = false;

	// 国家区域，默认中国
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAAURegion Region = EAAURegion::China;
};

