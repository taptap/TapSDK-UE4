#pragma once
#include "TUType.h"
#include "TUDBType.generated.h"

USTRUCT(BlueprintType)
struct TAPDB_API FTUDBConfig
{
	GENERATED_BODY()

	// 注册游戏时获得的APP ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientId;

	// 分包渠道名称，可为空
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Channel;

	// 游戏版本
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GameVersion;

	// 区域
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERegionType RegionType = ERegionType::CN;

};


namespace TUDBType {

	class LoginType {
	public:
		static FString TapTap;
		static FString WeiXin;
		static FString QQ;
		static FString Tourist;
		static FString Apple;
		static FString ZhiFuBao;
		static FString Facebook;
		static FString Google;
		static FString Twitter;
		static FString PhoneNumber;
	};
	
}
