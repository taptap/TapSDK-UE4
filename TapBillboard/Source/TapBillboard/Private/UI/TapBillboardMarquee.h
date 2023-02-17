// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapBillboardModule.h"
#include "TapBillboardTypes.h"
#include "Blueprint/UserWidget.h"
#include "TapBillboardMarquee.generated.h"

class UOverlay;
class UTextBlock;
class UScrollBox;
class UTapMarqueeItem;
class UImage;
class USpacer;

/**
 * 
 */
UCLASS(Abstract)
class TAPBILLBOARD_API UTapBillboardMarquee : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateStyle(const FAnnouncementStyleData& StyleData);

	void StartAnnouncementLoop();
	
	void StopAnnouncementLoop();

	FORCEINLINE bool IsLoopRunning() const
	{
		return CurrentLoop != nullptr;
	}

	TSubclassOf<UTapMarqueeItem> GetItemClass() const
	{
		return ItemClass;
	}

	void UpdateScrollSpeed(float NewSpeed);
	
protected:
	void NextAnnouncement();
	
	void ShowMarqueeItem(const FAnnouncementDetailData& DetailData, bool bMarkReadWhenFinished);

	void OnRightSideDisplay(UTapMarqueeItem* Item);

	void OnItemClipped(UTapMarqueeItem* Item);

	void OnOrientationChanged(int32 Orientation);
	
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	TSharedPtr<FAnnouncementDetailData> CurrentLoop;

	UPROPERTY()
	TArray<int64> ScrollingIds;

	UPROPERTY()
	int32 LeftLoopCount;

	UPROPERTY(Meta = (BindWidget))
	UImage* BGLeft;
	UPROPERTY(Meta = (BindWidget))
	UImage* BGWidget;
	UPROPERTY(Meta = (BindWidget))
	UImage* BGRight;
	
	UPROPERTY(Meta = (BindWidget))
	UScrollBox* ItemPanel;

	UPROPERTY(Meta = (BindWidget))
	UOverlay* LayoutOverlay;

	UPROPERTY(Meta = (BindWidget))
	USpacer* PlaceHolder;
	UPROPERTY(Meta = (BindWidget))
	USpacer* PlaceHolderRight;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTapMarqueeItem> ItemClass;
	
	UPROPERTY(EditDefaultsOnly)
	float ScrollSpeed = 60.f;
	
	UPROPERTY()
	float Offset;

	UPROPERTY()
	TSet<UTapMarqueeItem*> PendingRemove;
	
	FTimerHandle DelayTimer;
	
	FTapBillboardPtr BillboardInterface;
};

 



