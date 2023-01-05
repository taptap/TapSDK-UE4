#pragma once
#include "JsonObjectConverter.h"

inline TSharedPtr<FJsonObject>& operator+=(TSharedPtr<FJsonObject> & Object, const TSharedPtr<FJsonObject> & AddObject) {
	if (Object.IsValid() && AddObject.IsValid())
	{
		Object->Values.Append(AddObject->Values);
	}
	return Object;
}

class TAPCOMMON_API TUJsonHelper
{
public:

	static FString GetStringField(TSharedPtr<FJsonObject> JsonObject, const FString& Field) {
		FString Temp = "";
		if (JsonObject.IsValid()) {
			JsonObject->TryGetStringField(Field, Temp);
		}
		return Temp;
	}
	
	static bool GetBoolField(TSharedPtr<FJsonObject> JsonObject, const FString& Field) {
		bool Result = false;
		if (JsonObject.IsValid()) {
			JsonObject->TryGetBoolField(Field, Result);
		}
		return Result;
	}
	
	static FString GetJsonString(const TSharedPtr<FJsonObject>& JsonObject)
	{
		FString OutputString;
		if (!JsonObject.IsValid())
		{
			return OutputString;
		}
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
		return OutputString;
	}

	template <typename UStructType>
	static FString GetJsonString(const TSharedPtr<UStructType>& Value)
	{
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(*Value.Get());
		return GetJsonString(JsonObject);
	}

	template <typename UStructType>
	static FString GetJsonString(const UStructType& Value)
	{
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Value);
		return GetJsonString(JsonObject);
	}

	template <typename UStructType>
	static FString GetJsonString(const TArray<UStructType>& Values)
	{
		FString JsonStr;
		TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonStr);
		JsonWriter->WriteArrayStart();
		for (auto Value : Values) {
			auto ValueJsonStr = GetJsonString(Value);
			if (!ValueJsonStr.IsEmpty()) {
				JsonWriter->WriteRawJSONValue(ValueJsonStr);
			}
		}
		JsonWriter->WriteArrayEnd();
		JsonWriter->Close();

		return JsonStr;
	}

	
	static TSharedPtr<FJsonObject> GetJsonObject(const FString& JsonString)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			return JsonObject;
		} else
		{
			return nullptr;
		}
		
	}

	template <typename UStructType>
	static TSharedPtr<FJsonObject> GetJsonObject(const TSharedPtr<UStructType>& value)
	{
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(*value.Get());
		return JsonObject;
	}

	template <typename UStructType>
	static TSharedPtr<FJsonObject> GetJsonObject(const UStructType& value)
	{
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(value);
		return JsonObject;
	}

	template <typename UStructType>
	static TSharedPtr<UStructType> GetUStruct(const TSharedPtr<FJsonObject>& JsonObject)
	{
		if (!JsonObject.IsValid())
		{
			return nullptr;
		}
		TSharedPtr<UStructType> value = MakeShareable(new UStructType);
		if (FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), value.Get()))
		{
			return value;
		} else
		{
			return nullptr;
		}
	}

	template <typename UStructType>
	static TSharedPtr<UStructType> GetUStruct(const FString& JsonString)
	{
		return GetUStruct<UStructType>(GetJsonObject(JsonString));
	}

	template <typename UStructType>
	static TArray<UStructType> GetUStructArray(const FString& JsonString)
	{
		TArray<UStructType> Results;
		FJsonObjectConverter::JsonArrayStringToUStruct(JsonString, &Results, 0, 0);
		return Results;
	}
};


