// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapStyleAntiAddiction.h"
#include "Styles/RealNameWidgetStyle.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnSelectChanged, int32);

/**
 * 
 */
class ANTIADDICTION_API SAAUTimeItem : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAAUTimeItem)
		: _bIsSelected(false)
		, _MinimumIntegralDigits(1)
		, _TextStyle()
		{
		}

	SLATE_ARGUMENT(int32, TimeNumber)

	SLATE_ARGUMENT(bool, bIsSelected)

	SLATE_ARGUMENT(int32, MinimumIntegralDigits)

	SLATE_ARGUMENT(const FTextBlockStyle*, TextStyle)
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void UpdateSelection(bool bIsSelected);

	void UpdateTimeNumber(int32 NewTime);

	TSharedPtr<STextBlock> Content;

	FOnSelectChanged OnSelectChanged;

	int32 SavedMinimumIntegralDigits;
};


