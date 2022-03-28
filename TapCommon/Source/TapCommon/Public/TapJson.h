#pragma once

#include <string>
#include "Engine.h"
#include "Json.h"
#include "Serialization/JsonWriter.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

class TAPCOMMON_API TapJson
{

public:
    static TSharedPtr<FJsonObject> Parse(FString json);

public:
    static FString ToJSON(TSharedPtr<FJsonObject> jsonObject);

public:
    static FString ToJSON(TMap<FString, FString> TMap);

public:
    static FString ConstructorCommand(FString serviceName, FString method, FString args, bool callback, FString callbackId,bool onceTime);

};

