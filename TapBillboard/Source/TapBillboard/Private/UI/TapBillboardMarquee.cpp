// Fill out your copyright notice in the Description page of Project Settings.


#include "TapBillboardMarquee.h"

#include "JsonWrapper.h"
#include "TapBillboardCommon.h"
#include "TapMarqueeItem.h"
#include "Animation/UMGSequencePlayer.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/Spacer.h"

#if PLATFORM_ANDROID || PLATFORM_IOS
#define MARQUEE_HEIGHT (57.6f)
#else
#define MARQUEE_HEIGHT (40.f)
#endif

void UTapBillboardMarquee::UpdateStyle(const FAnnouncementStyleData& StyleData)
{
	if (GEngine && GEngine->GameViewport)
	{
		FColor BGColor = FColor::FromHex(StyleData.content_background_color);
		BGColor.A = 127;
		BGLeft->SetColorAndOpacity(BGColor);
		BGWidget->SetColorAndOpacity(BGColor);
		BGRight->SetColorAndOpacity(BGColor);
		
		if (UCanvasPanelSlot* BGSlot = Cast<UCanvasPanelSlot>(LayoutOverlay->Slot))
		{
			FVector2D ViewportSize;
			GEngine->GameViewport->GetViewportSize(ViewportSize);
			const FAnnouncementLayout& Layout = ViewportSize.X > ViewportSize.Y ? StyleData.horizontal : StyleData.vertical;
			if (Layout.scroll_width_type == TEXT("fixed"))
			{
				BGLeft->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				BGRight->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				FVector2D Size;
				GEngine->GameViewport->GetViewportSize(Size);
				const float DPIScale = UWidgetLayoutLibrary::GetViewportScale(GEngine->GameViewport);
				float Scale = Size.X / 812.f / DPIScale;
				float Width = Layout.scroll_width * Scale + 30.f;
				if (UOverlaySlot* OSlot = Cast<UOverlaySlot>(ItemPanel->Slot))
				{
					OSlot->SetPadding(FMargin(15.f, 0.f));
				}
				if (Layout.position == TEXT("topLeft"))
				{
					BGSlot->SetAnchors(FAnchors(0.f, 0.f));
					BGSlot->SetAlignment(FVector2D(0.f, 0.f));
					BGSlot->SetOffsets(FMargin(Layout.position_offset_x, Layout.position_offset_y, Width, MARQUEE_HEIGHT));
				}
				else if(Layout.position == TEXT("top"))
				{
					BGSlot->SetAnchors(FAnchors(0.5f, 0.f));
					BGSlot->SetAlignment(FVector2D(0.5f, 0.f));
					BGSlot->SetOffsets(FMargin(Layout.position_offset_x, Layout.position_offset_y, Width, MARQUEE_HEIGHT));
				}
				else if(Layout.position == TEXT("topRight"))
				{
					BGSlot->SetAnchors(FAnchors(1.f, 0.f));
					BGSlot->SetAlignment(FVector2D(1.f, 0.f));
					BGSlot->SetOffsets(FMargin(-Layout.position_offset_x, Layout.position_offset_y, Width, MARQUEE_HEIGHT));
				}
				else if(Layout.position == TEXT("bottomLeft"))
				{
					BGSlot->SetAnchors(FAnchors(0.f, 1.f));
					BGSlot->SetAlignment(FVector2D(0.f, 1.f));
					BGSlot->SetOffsets(FMargin(Layout.position_offset_x, -Layout.position_offset_y, Width, MARQUEE_HEIGHT));
				}
				else if(Layout.position == TEXT("bottom"))
				{
					BGSlot->SetAnchors(FAnchors(0.5f, 1.f));
					BGSlot->SetAlignment(FVector2D(0.5f, 1.f));
					BGSlot->SetOffsets(FMargin(Layout.position_offset_x, -Layout.position_offset_y, Width, MARQUEE_HEIGHT));
				}
				else
				{
					ensure(Layout.position == TEXT("bottomRight"));
					BGSlot->SetAnchors(FAnchors(1.f, 1.f));
					BGSlot->SetAlignment(FVector2D(1.f, 1.f));
					BGSlot->SetOffsets(FMargin(-Layout.position_offset_x, -Layout.position_offset_y, Width, MARQUEE_HEIGHT));
				}
			}
			else if(Layout.scroll_width_type == TEXT("fill"))
			{
				BGLeft->SetVisibility(ESlateVisibility::Collapsed);
				BGRight->SetVisibility(ESlateVisibility::Collapsed);
				if (UOverlaySlot* OSlot = Cast<UOverlaySlot>(ItemPanel->Slot))
				{
					OSlot->SetPadding(FMargin(0.f));
				}
				if (Layout.position == TEXT("top"))
				{
					BGSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 0.f));
					BGSlot->SetAlignment(FVector2D(0.f, 0.f));
					BGSlot->SetOffsets(FMargin(0.f, Layout.position_offset_y, 0.f, MARQUEE_HEIGHT));
				}
				else
				{
					ensure(Layout.position == TEXT("bottom"));
					BGSlot->SetAnchors(FAnchors(0.f, 1.f, 1.f, 1.f));
					BGSlot->SetAlignment(FVector2D(0.f, 1.f));
					BGSlot->SetOffsets(FMargin(0.f, -Layout.position_offset_y, 0.f, MARQUEE_HEIGHT));
				}
			}
		}
		else
		{
			ensure(false);
		}
	}
}

void UTapBillboardMarquee::StartAnnouncementLoop()
{
	if (IsLoopRunning())
	{
		return;
	}
	
	Offset = 0.f;
	
	NextAnnouncement();
}

void UTapBillboardMarquee::StopAnnouncementLoop()
{
	if (!IsLoopRunning())
	{
		return;
	}
	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DelayTimer);
	}
	CurrentLoop = nullptr;
}

void UTapBillboardMarquee::UpdateScrollSpeed(float NewSpeed)
{
	ScrollSpeed = NewSpeed;
}

void UTapBillboardMarquee::NextAnnouncement()
{
	if (const FAnnouncementDetailData* DataP = BillboardInterface->FindNextMarqueeData(ScrollingIds))
	{
		CurrentLoop = MakeShared<FAnnouncementDetailData>(*DataP);
	}
	else
	{
		CurrentLoop.Reset();
	}
	
	if (CurrentLoop)
	{
		LeftLoopCount = BillboardInterface->GetMarqueeStyle().repeat_count;
		ShowMarqueeItem(*CurrentLoop, LeftLoopCount == 1);//还剩最后一次的时候发送已读
	}// else 正在显示的等待结束
}

void UTapBillboardMarquee::ShowMarqueeItem(const FAnnouncementDetailData& DetailData, bool bMarkReadWhenFinished)
{
	FText Content;
	TArray<FMarqueeContentElement> Results;
	if (FJsonObjectConverter::JsonArrayStringToUStruct(DetailData.content, &Results, 0, 0))
	{
		FString ContentStr;
		for (const FMarqueeContentElement& Elm : Results)
		{
			if (Elm.type == TEXT("paragraph"))
			{
				for (const FMarqueeContentChild& Child : Elm.children)
				{
					ContentStr += Child.text;
				}
			}
		}
		Content = FText::FromString(ContentStr);
	}

	if (ensure(ItemClass))
	{
		if (UTapMarqueeItem* UI = CreateWidget<UTapMarqueeItem>(this, ItemClass))
		{
			UI->UpdateItem(Content, BillboardInterface->GetMarqueeIconTexture(), ItemPanel->GetCachedGeometry().GetLocalSize().X, DetailData.id, bMarkReadWhenFinished);

			const FAnnouncementStyleData& StyleData = BillboardInterface->GetMarqueeStyle();
			UI->UpdateStyle(StyleData);
			UI->OnRightSideDisplay.BindUObject(this, &UTapBillboardMarquee::OnRightSideDisplay);
			UI->OutsideOfParent.BindUObject(this, &UTapBillboardMarquee::OnItemClipped);
			ItemPanel->RemoveChild(PlaceHolderRight);
			if (UScrollBoxSlot* NewSlot = Cast<UScrollBoxSlot>(ItemPanel->AddChild(UI)))
			{
				NewSlot->SetHorizontalAlignment(HAlign_Fill);
				NewSlot->SetVerticalAlignment(VAlign_Fill);
				
				ItemPanel->AddChild(PlaceHolderRight);
				float SizeX = ItemPanel->GetCachedGeometry().Size.X;
				PlaceHolderRight->SetSize(FVector2D(SizeX + 100.f, 1.f));
			}
			else
			{
				ensure(false);
			}

			ScrollingIds.Add(DetailData.id);
		}
	}
}

void UTapBillboardMarquee::OnRightSideDisplay(UTapMarqueeItem* Item)
{
	Item->OnRightSideDisplay.Unbind();
	--LeftLoopCount;

	if (LeftLoopCount == 0)
	{
		NextAnnouncement();
	}
	else
	{
		ShowMarqueeItem(*CurrentLoop, LeftLoopCount == 1);
	}
}

void UTapBillboardMarquee::OnItemClipped(UTapMarqueeItem* Item)
{
	Item->OutsideOfParent.Unbind();
	PendingRemove.Add(Item);
	ScrollingIds.RemoveSingle(Item->MarkReadID);
	if (ItemPanel->GetChildrenCount() - PendingRemove.Num() == 2)
	{
		ensure(ItemPanel->GetChildAt(0) == PlaceHolder && ItemPanel->GetChildAt(ItemPanel->GetChildrenCount() - 1) == PlaceHolderRight);
		StopAnnouncementLoop();
		RemoveFromParent();
	}
}

void UTapBillboardMarquee::OnOrientationChanged(int32 Orientation)
{
	const FAnnouncementStyleData& StyleDate = BillboardInterface->GetMarqueeStyle();
	UpdateStyle(StyleDate);
	for (int32 i = 0; i < ItemPanel->GetChildrenCount(); ++i)
	{
		if (UTapMarqueeItem* Item = Cast<UTapMarqueeItem>(ItemPanel->GetChildAt(i)))
		{
			Item->UpdateStyle(StyleDate);
		}
	}
}

void UTapBillboardMarquee::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	BillboardInterface = FTapBillboardModule::GetTapBillboardInterface();

	UpdateStyle(BillboardInterface->GetMarqueeStyle());

	UpdateScrollSpeed(BillboardInterface->GetMarqueeSpeed());

	FCoreDelegates::ApplicationReceivedScreenOrientationChangedNotificationDelegate.AddUObject(this, &UTapBillboardMarquee::OnOrientationChanged);
}

void UTapBillboardMarquee::NativeConstruct()
{
	Super::NativeConstruct();
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &UTapBillboardMarquee::StartAnnouncementLoop, 0.5f);
}

void UTapBillboardMarquee::NativeDestruct()
{
	Super::NativeDestruct();
	StopAnnouncementLoop();
}

void UTapBillboardMarquee::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	for (UTapMarqueeItem* Item : PendingRemove)
	{
		if (IsValid(Item))
		{
			Offset -= Item->GetCachedGeometry().Size.X;
			Item->RemoveFromParent();
		}
	}
	PendingRemove.Reset();

	float SizeX = ItemPanel->GetCachedGeometry().Size.X;
	if (SizeX != PlaceHolder->Size.X)
	{
		PlaceHolder->SetSize(FVector2D(SizeX, 1.f));
	}
	if (SizeX + 100.f != PlaceHolderRight->Size.X)
	{
		PlaceHolderRight->SetSize(FVector2D(SizeX + 100.f, 1.f));
	}

	Offset += ScrollSpeed * InDeltaTime;
	ItemPanel->SetScrollOffset(Offset);
}
