#include "Android/LCAndroidPush.h"
#include "Android/LCJNI.h"
#include "Tools/LCHelper.h"
#include "Tools/LCDebuger.h"
#define LCMixPushUE "com/lc/LCMixPushUE"

void FLCAndroidPush::RegisterHuaWei() {
	LCJNI::JNI JNI;
	auto Class = JNI.FindClass(LCMixPushUE);
	JNI.CallStaticVoidMethod(Class, "registerHMSPush", "(Landroid/app/Activity;)V", *JNI.GetActivity());
}

void FLCAndroidPush::RegisterHonor() {
	LCJNI::JNI JNI;
	auto Class = JNI.FindClass(LCMixPushUE);
	JNI.CallStaticVoidMethod(Class, "registerHonorPush", "(Landroid/app/Activity;)V", *JNI.GetActivity());
}

void FLCAndroidPush::RegisterOPPO(const FString& AppKey, const FString& AppSecret) {
	LCJNI::JNI JNI;
	auto Class = JNI.FindClass(LCMixPushUE);
	JNI.CallStaticVoidMethod(Class, "registerOppoPush", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;)V", *JNI.GetActivity(), *JNI.ToJavaString(AppKey), *JNI.ToJavaString(AppSecret));
}

void FLCAndroidPush::RegisterVIVO() {
	LCJNI::JNI JNI;
	auto Class = JNI.FindClass(LCMixPushUE);
	JNI.CallStaticVoidMethod(Class, "registerVIVOPush", "(Landroid/app/Activity;)V", *JNI.GetActivity());
}

void FLCAndroidPush::RegisterMeiZu(const FString& AppId, const FString& AppKey) {
	LCJNI::JNI JNI;
	auto Class = JNI.FindClass(LCMixPushUE);
	JNI.CallStaticVoidMethod(Class, "registerFlymePush", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;)V", *JNI.GetActivity(), *JNI.ToJavaString(AppId), *JNI.ToJavaString(AppKey));
}

void FLCAndroidPush::RegisterXiaoMi(const FString& AppId, const FString& AppKey) {
	LCJNI::JNI JNI;
	auto Class = JNI.FindClass(LCMixPushUE);
	JNI.CallStaticVoidMethod(Class, "registerXiaomiPush", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;)V", *JNI.GetActivity(), *JNI.ToJavaString(AppId), *JNI.ToJavaString(AppKey));
}

FString FLCAndroidPush::GetDeviceName() {
	LCJNI::JNI JNI;
	auto Class = JNI.FindClass(LCMixPushUE);
	auto Name = JNI.CallStaticObjectMethod(Class, "getDeviceName", "()Ljava/lang/String;");
	return JNI.GetFString(Name);
}

#ifdef __cplusplus
extern "C" {
#endif
	

	__attribute__((visibility("default"))) void Java_com_lc_VIVOPushMessageReceiver_onVIVOMessage(JNIEnv *jenv, jclass thiz, jstring Message)
	{
		LCJNI::JNI JNI(jenv);
		FString Message_UE = JNI.GetFStringFromParam(Message);
		LCHelper::PerformOnGameThread([=]() {
			LCDebuger::DisplayLog("onVIVOMessage：" + Message_UE);
		});
	}


#ifdef __cplusplus
}
#endif
