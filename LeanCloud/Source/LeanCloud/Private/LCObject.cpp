#include "LCObject.h"
#include "Tools/LCJsonHelper.h"

FString FLCObject::KeyCreatedAt = "createdAt";
FString FLCObject::KeyUpdateAt = "updatedAt";
FString FLCObject::KeyObjectID = "objectId";

FLCObject::FLCObject(const FString& InClassName, TSharedPtr<FJsonObject> InServerData) {
	ClassName = InClassName;
	if (!InServerData.IsValid()) {
		ServerData = MakeShareable(new FJsonObject);
		return;
	}
	ServerData = MakeShareable(new FJsonObject(*InServerData));
}

FLCObject::~FLCObject() {
}

FLCObject FLCObject::CreateWithoutData(const FString& InClassName, const FString& ObjectId) {
	FLCObject Object = FLCObject(InClassName);
	Object.SetObjectId(ObjectId);
	return Object;
}

void FLCObject::SetObjectId(const FString& ObjectId) {
	ServerData->SetStringField(KeyObjectID, ObjectId);
}

FString FLCObject::GetClassName() const {
	return ClassName;
}

FDateTime FLCObject::GetCreatedAt() const {
	FString DateStr = LCJsonHelper::GetStringField(ServerData, KeyCreatedAt);
	FDateTime TempTime;
	if (DateStr.IsEmpty()) {
		return TempTime;
	}
	ParseTime(DateStr, TempTime);
	return TempTime;
}

FDateTime FLCObject::GetUpdatedAt() const {
	FString DateStr = LCJsonHelper::GetStringField(ServerData, KeyUpdateAt);
	FDateTime TempTime;
	if (DateStr.IsEmpty()) {
		return TempTime;
	}
	ParseTime(DateStr, TempTime);
	return TempTime;
}

FString FLCObject::GetObjectId() const {
	return LCJsonHelper::GetStringField(ServerData, KeyObjectID);
}

const TSharedRef<FJsonObject> FLCObject::GetServerData() const {
	return ServerData.ToSharedRef();
}

bool FLCObject::ParseTime(const FString& InTimeString, FDateTime& OutTime) const
{
	//Format: YYYY-MM-DDTHH:MM:SS.MMMZ
	const int32 Year = FCString::Atoi(*FString(InTimeString.GetCharArray().GetData(), 4));
	const int32 Month = FCString::Atoi(*FString(InTimeString.GetCharArray().GetData() + 5, 2));
	const int32 Day = FCString::Atoi(*FString(InTimeString.GetCharArray().GetData() + 8, 2));
	const int32 Hour = FCString::Atoi(*FString(InTimeString.GetCharArray().GetData() + 11, 2));
	const int32 Minute = FCString::Atoi(*FString(InTimeString.GetCharArray().GetData() + 14, 2));
	const int32 Second = FCString::Atoi(*FString(InTimeString.GetCharArray().GetData() + 17, 2));
	const int32 Millisecond = FCString::Atoi(*FString(InTimeString.GetCharArray().GetData() + 20, 3));
	if (FDateTime::Validate(Year, Month, Day, Hour, Minute, Second, Millisecond))
	{
		OutTime = FDateTime(Year, Month, Day, Hour, Minute, Second, Millisecond);
		return true;
	}
	return false;
}