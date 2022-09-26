#include "TUDebuger.h"

#if PLATFORM_IOS
#import <TapCommonSDK/TapCommonSDK.h>
#include "IOSHelper.h"
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
#endif

#if PLATFORM_ANDROID
#include "Engine.h"
#include "Core.h"
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#include "Runtime/Core/Public/Android/AndroidJava.h"
#define UNREAL4_CLASS_NAME_DEBUG_COMMON "com/tds/TUDebugerUnreal4"
#endif


DEFINE_LOG_CATEGORY_STATIC(TapLog, Log, All);

bool TUDebuger::IsTest = false;
TMap<FString, FString> TUDebuger::ReplaceHosts;

void TUDebuger::AddReplacedHostPair(const FString& OriginHost, const FString& ReplacedHost) {
	ReplaceHosts.Add(OriginHost, ReplacedHost);
	TUDebuger::DisplayLog(FString::Printf(TEXT("原始地址：%s， 替换地址：%s"), *OriginHost, *ReplacedHost));
#if PLATFORM_IOS
	NSString *nsOriginHost = IOSHelper::convertString(OriginHost);
	NSString *nsReplacedHost = IOSHelper::convertString(ReplacedHost);
	[[TDSHostReplaceUtil shareInstance] addReplacedHostPair:nsOriginHost replacedHost:nsReplacedHost];
#elif PLATFORM_ANDROID

	JNIEnv *env = FAndroidApplication::GetJavaEnv();
	auto jSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_DEBUG_COMMON);
	if (jSDKUnreal4Class)
	{
		const char *strMethod = "addReplacedHostPair";
		auto jMethod = env->GetStaticMethodID(jSDKUnreal4Class, strMethod, "(Ljava/lang/String;Ljava/lang/String;)V");
		if (jMethod)
		{
			auto jOriginHost = env->NewStringUTF(TCHAR_TO_UTF8(*OriginHost));
			auto jReplacedHost = env->NewStringUTF(TCHAR_TO_UTF8(*ReplacedHost));
			env->CallStaticVoidMethod(jSDKUnreal4Class, jMethod, jOriginHost, jReplacedHost);
			env->DeleteLocalRef(jOriginHost);
			env->DeleteLocalRef(jReplacedHost);
		}
	}
	env->DeleteLocalRef(jSDKUnreal4Class);   
#endif
}

void TUDebuger::RemoveReplacedHostPair(const FString& OriginHost) {
	ReplaceHosts.Remove(OriginHost);
#if PLATFORM_IOS
	NSString *nsOriginHost = IOSHelper::convertString(OriginHost);
	[[TDSHostReplaceUtil shareInstance] clearReplacedHostPair:nsOriginHost];
#elif PLATFORM_ANDROID
	
	JNIEnv *env = FAndroidApplication::GetJavaEnv();
	auto jSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_DEBUG_COMMON);
	if (jSDKUnreal4Class)
	{
		const char *strMethod = "removeReplacedHostPair";
		auto jMethod = env->GetStaticMethodID(jSDKUnreal4Class, strMethod, "(Ljava/lang/String;)V");
		if (jMethod)
		{
			auto jOriginHost = env->NewStringUTF(TCHAR_TO_UTF8(*OriginHost));
			env->CallStaticVoidMethod(jSDKUnreal4Class, jMethod, jOriginHost);
			env->DeleteLocalRef(jOriginHost);
		}
	}
	env->DeleteLocalRef(jSDKUnreal4Class);   

#endif
}

void TUDebuger::ClearAllReplacedHostPairs() {
	ReplaceHosts.Empty();
#if PLATFORM_IOS
	[[TDSHostReplaceUtil shareInstance] clear];
#elif PLATFORM_ANDROID
	
	JNIEnv *env = FAndroidApplication::GetJavaEnv();
	auto jSDKUnreal4Class = FAndroidApplication::FindJavaClass(UNREAL4_CLASS_NAME_DEBUG_COMMON);
	if (jSDKUnreal4Class)
	{
		const char *strMethod = "clearAllReplacedHostPairs";
		auto jMethod = env->GetStaticMethodID(jSDKUnreal4Class, strMethod, "()V");
		if (jMethod)
		{
			env->CallStaticVoidMethod(jSDKUnreal4Class, jMethod);
		}
	}
	env->DeleteLocalRef(jSDKUnreal4Class);

#endif
}

FString TUDebuger::GetReplacedHost(const FString& OriginHost) {
	if (IsTest) {
		auto Value = ReplaceHosts.Find(OriginHost);
		if (Value != nullptr) {
			return *Value;
		}
	}
	return OriginHost;
}

FString TUDebuger::GetReplacedUrl(const FString& OriginUrl) {
	if (IsTest) {
		FString TempUrl = OriginUrl;
		for (auto ReplaceHost : TUDebuger::ReplaceHosts) {
			if (TempUrl.Contains(ReplaceHost.Key)) {
				TempUrl.ReplaceInline(*ReplaceHost.Key, *ReplaceHost.Value);
				break;
			}
		}
		return TempUrl;
	}
	else {
		return OriginUrl;
	}
}

void TUDebuger::DisplayLog(const FString& Info) {
	UE_LOG(TapLog, Display, TEXT("%s"), *Info);
}

void TUDebuger::WarningLog(const FString& Info) {
	UE_LOG(TapLog, Warning, TEXT("%s"), *Info);
}

void TUDebuger::ErrorLog(const FString& Info) {
	UE_LOG(TapLog, Error, TEXT("%s"), *Info);
}

void TUDebuger::DisplayShow(const FString& Info, float Duration) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Green, Info, true, FVector2D(2, 2));
	}
	DisplayLog(Info);
}

void TUDebuger::WarningShow(const FString& Info, float Duration) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, Info, true, FVector2D(2, 2));
	}
	WarningLog(Info);
}

void TUDebuger::ErrorShow(const FString& Info, float Duration) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Red, Info, true, FVector2D(2, 2));
	}
	ErrorLog(Info);
}
