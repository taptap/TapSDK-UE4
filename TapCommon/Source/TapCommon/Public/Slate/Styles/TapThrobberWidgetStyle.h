// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "TapThrobberWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TAPCOMMON_API FTapThrobberStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FTapThrobberStyle();
	virtual ~FTapThrobberStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FTapThrobberStyle& GetDefault();

	UPROPERTY(EditAnywhere, Category=BackGround)
	FSlateBrush BackGroundBrush;

	UPROPERTY(EditAnywhere, Category=BackGround)
	FSlateBrush BackGroundBrush_PC;

	UPROPERTY(EditAnywhere, Category=BackGround)
	FSlateBrush BackGroundBrush_PC_Text;
	
	UPROPERTY(EditAnywhere, Category=BackGround)
	FSlateBrush ShadowBrush;

	UPROPERTY(EditAnywhere, Category=Throbber)
	FSlateBrush PieceImage;
	
	UPROPERTY(EditAnywhere, Category=Throbber)
	FSlateBrush PieceImage_PC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Throbber)
	FSlateColor ColorAndOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TextBlock)
	FTextBlockStyle ContentStyle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TextBlock)
	FTextBlockStyle ContentStyle_PC;

	UPROPERTY(EditAnywhere)
	float RotationRate = 0.f;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UTapThrobberWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FTapThrobberStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&WidgetStyle);
	}
};
