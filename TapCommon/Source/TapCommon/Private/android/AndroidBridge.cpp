#include "AndroidBridge.h"
#include "TapCommon.h"
#include "Engine.h"
#include "TUMobileBridge.h"

#define TDS_BRIDGE_CLASS "com/tds/common/bridge/Bridge"

AndroidBridge::AndroidBridge()
{
    Init();
}

AndroidBridge::~AndroidBridge()
{
    Init();
}

jobject AndroidBridge::GetJBridge()
{
    jobject jBridge = nullptr;
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jUnrealClass = FAndroidApplication::FindJavaClass(TDS_BRIDGE_CLASS);
    if (jUnrealClass)
    {
        const char *strMethod = "getInstance";
        auto jMethod = env->GetStaticMethodID(jUnrealClass, strMethod, "()Lcom/tds/common/bridge/Bridge;");
        jBridge = env->CallStaticObjectMethod(jUnrealClass, jMethod);
    }
    return jBridge;
}

void AndroidBridge::Init()
{
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jUnrealClass = FAndroidApplication::FindJavaClass(TDS_BRIDGE_CLASS);
    if (jUnrealClass)
    {
        const char *strMethod = "init";
        auto jMethod = env->GetMethodID(jUnrealClass, strMethod, "(Landroid/app/Activity;)V");
        if (jMethod)
        {
            auto jActivity = FAndroidApplication::GetGameActivityThis();
            env->CallVoidMethod(GetJBridge(), jMethod, jActivity);
        }
    }
}

void AndroidBridge::Register(FString serviceClz, FString serviceImpl)
{
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jUnrealClass = FAndroidApplication::FindJavaClass(TDS_BRIDGE_CLASS);
    if (jUnrealClass)
    {
        const char *strMethod = "register";
        const char *strServiceClz = TCHAR_TO_UTF8(*serviceClz);
        const char *strServiceImpl = TCHAR_TO_UTF8(*serviceImpl);

        auto jServiceClz = FAndroidApplication::FindJavaClass(strServiceClz);
        auto jServiceImplClz = FAndroidApplication::FindJavaClass(strServiceImpl);
        
        auto jServiceImpl = env->NewObject(jServiceImplClz,  env->GetMethodID(jServiceImplClz, "<init>", "()V"));
        auto jRegisterMethod = env->GetMethodID(jUnrealClass, strMethod, "(Ljava/lang/Class;Lcom/tds/common/bridge/IBridgeService;)V");
        if (jRegisterMethod)
        {
            env->CallVoidMethod(GetJBridge(), jRegisterMethod, jServiceClz, jServiceImpl);
        }
        env->DeleteLocalRef(jServiceImpl);
    }
}

void AndroidBridge::CallHandler(FString command)
{
    JNIEnv *env = FAndroidApplication::GetJavaEnv();
    auto jUnrealClass = FAndroidApplication::FindJavaClass(TDS_BRIDGE_CLASS);
    if (jUnrealClass)
    {
        const char *strMethod = "callHandler";
        auto jMethod = env->GetMethodID(jUnrealClass, strMethod, "(Ljava/lang/String;)V");
        if (jMethod)
        {
            auto jCommand = env->NewStringUTF(TCHAR_TO_UTF8(*command));
            env->CallVoidMethod(GetJBridge(), jMethod, jCommand);
            env->DeleteLocalRef(jCommand);
        }
    }
}

#ifdef __cplusplus
extern "C"
{
#endif

    __attribute__((visibility("default"))) void Java_com_tds_common_bridge_Bridge_nativeOnResult(JNIEnv *jenv, jclass thiz, jstring msg)
    {
        const char *cMsg = jenv->GetStringUTFChars(msg, 0);
        FString fMsg = UTF8_TO_TCHAR(cMsg);
        FTapCommonModule::OnBridgeCallback.Broadcast(fMsg);
        TUMobileBridge::DoCallBack(fMsg);
        jenv->ReleaseStringUTFChars(msg, cMsg);
    }

#ifdef __cplusplus
}
#endif