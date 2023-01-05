#pragma once
#include "TapJNI.h"

class TapJavaHelper {
public:
	// to map
	static TapJNI::Object ConvertUEMap(const TapJNI::JNI& JNI, TMap<FString, double> UE_Map);
	// to list
	static TapJNI::Object ConvertUEArray(const TapJNI::JNI& JNI, TArray<FString> UE_Array);

	static FString GetJsonStrFromMap(const TapJNI::JNI& JNI, const TapJNI::Object& object);

	static TSharedPtr<FJsonObject> GetJsonObjectFromMap(const TapJNI::JNI& JNI, const TapJNI::Object& object);

	static TapJNI::Object GetMapFromJsonObject(const TapJNI::JNI& JNI, const TSharedPtr<FJsonObject>& JsonObject);

	static void ListForEach(const TapJNI::JNI& JNI, const TapJNI::Object& listObject, TFunction<void(int Index, const TapJNI::Object& Object)> Block);
};
