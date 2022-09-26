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
	static FString GetJsonString(const TSharedPtr<UStructType>& value)
	{
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(*value.Get());
		return GetJsonString(JsonObject);
	}

	template <typename UStructType>
	static FString GetJsonString(const UStructType& value)
	{
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(value);
		return GetJsonString(JsonObject);
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
};
