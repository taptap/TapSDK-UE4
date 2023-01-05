#pragma once
#include "CoreMinimal.h"
#include "TUQrCodeModel.generated.h"

USTRUCT()
struct FTUQrCodeModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString qrcode_url;

	UPROPERTY()
	FString verification_url;

	UPROPERTY()
	FString device_code;

	UPROPERTY()
	FString user_code;

	UPROPERTY()
	int64 expires_in = 0;

	UPROPERTY()
	int64 interval = 0;
	
};
