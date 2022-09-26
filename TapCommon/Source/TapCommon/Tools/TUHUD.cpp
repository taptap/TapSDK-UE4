#include "TUHUD.h"

#include "TUSettings.h"
// WidgetBlueprint'/TapCommon/BPTDUHUD.BPTDUHUD'

UTUHUD* UTUHUD::HUD = nullptr;

UTUHUD*& UTUHUD::GetCurrentHUD()
{
	if (HUD)
	{
		// HUD->GetWorld()->GetTimerManager().
		HUD->GetWorld()->GetTimerManager().ClearTimer(HUD->DismissTimerHandle);
		if (!HUD->IsInViewport())
		{
			HUD->AddToViewport(MAX_int16);
		}
		return HUD;
	} else
	{
		if (UClass* MyWidgetClass = LoadClass<UTUHUD>(nullptr, TEXT("WidgetBlueprint'/TapCommon/BPTDUHUD.BPTDUHUD_C'")))
		{
			if (TUSettings::GetGameInstance().IsValid()) {
				HUD = CreateWidget<UTUHUD>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
				HUD->AddToViewport(MAX_int16);
			}
        }
		return HUD;
	}
}

UTUHUD::UTUHUD(const FObjectInitializer& ObjectInitializer)  : Super(ObjectInitializer)
{
}

void UTUHUD::ShowWait()
{
	UTUHUD* Current = GetCurrentHUD();
	Current->TitleLabel->SetVisibility(ESlateVisibility::Collapsed);
	Current->ImageBox->SetVisibility(ESlateVisibility::Visible);
	Current->BackView->SetVisibility(ESlateVisibility::Visible);
}

void UTUHUD::ShowWaitAndToast(const FString& Toast)
{
	UTUHUD* Current = GetCurrentHUD();
	Current->TitleLabel->SetVisibility(ESlateVisibility::Visible);
	Current->ImageBox->SetVisibility(ESlateVisibility::Visible);
	Current->TitleLabel->SetText(FText::FromString(Toast));
	Current->BackView->SetVisibility(ESlateVisibility::Visible);
}

void UTUHUD::Dismiss()
{
	if (!HUD)
	{
		return;
	}
	HUD->GetWorld()->GetTimerManager().ClearTimer(HUD->DismissTimerHandle);
	if (HUD->IsInViewport())
	{
		HUD->RemoveFromParent();
	}
	HUD = nullptr;
}

void UTUHUD::ShowToast(const FString& Toast, float TimeInterval)
{
	UTUHUD* Current = GetCurrentHUD();
	Current->TitleLabel->SetVisibility(ESlateVisibility::Visible);
	Current->ImageBox->SetVisibility(ESlateVisibility::Collapsed);
	Current->BackView->SetVisibility(ESlateVisibility::HitTestInvisible);
	Current->TitleLabel->SetText(FText::FromString(Toast));
	Current->GetWorld()->GetTimerManager().SetTimer(Current->DismissTimerHandle,
	[=]()
	{
		HUD->RemoveFromParent();
		HUD = nullptr;
	}, TimeInterval, false);
}

void UTUHUD::NativeConstruct()
{
	Super::NativeConstruct();
	DismissTimerHandle.Invalidate();
}


