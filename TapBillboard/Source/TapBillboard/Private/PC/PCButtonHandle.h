// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "UI/TapPlatformButton.h"

/**
 * 
 */
class TAPBILLBOARD_API FPCButtonHandle : public IPlatformButtonHandle, public TSharedFromThis<FPCButtonHandle>
{
public:
	FPCButtonHandle(UTapPlatformButton* Button);
	
	virtual void UpdateTexture(const FString IconUrl) override;

	void HandleImageRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	TWeakObjectPtr<UTapPlatformButton> SavedButton;
};
