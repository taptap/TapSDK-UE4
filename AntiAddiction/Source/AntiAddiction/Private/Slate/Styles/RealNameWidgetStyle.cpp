// Fill out your copyright notice in the Description page of Project Settings.


#include "RealNameWidgetStyle.h"


FRealNameStyle::FRealNameStyle()
{
}

FRealNameStyle::~FRealNameStyle()
{
}

const FName FRealNameStyle::TypeName(TEXT("FRealNameStyle"));

const FRealNameStyle& FRealNameStyle::GetDefault()
{
	static FRealNameStyle Default;
	return Default;
}

void FRealNameStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
	OutBrushes.Add(&BackGroundBrush);
	OutBrushes.Add(&CloseButtonBrush);
	OutBrushes.Add(&SubmitBrush);
	OutBrushes.Add(&OpenComboBrush);
	OutBrushes.Add(&CloseComboBrush);

	TitleTextStyle.GetResources(OutBrushes);
	ContentTextStyle.GetResources(OutBrushes);
	SubmitButtonTextStyle.GetResources(OutBrushes);
	ComboItemStyle.GetResources(OutBrushes);
	ComboBoxStyle.GetResources(OutBrushes);
}
