// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TapPlatformButton.h"
#include "Android/AndroidJava.h"

class FButtonHandle : public FJavaClassObject, public IPlatformButtonHandle
{
public:
	FButtonHandle(UTapPlatformButton* Button);

	virtual ~FButtonHandle() override;

	virtual void Show() override;

	virtual void Close() override;

	virtual void UpdateTexture(const FString IconUrl) override;

	virtual void UpdateLayout(const FGeometry& PlaceHolderGeometry) override;
	
	virtual void SetVisible(bool bNewVisible) override;

	static TMap<int64, TWeakObjectPtr<UTapPlatformButton>> AllButtons;

protected:
	FJavaClassMethod ShowMethod;
	FJavaClassMethod CloseMethod;
	FJavaClassMethod UpdateTextureMethod;
	FJavaClassMethod UpdateLayoutMethod;
	FJavaClassMethod SetVisibleMethod;

	int64 GetNativePtr();
	FJavaClassMethod GetNativePtrMethod;
};
