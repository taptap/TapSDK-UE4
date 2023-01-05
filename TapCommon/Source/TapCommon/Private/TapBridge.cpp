#include "TapBridge.h"

#if PLATFORM_ANDROID

#include "Android/AndroidBridge.h"

AndroidBridge * GetAndroidBridge(){
    static AndroidBridge * bridge = nullptr;
    if(bridge == nullptr){
        bridge = new AndroidBridge();
    }
    return bridge;
}

IBridge * GetBridge(){
    return GetAndroidBridge();
}

#elif PLATFORM_IOS

#include "iOS/IOSBridge.h"

IOSBridge * GetIOSBridge(){
    static IOSBridge * bridge = nullptr;
    if(bridge == nullptr){
        bridge = new IOSBridge();
    }
    return bridge;
}

IBridge * GetBridge(){
    return GetIOSBridge();
}

#else

IBridge * GetBridge(){
    return nullptr;
}

#endif