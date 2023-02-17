// Fill out your copyright notice in the Description page of Project Settings.


#include "SAAUTimeItem.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAAUTimeItem::Construct(const FArguments& InArgs)
{
	FNumberFormattingOptions Options = FNumberFormattingOptions::DefaultNoGrouping();
	Options.SetMinimumIntegralDigits(InArgs._MinimumIntegralDigits);
	SavedMinimumIntegralDigits = InArgs._MinimumIntegralDigits;
	
	ChildSlot
	[
		SNew(SBox)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
#if PLATFORM_WINDOWS || PLATFORM_MAC
		.HeightOverride(38.f)
#else
		.HeightOverride(86.f)
#endif
		[
			SAssignNew(Content, STextBlock)
			.Text(FText::AsNumber(InArgs._TimeNumber, &Options))
			.Justification(ETextJustify::Center)
			.TextStyle(InArgs._TextStyle)
		]
	];

	UpdateSelection(InArgs._bIsSelected);
}

void SAAUTimeItem::UpdateSelection(bool bIsSelected)
{
	if (Content)
	{
		Content->SetColorAndOpacity(bIsSelected ? FLinearColor::Black : FLinearColor(0.246f, 0.246f, 0.246f));
	}
}

void SAAUTimeItem::UpdateTimeNumber(int32 NewTime)
{
	FNumberFormattingOptions Options = FNumberFormattingOptions::DefaultNoGrouping();
	Options.SetMinimumIntegralDigits(SavedMinimumIntegralDigits);
	
	Content->SetText(FText::AsNumber(NewTime, &Options));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
