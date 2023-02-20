#pragma once
#include "LCError.h"
#include "LeanCloudType.generated.h"


USTRUCT(BlueprintType)
struct LEANCLOUD_API FLCConfig {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClientToken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ServerURL;

	static FLCConfig& Get();

	TSharedPtr<FLCError> CheckConfig() const;
protected:
	static FLCConfig Instance;
};

DECLARE_DELEGATE_OneParam(FStringSignature, const FString&);
