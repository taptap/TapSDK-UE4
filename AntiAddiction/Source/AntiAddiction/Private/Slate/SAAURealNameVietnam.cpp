// Fill out your copyright notice in the Description page of Project Settings.


#include "SAAURealNameVietnam.h"

#include "SAAUTimeItem.h"
#include "SlateOptMacros.h"
#include "Server/AAUHelper.h"
#include "Styles/RealNameWidgetStyle.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SConstraintCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define COMBO_BOX_NUMBER (3)

#define COMBO_BOX_YEAR (0)
#define COMBO_BOX_MONTH (1)
#define COMBO_BOX_DAY (2)

void SAAURealNameVietnam::Construct(const FArguments& InArgs)
{
	ComboBoxArray.SetNum(COMBO_BOX_NUMBER);
	ImageArray.SetNum(COMBO_BOX_NUMBER);
	AllItemsArray.SetNum(COMBO_BOX_NUMBER);
	ButtonItemArray.SetNum(COMBO_BOX_NUMBER);

	SubmitBlock = InArgs._OnSubmit;
	CloseBlock = InArgs._OnClosed;
	SavedFont = InArgs._Style->Font;

	OpenBrush = &InArgs._Style->OpenComboBrush;
	CloseBrush = &InArgs._Style->CloseComboBrush;
	SavedStyle = InArgs._Style;

	ComboBoxArray[0];
	const FDateTime CurrentTime = AAUHelper::GetVietnamCurrentTime();
	const int32 MaxYear = FMath::Max(2022, CurrentTime.GetYear());

	for (int32 Y = MaxYear - 100; Y <= MaxYear; Y++)
	{
		OptionYear.Add(MakeShared<int32>(Y));
	}
	for (int32 M = 1; M <= 12; M++)
	{
		OptionMonth.Add(MakeShared<int32>(M));
	}
	for (int32 D = 1; D <= 30; D++)
	{
		OptionDay.Add(MakeShared<int32>(D));
	}

	const TSharedPtr<int32>& SelectYear = OptionYear[100 - 22];
	const TSharedPtr<int32>& SelectMonth = OptionMonth[5];
	const TSharedPtr<int32>& SelectDay = OptionDay[14];

	ChildSlot
	[
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
			.Alignment(FVector2D(0.5f))
			.Anchors(FAnchors(0.5f))
			.Offset(InArgs._Style->WidgetMargin)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			  .HAlign(HAlign_Fill)
			  .VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(&InArgs._Style->BackGroundBrush)
			]
			+ SOverlay::Slot()
			  .HAlign(HAlign_Fill)
			  .VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
#if PLATFORM_IOS || PLATFORM_ANDROID
						.Padding(0.f, 32.f)
#else
						.Padding(0.f, 12.f)
#endif
					[
						SAssignNew(TB_Title, STextBlock)
						.Text(InArgs._Title)
						.TextStyle(&InArgs._Style->TitleTextStyle)
					]
					+ SOverlay::Slot()
					  .HAlign(HAlign_Right)
					  .VAlign(VAlign_Center)
					[
						SNew(SButton)
						.ButtonColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 0.f))
						.OnClicked(this, &SAAURealNameVietnam::OnCloseButtonClicked)
						.ContentPadding(20.f)
						[
							SNew(SImage)
							.Image(&InArgs._Style->CloseButtonBrush)
						]
					]
				]
				+ SVerticalBox::Slot()
				  .FillHeight(1.f)
				  .HAlign(HAlign_Fill)
				  .VAlign(VAlign_Bottom)
				  .Padding(InArgs._Style->ContentTextPadding)
				  // .Padding(54.f, 0.f)
				[
					SAssignNew(TB_Content, STextBlock)
					.Text(InArgs._Content)
					.TextStyle(&InArgs._Style->ContentTextStyle)
#if PLATFORM_IOS || PLATFORM_ANDROID
					.Justification(ETextJustify::Left)
#else
					.Justification(ETextJustify::Center)
#endif
					.AutoWrapText(true)
				]
				+ SVerticalBox::Slot()
				  .AutoHeight()
				  .Padding(InArgs._Style->DateTimePadding)
				  .HAlign(HAlign_Center)
				  // .Padding(54.f, 0.f, 54.f, 100.f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						[
							SAssignNew(ImageArray[COMBO_BOX_YEAR], SImage)
							.Image(CloseBrush)
							.Visibility(EVisibility::SelfHitTestInvisible)
						]
						+ SOverlay::Slot()
						.Padding(1.f)
						[
							SAssignNew(ComboBoxArray[COMBO_BOX_YEAR], SComboBox<TSharedPtr<int32>>)
							.OptionsSource(&OptionYear)
							.InitiallySelectedItem(SelectYear)
							.ItemStyle(&InArgs._Style->ComboItemStyle)
							.ComboBoxStyle(&InArgs._Style->ComboBoxStyle)
							.ContentPadding(InArgs._Style->ContentPadding)
							.ForegroundColor(InArgs._Style->ForegroundColor)
							.CustomScrollbar(SNew(SScrollBar).Visibility(EVisibility::Collapsed))
#if PLATFORM_MAC
							.IsFocusable(false)
#endif
							.MaxListHeight(300.f)
							.OnSelectionChanged(this, &SAAURealNameVietnam::OnSelectChangedEvent, 0)
							.OnGenerateWidget(this, &SAAURealNameVietnam::OnGenerateWidgetEvent, 0, 4)
							.OnComboBoxOpening(this, &SAAURealNameVietnam::OnComboBoxOpeningEvent, 0)
							[
								SAssignNew(ButtonItemArray[0], SAAUTimeItem)
								.TimeNumber(*SelectYear)
								.bIsSelected(true)
								.MinimumIntegralDigits(4)
								.TextStyle(&InArgs._Style->ItemStyle)
							]
						]
					]
					+ SHorizontalBox::Slot()
					  .Padding(20.f, 0.f)
					  .FillWidth(1.f)
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						[
							SAssignNew(ImageArray[COMBO_BOX_MONTH], SImage)
							.Image(CloseBrush)
							.Visibility(EVisibility::SelfHitTestInvisible)
						]
						+ SOverlay::Slot()
						.Padding(1.f)
						[
							SAssignNew(ComboBoxArray[COMBO_BOX_MONTH], SComboBox<TSharedPtr<int32>>)
							.OptionsSource(&OptionMonth)
							.InitiallySelectedItem(SelectMonth)
							.ItemStyle(&InArgs._Style->ComboItemStyle)
							.ComboBoxStyle(&InArgs._Style->ComboBoxStyle)
							.ContentPadding(InArgs._Style->ContentPadding)
							.ForegroundColor(InArgs._Style->ForegroundColor)
							.CustomScrollbar(SNew(SScrollBar).Visibility(EVisibility::Collapsed))
#if PLATFORM_MAC
							.IsFocusable(false)
#endif
							.MaxListHeight(300.f)
							.OnSelectionChanged(this, &SAAURealNameVietnam::OnSelectChangedEvent, 1)
							.OnGenerateWidget(this, &SAAURealNameVietnam::OnGenerateWidgetEvent, 1, 2)
							.OnComboBoxOpening(this, &SAAURealNameVietnam::OnComboBoxOpeningEvent, 1)
							[
								SAssignNew(ButtonItemArray[1], SAAUTimeItem)
								.TimeNumber(*SelectMonth)
								.bIsSelected(true)
								.MinimumIntegralDigits(2)
								.TextStyle(&InArgs._Style->ItemStyle)
							]
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						[
							SAssignNew(ImageArray[COMBO_BOX_DAY], SImage)
							.Image(CloseBrush)
							.Visibility(EVisibility::SelfHitTestInvisible)
						]
						+ SOverlay::Slot()
						.Padding(1.f)
						[
							SAssignNew(ComboBoxArray[COMBO_BOX_DAY], SComboBox<TSharedPtr<int32>>)
							.OptionsSource(&OptionDay)
							.InitiallySelectedItem(SelectDay)
							.ItemStyle(&InArgs._Style->ComboItemStyle)
							.ComboBoxStyle(&InArgs._Style->ComboBoxStyle)
							.ContentPadding(InArgs._Style->ContentPadding)
							.ForegroundColor(InArgs._Style->ForegroundColor)
							.CustomScrollbar(SNew(SScrollBar).Visibility(EVisibility::Collapsed))
#if PLATFORM_MAC
							.IsFocusable(false)
#endif
							.MaxListHeight(300.f)
							.OnSelectionChanged(this, &SAAURealNameVietnam::OnSelectChangedEvent, 2)
							.OnGenerateWidget(this, &SAAURealNameVietnam::OnGenerateWidgetEvent, 2, 2)
							.OnComboBoxOpening(this, &SAAURealNameVietnam::OnComboBoxOpeningEvent, 2)
							[
								SAssignNew(ButtonItemArray[2], SAAUTimeItem)
								.TimeNumber(*SelectDay)
								.bIsSelected(true)
								.MinimumIntegralDigits(2)
								.TextStyle(&InArgs._Style->ItemStyle)
							]
						]
					]
				]
				+ SVerticalBox::Slot()
				  .AutoHeight()
				  .Padding(InArgs._Style->SubmitPadding)
				  // .Padding(54.f, 0.f, 54.f, 60.f)
				  .HAlign(HAlign_Center)
				  .VAlign(VAlign_Bottom)
				[
					SNew(SButton)
					.OnClicked(this, &SAAURealNameVietnam::OnSubmitButtonClicked)
					.ButtonColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 0.f))
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						[
							SNew(SImage)
							.Image(&InArgs._Style->SubmitBrush)
						]
						+ SOverlay::Slot()
						  .HAlign(HAlign_Center)
						  .VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(InArgs._SubmitText)
							.TextStyle(&InArgs._Style->SubmitButtonTextStyle)
						]
					]
				]
			]
		]
	];
}

void SAAURealNameVietnam::UpdateDayBox(int32 Year, int32 Month)
{
	if (!(Month >= 1 && Month <= 12))
	{
		ensure(false);
		return;
	}

	const int32 AllDays = FDateTime::DaysInMonth(Year, Month);
	int32 OldDay = ComboBoxArray[COMBO_BOX_DAY]->GetSelectedItem()
		               ? *ComboBoxArray[COMBO_BOX_DAY]->GetSelectedItem()
		               : -1;
	if (OptionDay.Num() != AllDays)
	{
		ComboBoxArray[COMBO_BOX_DAY]->ClearSelection();
		OptionDay.SetNum(AllDays);
		for (int32 i = 0; i < AllDays; i++)
		{
			OptionDay[i] = MakeShared<int32>(i + 1);
		}

		ComboBoxArray[COMBO_BOX_DAY]->RefreshOptions();
	}

	int32 Index = INDEX_NONE;
	for (int32 i = 0; i < OptionDay.Num(); ++i)
	{
		if (OptionDay[i] && *OptionDay[i] == OldDay)
		{
			Index = i;
			break;
		}
	}
	if (Index == INDEX_NONE)
	{
		ComboBoxArray[COMBO_BOX_DAY]->SetSelectedItem(OptionDay[14]);
	}
	else
	{
		ComboBoxArray[COMBO_BOX_DAY]->SetSelectedItem(OptionDay[Index]);
	}
}

FReply SAAURealNameVietnam::OnCloseButtonClicked()
{
	if (CloseBlock.IsBound())
	{
		CloseBlock.Execute();
	}
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(AsShared());
	}
	return FReply::Handled();
}

FReply SAAURealNameVietnam::OnSubmitButtonClicked()
{
	if (SubmitBlock.IsBound())
	{
		const FString Y = FString::Printf(TEXT("%04d"), *ComboBoxArray[COMBO_BOX_YEAR]->GetSelectedItem().Get());
		const FString M = FString::Printf(TEXT("%02d"), *ComboBoxArray[COMBO_BOX_MONTH]->GetSelectedItem().Get());
		const FString D = FString::Printf(TEXT("%02d"), *ComboBoxArray[COMBO_BOX_DAY]->GetSelectedItem().Get());
		SubmitBlock.Execute(Y, M, D);
	}
	return FReply::Handled();
}

FReply SAAURealNameVietnam::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
#if PLATFORM_MAC
	ComboBoxArray[COMBO_BOX_DAY]->SetIsOpen(false, false);
	ComboBoxArray[COMBO_BOX_MONTH]->SetIsOpen(false, false);
	ComboBoxArray[COMBO_BOX_YEAR]->SetIsOpen(false, false);
#endif
	return Reply;
}

void SAAURealNameVietnam::OnSelectChangedEvent(TListTypeTraits<TSharedPtr<int32>>::NullableType NewSelectedNumber,
                                               ESelectInfo::Type SelectType, int32 ArrayIndex)
{
	if (ArrayIndex != COMBO_BOX_DAY && ComboBoxArray[COMBO_BOX_YEAR]->GetSelectedItem() && ComboBoxArray[
		COMBO_BOX_MONTH]->GetSelectedItem())
	{
		UpdateDayBox(*ComboBoxArray[COMBO_BOX_YEAR]->GetSelectedItem(),
		             *ComboBoxArray[COMBO_BOX_MONTH]->GetSelectedItem());
	}
	ButtonItemArray[ArrayIndex]->UpdateTimeNumber(NewSelectedNumber ? *NewSelectedNumber : 0);

	const int32 NewNumber = NewSelectedNumber ? *NewSelectedNumber : -1;

	TArray<int32> InvalidKeys;
	for (TTuple<int32, TWeakPtr<SAAUTimeItem>>& T : AllItemsArray[ArrayIndex])
	{
		if (T.Value.IsValid())
		{
			T.Value.Pin()->UpdateSelection(T.Key == NewNumber);
		}
		else
		{
			InvalidKeys.Add(T.Key);
		}
	}

	for(const int32 Key : InvalidKeys)
	{
		AllItemsArray[ArrayIndex].Remove(Key);
	}
}

TSharedRef<SWidget> SAAURealNameVietnam::OnGenerateWidgetEvent(TSharedPtr<int32> Number, int32 ArrayIndex, int32 MinimumIntegralDigits)
{
	const int32 NewNumber = Number ? *Number : 0;

	const int32 SelectedNumber = ComboBoxArray[ArrayIndex]->GetSelectedItem()
		                             ? *ComboBoxArray[ArrayIndex]->GetSelectedItem()
		                             : -1;

	const bool bIsSelected = SelectedNumber == NewNumber;

	TSharedRef<SAAUTimeItem> NewItem = SNew(SAAUTimeItem)
		.TimeNumber(NewNumber)
		.bIsSelected(bIsSelected)
		.MinimumIntegralDigits(MinimumIntegralDigits)
		.TextStyle(&SavedStyle->ItemStyle);

	AllItemsArray[ArrayIndex].Emplace(NewNumber, NewItem);

	return NewItem;
}

void SAAURealNameVietnam::OnComboBoxOpeningEvent(int32 ArrayIndex)
{
	ImageArray[ArrayIndex]->SetImage(OpenBrush);
	OpenedIndexes.AddUnique(ArrayIndex);
}

void SAAURealNameVietnam::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime,
	const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	for (int32 i = 0; i < OpenedIndexes.Num(); ++i)
	{
		if (ComboBoxArray[OpenedIndexes[i]] && !ComboBoxArray[OpenedIndexes[i]]->IsOpen())
		{
			ImageArray[OpenedIndexes[i]]->SetImage(CloseBrush);
			OpenedIndexes.RemoveAt(i);
			break;
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
