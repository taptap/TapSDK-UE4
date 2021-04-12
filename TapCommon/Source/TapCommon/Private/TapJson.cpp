#include "TapJson.h"

#if PLATFORM_ANDROID || PLATFORM_IOS

TSharedPtr<FJsonObject> TapJson::Parse(FString json)
{
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(json);
    TSharedPtr<FJsonObject> Result;
    bool parseSuccess = FJsonSerializer::Deserialize(Reader, Result);
    return Result;
}

FString TapJson::ToJSON(TSharedPtr<FJsonObject> jsonObject)
{
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(jsonObject.ToSharedRef(), Writer);
    return OutputString;
}

FString TapJson::ConstructorCommand(FString serviceName, FString method, FString args, bool callback, FString callbackId,bool onceTime)
{
    TSharedPtr<FJsonObject> commandJson = MakeShareable(new FJsonObject);
    commandJson->SetStringField("service", serviceName);
    commandJson->SetStringField("method", method);
    commandJson->SetStringField("args", args);
    commandJson->SetBoolField("callback", callback);
    commandJson->SetStringField("callbackId", callbackId);
    commandJson->SetBoolField("onceTime",onceTime);
    return TapJson::ToJSON(commandJson);
}

#endif
