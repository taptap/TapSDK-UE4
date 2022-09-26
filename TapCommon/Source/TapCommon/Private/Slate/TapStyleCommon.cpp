// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/TapStyleCommon.h"

#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyle.h"

TSharedPtr<FSlateStyleSet > FTapStyleCommon::TapStyleInstance;

void FTapStyleCommon::Initialize()
{
	if ( !TapStyleInstance.IsValid() )
	{
		TapStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle( *TapStyleInstance );
	}
}

void FTapStyleCommon::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle( *TapStyleInstance );
	ensure( TapStyleInstance.IsUnique() );
	TapStyleInstance.Reset();
}

const ISlateStyle& FTapStyleCommon::Get()
{
	return *TapStyleInstance;
}

FName FTapStyleCommon::GetStyleSetName()
{
	static FName TapStyleSetName = TEXT("TapStyleCommon");
	return TapStyleSetName;
}

TSharedRef<FSlateStyleSet> FTapStyleCommon::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(GetStyleSetName(), "/TapCommon/Styles", "/TapCommon/Styles");
	return StyleRef;
}
	
