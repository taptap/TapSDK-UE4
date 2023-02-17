// Fill out your copyright notice in the Description page of Project Settings.


#include "TapMarqueeItem.h"

#include "TapBillboardCommon.h"
#include "TapBillboardModule.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/BorderSlot.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Engine/Font.h"

void UTapMarqueeItem::UpdateItem(const FText& Content, UTexture2DDynamic* InIconTexture, float InStartupOffset, int64 Id, bool bMarkReadWhenFinished)
{
	if (InIconTexture)
	{
		Icon->SetBrushFromTextureDynamic(InIconTexture, true);		
	}
	else
	{
		Icon->SetVisibility(ESlateVisibility::Hidden);
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(FindIconTimer, this, &UTapMarqueeItem::TimerFindIcon, 0.5f, true);
		}
	}
	ContentLabel->SetText(Content);
	MarkReadID = Id;
	bMarkRead = bMarkReadWhenFinished;
}

void UTapMarqueeItem::UpdateStyle(const FAnnouncementStyleData& StyleData)
{
	FColor TextColor = FColor::FromHex(StyleData.default_text_color);
	ContentLabel->SetColorAndOpacity(FSlateColor(TextColor));
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FAnnouncementLayout& Layout = ViewportSize.X > ViewportSize.Y ? StyleData.horizontal : StyleData.vertical;
	if (UBorderSlot* BorderSlot = Cast<UBorderSlot>(MarginWidget->Slot))
	{
		BorderSlot->SetPadding(FMargin(Layout.scroll_margin_x, Layout.scroll_margin_y));
	}
}

void UTapMarqueeItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UPanelWidget* Widget = GetParent())
	{
		FSlateRect ParentRect = Widget->GetCachedGeometry().GetRenderBoundingRect();
		float SelfRight = MyGeometry.GetRenderBoundingRect().Right;

		if (SelfRight < ParentRect.Right)
		{
			OnRightSideDisplay.ExecuteIfBound(this);
		}
		if (SelfRight < ParentRect.Left)
		{
			if (bMarkRead)
			{
				if (FTapBillboardPtr Interface = FTapBillboardModule::GetTapBillboardInterface())
				{
					Interface->Rest_AnnouncementsMarkRead({MarkReadID}, FSimpleDelegate(), FTapFailed());
					Interface->PopMarqueeData(MarkReadID);
				}
			}
			OutsideOfParent.ExecuteIfBound(this);
		}
	}
}

void UTapMarqueeItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (FTapBillboardPtr Interface = FTapBillboardModule::GetTapBillboardInterface())
	{
		if (const UFont* Font = Interface->GetDownloadFont())
		{
			ContentLabel->Font.FontObject = Font;
			ContentLabel->SetFont(ContentLabel->Font);
		}
	}
#if PLATFORM_IOS || PLATFORM_ANDROID
	ContentLabel->Font.Size = 16.f;
#endif
}

void UTapMarqueeItem::TimerFindIcon()
{
	if (FTapBillboardPtr Billboard = FTapBillboardModule::GetTapBillboardInterface())
	{
		if (UTexture2DDynamic* Tex = Billboard->GetMarqueeIconTexture())
		{
			Icon->SetBrushFromTextureDynamic(Tex);
			if (UWorld* World = GetWorld())
			{
				World->GetTimerManager().ClearTimer(FindIconTimer);
				FindIconTimer.Invalidate();
			}
		}
	}
}

