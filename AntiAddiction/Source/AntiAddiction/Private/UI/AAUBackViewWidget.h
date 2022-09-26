#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "AAUBackViewWidget.generated.h"

/**
 *  
 */
UCLASS()
class ANTIADDICTION_API UAAUBackViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAAUBackViewWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UOverlay* HeadOverlay;

	UPROPERTY(meta = (BindWidget))
	UButton* SubButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SubButtonLabel;
	

protected:

	virtual void NativeConstruct() override;
	
};
