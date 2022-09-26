// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/STapThrobber.h"

#include "SlateOptMacros.h"
#include "Slate/TapStyleCommon.h"
#include "Slate/WidgetTransform.h"
#include "Slate/Styles/TapThrobberWidgetStyle.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Images/SThrobber.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STapThrobber::Construct(const FArguments& InArgs)
{
	const FTapThrobberStyle* StyleP = &FTapStyleCommon::Get().GetWidgetStyle<FTapThrobberStyle>("TapThrobberStyle");

	SetVisibility(EVisibility::SelfHitTestInvisible);

	OnRemoveSelf = InArgs._OnRemoveSelf;
	SavedRotationRate = StyleP->RotationRate;

	ChildSlot
	[
		SNew(SBorder)
		.Padding(0.f)
		.BorderBackgroundColor(FLinearColor())
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Visibility(EVisibility::SelfHitTestInvisible)
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.BorderImage(&StyleP->BackGroundBrush)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(25.f)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SAssignNew(Throbber, SImage)
						.Image(&StyleP->PieceImage)
						.RenderTransform(Transform.ToSlateRenderTransform())
						.RenderTransformPivot(FVector(0.5f))
						.ColorAndOpacity(StyleP->ColorAndOpacity)
				]
				+ SVerticalBox::Slot()
				.Padding(30.f)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SAssignNew(ContentText, STextBlock)
						.TextStyle(&StyleP->ContentStyle)
						.Justification(ETextJustify::Center)
				]
			]
		]
	];

	ShowThrobber(InArgs._HasThrobber);
	UpdateContent(InArgs._Content);
	UpdateTimeInterval(InArgs._TimeInterval);
}

void STapThrobber::ShowThrobber(bool bNewShow)
{
	Throbber->SetVisibility(bNewShow ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed);
}

void STapThrobber::UpdateContent(const FText& NewContent)
{
	ContentText->SetText(NewContent);
	ContentText->SetVisibility(NewContent.IsEmpty() ? EVisibility::Collapsed : EVisibility::SelfHitTestInvisible);
}

void STapThrobber::UpdateTimeInterval(float NewTime)
{
	if (NewTime > 0.f)
	{
		RemoveTimerHandle = RegisterActiveTimer( NewTime, FWidgetActiveTimerDelegate::CreateSP( this, &STapThrobber::TimerRemoveTapThrobber));
	}
	else
	{
		if (RemoveTimerHandle.IsValid())
		{
			UnRegisterActiveTimer(RemoveTimerHandle.ToSharedRef());
			RemoveTimerHandle.Reset();
		}
	}
}

void STapThrobber::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	Transform.Angle = Transform.Angle + SavedRotationRate * InDeltaTime;
	Throbber->SetRenderTransform(Transform.ToSlateRenderTransform());
}

EActiveTimerReturnType STapThrobber::TimerRemoveTapThrobber(double InCurrentTime, float InDeltaTime)
{
	if (OnRemoveSelf.IsBound())
	{
		OnRemoveSelf.Execute(StaticCastSharedRef<STapThrobber>(AsShared()));
	}
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(AsShared());
		RemoveTimerHandle.Reset();
	}
	return EActiveTimerReturnType::Stop;
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
