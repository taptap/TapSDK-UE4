#include "Tools/LCJsonHelper.h"

FString LCJsonHelper::GetStringField(TSharedPtr<FJsonObject> JsonObject, const FString& Field) {
	FString Temp = "";
	if (JsonObject.IsValid()) {
		JsonObject->TryGetStringField(Field, Temp);
	}
	return Temp;
}

bool LCJsonHelper::GetBoolField(TSharedPtr<FJsonObject> JsonObject, const FString& Field) {
	bool Result = false;
	if (JsonObject.IsValid()) {
		JsonObject->TryGetBoolField(Field, Result);
	}
	return Result;
}

