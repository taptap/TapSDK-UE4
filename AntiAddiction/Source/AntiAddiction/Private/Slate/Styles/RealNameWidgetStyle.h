// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "RealNameWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ANTIADDICTION_API FRealNameStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FRealNameStyle();
	virtual ~FRealNameStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FRealNameStyle& GetDefault();

	UPROPERTY(EditAnywhere)
	FMargin WidgetMargin;

	UPROPERTY(EditAnywhere)
	FMargin ContentTextPadding;

	UPROPERTY(EditAnywhere)
	FMargin DateTimePadding;

	UPROPERTY(EditAnywhere)
	FMargin SubmitPadding;
	
	UPROPERTY(EditAnywhere)
	FSlateBrush BackGroundBrush;

	UPROPERTY(EditAnywhere)
	FSlateBrush CloseButtonBrush;

	UPROPERTY(EditAnywhere)
	FSlateBrush SubmitBrush;
	
	UPROPERTY(EditAnywhere)
	FTextBlockStyle TitleTextStyle;

	UPROPERTY(EditAnywhere)
	FTextBlockStyle ContentTextStyle;

	UPROPERTY(EditAnywhere)
	FTextBlockStyle SubmitButtonTextStyle;

	UPROPERTY(EditAnywhere, Category = "ComboBox")
	FTableRowStyle ComboItemStyle;

	UPROPERTY(EditAnywhere, Category = "ComboBox")
	FComboBoxStyle ComboBoxStyle;

	UPROPERTY(EditAnywhere, Category = "ComboBox")
	FMargin ContentPadding;

	UPROPERTY(EditAnywhere, Category = "ComboBox")
	FSlateFontInfo Font;

	UPROPERTY(EditAnywhere, Category = "ComboBox")
	FSlateColor ForegroundColor;

	UPROPERTY(EditAnywhere, Category = "ComboBox")
	FSlateBrush OpenComboBrush;

	UPROPERTY(EditAnywhere, Category = "ComboBox")
	FSlateBrush CloseComboBrush;

	UPROPERTY(EditAnywhere)
	FTextBlockStyle ItemStyle;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class URealNameWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FRealNameStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&WidgetStyle);
	}
};
