#include "LeanCloud_Android.h"
#include "Engine.h"
#include "Android/LCJNI.h"
#include "Android/AndroidJavaEnv.h"
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#include "Runtime/Core/Public/Android/AndroidJava.h"

#define LeanCloudUE "com/lc/LeanCloudUE"

FLeanCloud_Android::~FLeanCloud_Android() {
}

void FLeanCloud_Android::Init(const FLCConfig& InConfig) {
	LCJNI::JNI LCJNI;
	auto Class = LCJNI.FindClass(LeanCloudUE);

	auto jActivity = FAndroidApplication::GetGameActivityThis();
	auto ClientID = LCJNI.ToJavaString(InConfig.ClientID);
	auto ClientToken = LCJNI.ToJavaString(InConfig.ClientToken);
	auto ServerURL = LCJNI.ToJavaString(InConfig.ServerURL);

	LCJNI.CallStaticVoidMethod(Class, "initialize", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", jActivity, *ClientID, *ClientToken, *ServerURL);

}
