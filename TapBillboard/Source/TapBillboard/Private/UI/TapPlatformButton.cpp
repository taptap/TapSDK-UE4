// Fill out your copyright notice in the Description page of Project Settings.


#include "TapPlatformButton.h"


#include "Components/Button.h"
#if PLATFORM_IOS || PLATFORM_ANDROID
#include "ButtonHandle.h"
#else
#include "PCButtonHandle.h"
typedef FPCButtonHandle FButtonHandle;
#endif

void UTapPlatformButton::UpdateButtonTexture(const FString& Url)
{
	if (PlatformButtonHandle)
	{
		PlatformButtonHandle->UpdateTexture(Url);
	}
}

void UTapPlatformButton::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	
	const bool bNewVisible = (InVisibility == ESlateVisibility::Visible || InVisibility == ESlateVisibility::HitTestInvisible || InVisibility == ESlateVisibility::SelfHitTestInvisible);

	if (PlatformButtonHandle)
	{
		PlatformButtonHandle->SetVisible(bNewVisible);
	}
}

void UTapPlatformButton::ShowPlatformButtonNextFrame()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick(this, &UTapPlatformButton::ShowPlatformButtonImpl);
	}
}

void UTapPlatformButton::ClosePlatformButton()
{
	if (PlatformButtonHandle)
	{
		PlatformButtonHandle->Close();
	}
}

void UTapPlatformButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (PlatformButtonHandle)
	{
		const FGeometry& Geometry = Button->GetCachedGeometry();
		PlatformButtonHandle->UpdateLayout(Geometry);
	}
}

void UTapPlatformButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
#if PLATFORM_WINDOWS || PLATFORM_MAC
	Button->OnClicked.AddDynamic(this, &UTapPlatformButton::OnPCButtonClicked);
#else
	Button->SetColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 0.f));
#endif
	PlatformButtonHandle = MakeShared<FButtonHandle>(this);
}

void UTapPlatformButton::ShowPlatformButtonImpl()
{
	if (PlatformButtonHandle)
	{
		PlatformButtonHandle->Show();
	}
}


void UTapPlatformButton::OnPCButtonClicked()
{
	OnClicked.ExecuteIfBound();
}

