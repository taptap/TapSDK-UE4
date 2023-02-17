// Fill out your copyright notice in the Description page of Project Settings.


#include "TapStyleAntiAddiction.h"
#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyle.h"


TSharedPtr<FSlateStyleSet > FTapStyleAntiAddiction::TapStyleInstance;

void FTapStyleAntiAddiction::Initialize()
{
	if ( !TapStyleInstance.IsValid() )
	{
		TapStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle( *TapStyleInstance );
	}
}

void FTapStyleAntiAddiction::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle( *TapStyleInstance );
	ensure( TapStyleInstance.IsUnique() );
	TapStyleInstance.Reset();
}

const ISlateStyle& FTapStyleAntiAddiction::Get()
{
	return *TapStyleInstance;
}

FName FTapStyleAntiAddiction::GetStyleSetName()
{
	static FName TapStyleSetName = TEXT("TapStyleAntiAddiction");
	return TapStyleSetName;
}

TSharedRef<FSlateStyleSet> FTapStyleAntiAddiction::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(GetStyleSetName(), "/AntiAddiction/Styles", "/AntiAddiction/Styles");
	return StyleRef;
}
	
