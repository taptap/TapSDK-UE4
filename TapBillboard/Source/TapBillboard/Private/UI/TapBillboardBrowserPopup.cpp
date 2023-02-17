// Fill out your copyright notice in the Description page of Project Settings.


#include "TapBillboardBrowserPopup.h"
#include "SWebBrowserView.h"
#include "TapBillboardCommon.h"
#include "TapBillboardModule.h"
#include "TUSettings.h"
#include "Components/Button.h"
#include "Components/NativeWidgetHost.h"

UTapBillboardBrowserPopup* UTapBillboardBrowserPopup::OpenUrl(const FString& Url)
{
	if (FTapBillboardPtr Billboard = FTapBillboardModule::GetTapBillboardInterface())
	{
		TSubclassOf<UTapBillboardBrowserPopup> Class = Billboard->GetPopupBrowserClass();
		check(Class);

		if (UTapBillboardBrowserPopup* UI = CreateWidget<UTapBillboardBrowserPopup>(TUSettings::GetGameInstance().Get(), Class))
		{
			UI->SetVisibility(ESlateVisibility::Hidden);
			UI->BrowserView->LoadURL(Url);
			UI->AddToViewport(TUSettings::GetUILevel());
			return UI;
		}
	}
	return nullptr;
}

void UTapBillboardBrowserPopup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SAssignNew(BrowserView, SWebBrowserView)
			.InitialURL(TEXT(""))
			.OnLoadCompleted_UObject(this, &UTapBillboardBrowserPopup::OnLoadCompleted)
			.OnLoadError_UObject(this, &UTapBillboardBrowserPopup::RemoveFromParent)
			.BackgroundColor(FColor(0,0,0,0))
			.SupportsTransparency(true)
			.ShowErrorMessage(false)
			.BrowserFrameRate(60.f);
	
	NativeWidget->SetContent(BrowserView.ToSharedRef());

	if (ITextInputMethodSystem* InputSys = FSlateApplication::Get().GetTextInputMethodSystem())
	{
		BrowserView->BindInputMethodSystem(InputSys);
	}
	
	CloseButton->OnClicked.AddDynamic(this, &UTapBillboardBrowserPopup::RemoveFromParent);
}

void UTapBillboardBrowserPopup::OnLoadCompleted()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}


