// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapStyleAntiAddiction.h"
#include "Styles/RealNameWidgetStyle.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/SCompoundWidget.h"


class SAAUTimeItem;

DECLARE_DELEGATE(FOnRealNameClosed);
DECLARE_DELEGATE_ThreeParams(FOnRealNameSubmit, const FString&, const FString&, const FString&);

/**
 * 
 */
class ANTIADDICTION_API SAAURealNameVietnam : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAAURealNameVietnam)
#if PLATFORM_WINDOWS || PLATFORM_MAC
		: _Style(&FTapStyleAntiAddiction::Get().GetWidgetStyle<FRealNameStyle>("RealNameVietnamPC"))
#else
		: _Style(&FTapStyleAntiAddiction::Get().GetWidgetStyle<FRealNameStyle>("RealNameVietnam"))
#endif
		, _Title(NSLOCTEXT("FAntiAddictionModule", "RealNameTitle", "信息确认"))
		, _SubmitText(NSLOCTEXT("FAntiAddictionModule", "RealNameTitle", "Submit"))
		{}

	SLATE_ARGUMENT(const struct FRealNameStyle*, Style);

	SLATE_ARGUMENT(FText, Title);

	SLATE_ARGUMENT(FText, Content);

	SLATE_ARGUMENT(FText, SubmitText);

	SLATE_EVENT(FOnRealNameClosed, OnClosed);

	SLATE_EVENT(FOnRealNameSubmit, OnSubmit);

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void UpdateDayBox(int32 Year, int32 Month);
	
	FReply OnCloseButtonClicked();

	FReply OnSubmitButtonClicked();

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	void OnSelectChangedEvent(TListTypeTraits<TSharedPtr<int32>>::NullableType NewSelectedNumber, ESelectInfo::Type SelectType, int32 ArrayIndex);
	
	TSharedRef<SWidget> OnGenerateWidgetEvent(TSharedPtr<int32> Number, int32 ArrayIndex, int32 MinimumIntegralDigits);

	void OnComboBoxOpeningEvent(int32 ArrayIndex);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	TSharedPtr<STextBlock> TB_Title;

	TSharedPtr<STextBlock> TB_Content;

	TArray<TSharedPtr<int32>> OptionYear;
	
	TArray<TSharedPtr<int32>> OptionMonth;
	
	TArray<TSharedPtr<int32>> OptionDay;

	TArray<TSharedPtr<SImage>> ImageArray;

	TArray<int32> OpenedIndexes;
	
	TArray<TSharedPtr<SComboBox<TSharedPtr<int32>>>> ComboBoxArray;
	
	TArray<TMap<int32, TWeakPtr<SAAUTimeItem>>> AllItemsArray;

	TArray<TSharedPtr<SAAUTimeItem>> ButtonItemArray;

	FOnRealNameSubmit SubmitBlock;

	FOnRealNameClosed CloseBlock;
	
	FSlateFontInfo SavedFont;

	const FSlateBrush* OpenBrush = nullptr;
	const FSlateBrush* CloseBrush = nullptr;
	const FRealNameStyle* SavedStyle = nullptr;
};

