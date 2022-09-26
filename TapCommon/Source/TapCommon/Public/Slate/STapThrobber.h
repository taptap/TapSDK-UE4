// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate/WidgetTransform.h"
#include "Widgets/SCompoundWidget.h"

class STapThrobber;

DECLARE_DELEGATE_OneParam(FOnTapThrobberRemoveSelf, const TSharedRef<STapThrobber>&);

/**
 * 活动等待控件
 */
class TAPCOMMON_API STapThrobber : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STapThrobber)
	: _HasThrobber(true)
	, _Content(FText::GetEmpty())
	, _TimeInterval(0.f)
	{}

	/** 是否有旋转等待指示器 */
	SLATE_ARGUMENT( bool, HasThrobber )

	/** 显示文本内容 */
	SLATE_ARGUMENT( FText, Content )

	/** 自动销毁时间 */
	SLATE_ARGUMENT( float, TimeInterval )

	SLATE_EVENT(FOnTapThrobberRemoveSelf, OnRemoveSelf)
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void ShowThrobber(bool bNewShow);

	void UpdateContent(const FText& NewContent);
	
	void UpdateTimeInterval(float NewTime);

	FORCEINLINE void UpdateRemoveSelfDelegate(const FOnTapThrobberRemoveSelf& InDelegate)
	{
		OnRemoveSelf = InDelegate;
	}

protected:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	EActiveTimerReturnType TimerRemoveTapThrobber( double InCurrentTime, float InDeltaTime );
	
	TSharedPtr<SImage> Throbber;

	TSharedPtr<STextBlock> ContentText;

	TSharedPtr<FActiveTimerHandle> RemoveTimerHandle;

	FOnTapThrobberRemoveSelf OnRemoveSelf;

	FWidgetTransform Transform;

	float SavedRotationRate = 0.f;
};

