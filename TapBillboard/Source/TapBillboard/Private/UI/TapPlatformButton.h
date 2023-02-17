// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TapPlatformButton.generated.h"

class UButton;

class IPlatformButtonHandle
{
public:
	virtual ~IPlatformButtonHandle() {}
	virtual void Show() {}
	virtual void Close() {}
	virtual void UpdateTexture(const FString IconUrl) {};
	virtual void UpdateLayout(const FGeometry& PlaceHolderGeometry) {}
	virtual void SetVisible(bool bNewVisible) {}
};

UCLASS(Abstract)
class TAPBILLBOARD_API UTapPlatformButton : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateButtonTexture(const FString& Url);

	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	void ShowPlatformButtonNextFrame();//WebBrowser 移动端的显示是在Tick事件里面调用的，Button要放在上面，暂时先晚一点Show

	void ClosePlatformButton();
	
	FSimpleDelegate OnClicked;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeOnInitialized() override;

	void ShowPlatformButtonImpl();
	
	UFUNCTION()
	void OnPCButtonClicked();

	TSharedPtr<IPlatformButtonHandle> PlatformButtonHandle;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* Button;
	friend class FPCButtonHandle;
};

