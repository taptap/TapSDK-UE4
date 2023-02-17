// Fill out your copyright notice in the Description page of Project Settings.


#include "TapBillboard.h"

#include "Android/AndroidApplication.h"
#include "Android/AndroidJavaEnv.h"
#include <jni.h>

#include "TUHelper.h"
#include "Android/AndroidPlatform.h"
#include "Android/AndroidJNI.h"

TMap<int64, FTapBillboard*> FTapBillboard::AllAndroidBillboard;

FTapBillboard::FTapBillboard()
	: FJavaClassObject("com.tds.UEBillboard", "(J)V", reinterpret_cast<int64>(this))
	, InitMethod(GetClassMethod("init", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;I)V"))
	, OpenPanelMethod(GetClassMethod("openPanel", "()V"))
	, OpenSplashPanelMethod(GetClassMethod("openSplashPanel", "()V"))
	, CloseSplashPanelMethod(GetClassMethod("closeSplashPanel", "()V"))
	, StartFetchMarqueeDataMethod(GetClassMethod("startFetchMarqueeData", "()V"))
	, StopFetchMarqueeDataMethod(GetClassMethod("stopFetchMarqueeData", "(Z)V"))
{
	AllAndroidBillboard.Add(reinterpret_cast<int64>(this), this);
}

FTapBillboard::~FTapBillboard()
{
	AllAndroidBillboard.Remove(reinterpret_cast<int64>(this));
}
void FTapBillboard::Init(const FTUConfig& InConfig)
{
	FTapBillboardCommon::Init(InConfig);
	if (CheckConfig().IsValid())
	{
		return;
	}
	
	if (InConfig.BillboardConfig)
	{
		JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();

		FScopedJavaObject<jstring> jClientID = FJavaHelper::ToJavaString(JEnv, InConfig.ClientID);
		FScopedJavaObject<jstring> jClientToken = FJavaHelper::ToJavaString(JEnv, InConfig.ClientToken);
		FScopedJavaObject<jstring> jServerUrl = FJavaHelper::ToJavaString(JEnv, InConfig.ServerURL);
		
		FScopedJavaObject<jstring> jBillboardUrl = FJavaHelper::ToJavaString(JEnv, InConfig.BillboardConfig->BillboardUrl);

		auto jDimensionString = NewScopedJavaObject(JEnv, (jobjectArray)JEnv->NewObjectArray(InConfig.BillboardConfig->Dimensions.Num() * 2, FJavaWrapper::JavaStringClass, nullptr));
		int32 Index = 0;
		for (TTuple<FString, FString>& T : InConfig.BillboardConfig->Dimensions)
		{
			JEnv->SetObjectArrayElement(*jDimensionString, Index++, *FJavaHelper::ToJavaString(JEnv, T.Key));
			JEnv->SetObjectArrayElement(*jDimensionString, Index++, *FJavaHelper::ToJavaString(JEnv, T.Value));
		}
		
		CallMethod<void>(InitMethod, *jClientID, *jClientToken, *jServerUrl, *jBillboardUrl, *jDimensionString, (InConfig.RegionType == ERegionType::CN ? 0 : 1));
	}
	else
	{
		ensure(false);
	}
}

void FTapBillboard::OpenPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose)
{
	if (NavigateSuccess.IsBound())
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Duplicate operation.")));
		return;
	}
	NavigateSuccess = OnSuccess;
	NavigateFailed = OnFailed;
	NavigateClose = OnClose;
	CallMethod<void>(OpenPanelMethod);
}

void FTapBillboard::OpenSplashPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose)
{
	if (SplashSuccess.IsBound())
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Duplicate operation.")));
		return;
	}
	SplashSuccess = OnSuccess;
	SplashFailed = OnFailed;
	SplashClose = OnClose;
	CallMethod<void>(OpenSplashPanelMethod);
}

void FTapBillboard::CloseSplashPanel()
{
	CallMethod<void>(CloseSplashPanelMethod);
}

void FTapBillboard::StartFetchMarqueeData()
{
	CallMethod<void>(StartFetchMarqueeDataMethod);
}

void FTapBillboard::StopFetchMarqueeData(bool bCloseNow)
{
	CallMethod<void>(StopFetchMarqueeDataMethod, bCloseNow);
}

void FTapBillboard::HandleCustomUrl(const FString& Url)
{
	OnCustomLinkClicked.Broadcast(Url);
}

void FTapBillboard::HandleAudioStatusChanged(bool newPlaying)
{
	OnAudioOutputStateChanged.ExecuteIfBound(newPlaying);
}

void FTapBillboard::HandleNavigateOpenSuccess()
{
	NavigateSuccess.ExecuteIfBound();
}

void FTapBillboard::HandleNavigateOpenFailed(const FTUError& Error)
{
	NavigateFailed.ExecuteIfBound(Error);
	NavigateSuccess.Unbind();
	NavigateFailed.Unbind();
	NavigateClose.Unbind();
}

void FTapBillboard::HandleNavigateClosed()
{
	NavigateClose.ExecuteIfBound();
	NavigateSuccess.Unbind();
	NavigateFailed.Unbind();
	NavigateClose.Unbind();
}

void FTapBillboard::HandleSplashOpenSuccess()
{
	SplashSuccess.ExecuteIfBound();
}

void FTapBillboard::HandleSplashOpenFailed(const FTUError& Error)
{
	SplashFailed.ExecuteIfBound(Error);
	SplashSuccess.Unbind();
	SplashFailed.Unbind();
	SplashClose.Unbind();
}

void FTapBillboard::HandleSplashClosed()
{
	SplashClose.ExecuteIfBound();
	SplashSuccess.Unbind();
	SplashFailed.Unbind();
	SplashClose.Unbind();
}

JNI_METHOD void Java_com_tds_UEBillboard_NotifyCustomUrl(JNIEnv* JEnv, jobject jBillboard, jlong nativePtr, jstring Url)
{
	if (FTapBillboard* Ptr = FTapBillboard::AllAndroidBillboard.FindRef(nativePtr))
	{
		FString URL = FJavaHelper::FStringFromParam(JEnv, Url);
		TUHelper::PerformOnGameThread( [Ptr, URL](){ Ptr->HandleCustomUrl(URL); });
	}
}

JNI_METHOD void Java_com_tds_UEBillboard_NotifyAudioStatusChanged(JNIEnv* JEnv, jobject jBillboard, jlong nativePtr, bool newPlaying)
{
	if (FTapBillboard* Ptr = FTapBillboard::AllAndroidBillboard.FindRef(nativePtr))
	{
		TUHelper::PerformOnGameThread( [Ptr, newPlaying](){ Ptr->HandleAudioStatusChanged(newPlaying); });
	}
}

JNI_METHOD void Java_com_tds_UEBillboard_NotifyNavigateOpenSuccess(JNIEnv* JEnv, jobject jBillboard, jlong nativePtr)
{
	if (FTapBillboard* Ptr = FTapBillboard::AllAndroidBillboard.FindRef(nativePtr))
	{
		TUHelper::PerformOnGameThread( [Ptr](){ Ptr->HandleNavigateOpenSuccess(); });
	}
}
JNI_METHOD void Java_com_tds_UEBillboard_NotifyNavigateOpenFailed(JNIEnv* JEnv, jobject jBillboard, jlong nativePtr, jint code, jstring message)
{
	if (FTapBillboard* Ptr = FTapBillboard::AllAndroidBillboard.FindRef(nativePtr))
	{
		FTUError Error(static_cast<int32>(code), FJavaHelper::FStringFromParam(JEnv, message));
		TUHelper::PerformOnGameThread( [Ptr, Error](){ Ptr->HandleNavigateOpenFailed(Error); });
	}
}
JNI_METHOD void Java_com_tds_UEBillboard_NotifyNavigateClosed(JNIEnv* JEnv, jobject jBillboard, jlong nativePtr)
{
	if (FTapBillboard* Ptr = FTapBillboard::AllAndroidBillboard.FindRef(nativePtr))
	{
		TUHelper::PerformOnGameThread( [Ptr](){ Ptr->HandleNavigateClosed(); });
	}
}

JNI_METHOD void Java_com_tds_UEBillboard_NotifySplashOpenSuccess(JNIEnv* JEnv, jobject jBillboard, jlong nativePtr)
{
	if (FTapBillboard* Ptr = FTapBillboard::AllAndroidBillboard.FindRef(nativePtr))
	{
		TUHelper::PerformOnGameThread( [Ptr](){ Ptr->HandleSplashOpenSuccess(); });
	}
}
JNI_METHOD void Java_com_tds_UEBillboard_NotifySplashOpenFailed(JNIEnv* JEnv, jobject jBillboard, jlong nativePtr, jint code, jstring message)
{
	if (FTapBillboard* Ptr = FTapBillboard::AllAndroidBillboard.FindRef(nativePtr))
	{
		FTUError Error(static_cast<int32>(code), FJavaHelper::FStringFromParam(JEnv, message));
		TUHelper::PerformOnGameThread( [Ptr, Error](){ Ptr->HandleSplashOpenFailed(Error); });
	}
}
JNI_METHOD void Java_com_tds_UEBillboard_NotifySplashClosed(JNIEnv* JEnv, jobject jBillboard, jlong nativePtr)
{
	if (FTapBillboard* Ptr = FTapBillboard::AllAndroidBillboard.FindRef(nativePtr))
	{
		TUHelper::PerformOnGameThread( [Ptr](){ Ptr->HandleSplashClosed(); });
	}
}


