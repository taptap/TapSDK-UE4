// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/Styles/TapThrobberWidgetStyle.h"


FTapThrobberStyle::FTapThrobberStyle()
{
}

FTapThrobberStyle::~FTapThrobberStyle()
{
}

const FName FTapThrobberStyle::TypeName(TEXT("FTapThrobberStyle"));

const FTapThrobberStyle& FTapThrobberStyle::GetDefault()
{
	static FTapThrobberStyle Default;
	return Default;
}

void FTapThrobberStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
	OutBrushes.Add(&BackGroundBrush);
	OutBrushes.Add(&PieceImage);

	ContentStyle.GetResources(OutBrushes);
}
