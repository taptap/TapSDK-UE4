// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonHandle.h"

#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidWindow.h"
#include "Android/AndroidPlatform.h"


TMap<int64, TWeakObjectPtr<UTapPlatformButton>> FButtonHandle::AllButtons;

FButtonHandle::FButtonHandle(UTapPlatformButton* Button)
	: FJavaClassObject("com/tds/TapPlatformButton", "(J)V", reinterpret_cast<int64>(Button))
	, ShowMethod(GetClassMethod("Show", "()V"))
	, CloseMethod(GetClassMethod("Close", "()V"))
	, UpdateTextureMethod(GetClassMethod("UpdateTexture", "(Ljava/lang/String;)V"))
	, UpdateLayoutMethod(GetClassMethod("UpdateLayout", "(IIII)V"))
	, SetVisibleMethod(GetClassMethod("SetVisible", "(Z)V"))
	, GetNativePtrMethod(GetClassMethod("GetNativePtr", "()J"))
{
	AllButtons.Add(reinterpret_cast<int64>(Button), Button);
}

FButtonHandle::~FButtonHandle()
{
	AllButtons.Remove(GetNativePtr());
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	JEnv->DeleteGlobalRef(Object);
	JEnv->DeleteGlobalRef(Class);
}

void FButtonHandle::Show()
{
	CallMethod<void>(ShowMethod);
}

void FButtonHandle::Close()
{
	CallMethod<void>(CloseMethod);
}
void FButtonHandle::UpdateTexture(const FString IconUrl)
{
	CallMethod<void>(UpdateTextureMethod, *GetJString(IconUrl));
}
void FButtonHandle::UpdateLayout(const FGeometry& PlaceHolderGeometry)
{
	float UIScale;
	int32_t ScreenWidth, ScreenHeight;
	FPlatformRect ScreenRect = FAndroidWindow::GetScreenRect();
	FAndroidWindow::CalculateSurfaceSize(ScreenWidth, ScreenHeight);
	if (ScreenWidth > ScreenHeight)
	{
		UIScale = (float)ScreenHeight / (ScreenRect.Bottom - ScreenRect.Top);
	}
	else
	{
		UIScale = (float)ScreenHeight / (ScreenRect.Bottom - ScreenRect.Top);
	}
	FVector2D Position = PlaceHolderGeometry.GetAccumulatedRenderTransform().GetTranslation() * UIScale;
	FVector2D Size = TransformVector(PlaceHolderGeometry.GetAccumulatedRenderTransform(), PlaceHolderGeometry.GetLocalSize()) * UIScale;
	FIntPoint IntPos(FMath::RoundToInt(Position.X), FMath::RoundToInt(Position.Y));
	FIntPoint IntSize = FIntPoint(FMath::RoundToInt(Position.X + Size.X), FMath::RoundToInt(Size.Y + Position.Y)) - IntPos;

	CallMethod<void>(UpdateLayoutMethod, IntPos.X, IntPos.Y, IntSize.X, IntSize.Y);
}
void FButtonHandle::SetVisible(bool bNewVisible)
{
	CallMethod<void>(SetVisibleMethod, bNewVisible);
}

int64 FButtonHandle::GetNativePtr()
{
	return CallMethod<int64>(GetNativePtrMethod);
}

JNI_METHOD void Java_com_tds_TapPlatformButton_NotifyButtonClicked(JNIEnv* JEnv, jobject jBillboard, jlong nativePtr)
{
	TWeakObjectPtr<UTapPlatformButton> NativeButton = FButtonHandle::AllButtons.FindRef(nativePtr);
	if (IsValid(NativeButton.Get()))
	{
		AsyncTask(ENamedThreads::GameThread, [NativeButton](){NativeButton->OnClicked.ExecuteIfBound();});
	}
}

