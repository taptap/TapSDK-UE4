#pragma once
#include "Android/LCJNI.h"
#include "LCUser.h"

class LCJavaHelper {
public:
	// to map
	static LCJNI::Object ConvertUEMap(const LCJNI::JNI& LCJNI, TMap<FString, double> UE_Map);
	// to list
	static LCJNI::Object ConvertUEArray(const LCJNI::JNI& LCJNI, TArray<FString> UE_Array);

	static FString GetJsonFromMap(const LCJNI::JNI& LCJNI, const LCJNI::Object& object);
	static TSharedPtr<FLCObject> ConvertObject(const LCJNI::JNI& LCJNI, const LCJNI::Object& object);
	static TSharedPtr<FLCUser> ConvertUser(const LCJNI::JNI& LCJNI, const LCJNI::Object& user);

	static void ListForEach(const LCJNI::JNI& LCJNI, const LCJNI::Object& listObject, TFunction<void(int Index, const LCJNI::Object& Object)> Block);
};