#include "TapJavaHelper.h"

#include "TUJsonHelper.h"
#define TapCommonUE "com/tds/TapCommonUE"

TapJNI::Object TapJavaHelper::ConvertUEMap(const TapJNI::JNI& JNI, TMap<FString, double> UE_Map) {
	auto mapClass = JNI.FindClass("java/util/HashMap");
	auto hashMap = JNI.NewObject(mapClass, "()V");
	for (auto Map : UE_Map) {
		auto keyJava = JNI.ToJavaString(Map.Key);
		JNI.CallObjectMethod(hashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;", *keyJava, *JNI.GetJavaObject(Map.Value));
	}
	return hashMap;
}

TapJNI::Object TapJavaHelper::ConvertUEArray(const TapJNI::JNI& JNI, TArray<FString> UE_Array) {
	auto ArrayListClass = JNI.FindClass("java/util/ArrayList");
	auto list = JNI.NewObject(ArrayListClass, "()V");
	for (auto Array : UE_Array) {
		auto string = JNI.ToJavaString(Array);
		JNI.CallBooleanMethod(list, "add", "(Ljava/lang/Object;)Z", *string);
	}
	return list;
}


FString TapJavaHelper::GetJsonStrFromMap(const TapJNI::JNI& JNI, const TapJNI::Object& object) {
	auto Class = JNI.FindClass(TapCommonUE);
	auto string = JNI.CallStaticObjectMethod(Class, "mapToJson", "(Ljava/util/Map;)Ljava/lang/String;", *object);
	return JNI.GetFString(string);
}

TSharedPtr<FJsonObject> TapJavaHelper::GetJsonObjectFromMap(const TapJNI::JNI& JNI, const TapJNI::Object& object) {
	FString jsonstr = TapJavaHelper::GetJsonStrFromMap(JNI, object);
	return TUJsonHelper::GetJsonObject(jsonstr);
}

TapJNI::Object TapJavaHelper::GetMapFromJsonObject(const TapJNI::JNI& JNI, const TSharedPtr<FJsonObject>& JsonObject) {
	FString jsonstr = TUJsonHelper::GetJsonString(JsonObject);
	auto Class = JNI.FindClass(TapCommonUE);
	return JNI.CallStaticObjectMethod(Class, "jsonToMap", "(Ljava/lang/String;)Ljava/util/Map;", *JNI.ToJavaString(jsonstr));
}


void TapJavaHelper::ListForEach(const TapJNI::JNI& JNI, const TapJNI::Object& listObject, TFunction<void(int Index, const TapJNI::Object& Object)> Block) {
	if (!listObject) {
		return;
	}
	int list_size = JNI.CallIntMethod(listObject, "size", "()I");
	for (int i = 0; i < list_size; i++) {
		auto obj = JNI.CallObjectMethod(listObject, "get", "(I)Ljava/lang/Object;", i);
		Block(i, obj);
	}
}
