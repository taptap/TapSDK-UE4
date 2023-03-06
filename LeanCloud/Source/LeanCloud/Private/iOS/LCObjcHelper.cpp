#include "iOS/LCObjcHelper.h"
#include "Tools/LCJsonHelper.h"

FString LCObjcHelper::Convert(NSString *String) {
	return FString(String);
}

NSString * LCObjcHelper::Convert(const FString& String) {
	return String.GetNSString();
}

TArray<FString> LCObjcHelper::Convert(NSArray<NSString *> *strings) {
	TArray<FString> UE_Strings;
	for (NSString *string in strings) {
		UE_Strings.Add(LCObjcHelper::Convert(string));
	}
	return UE_Strings;
}

NSArray<NSString *> * LCObjcHelper::Convert(const TArray<FString>& Strings) {
	NSMutableArray<NSString *> *strings = [NSMutableArray arrayWithCapacity:Strings.Num()];
	for (auto String : Strings) {
		[strings addObject:LCObjcHelper::Convert(String)];
	}
	return strings;
}

TSharedPtr<FJsonObject> LCObjcHelper::Convert(NSDictionary *dic) {
	FString JsonStr = LCObjcHelper::GetJson(dic);
	return LCJsonHelper::GetJsonObject(JsonStr);
}

FLCError LCObjcHelper::Convert(NSError *Error) {
	FLCError Result;
	Result.Code = Error.code;
	Result.Message = LCObjcHelper::Convert(Error.localizedDescription);
	return Result;
}

FString LCObjcHelper::GetJson(NSDictionary *dic) {
	if ([dic isKindOfClass:[NSDictionary class]]) {
		NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:nil];
		if (jsonData) {
			NSString *dataStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
			return LCObjcHelper::Convert(dataStr);
		}
	}
	NSLog(@"iOSHelper::getJson NSDictionary convert error");
	FString tempStr;
	return tempStr;
}

FString LCObjcHelper::GetJson(NSArray *array) {
	if ([array isKindOfClass:[NSArray class]]) {
		NSData *jsonData = [NSJSONSerialization dataWithJSONObject:array options:NSJSONWritingPrettyPrinted error:nil];
		if (jsonData) {
			NSString *dataStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
			return LCObjcHelper::Convert(dataStr);
		}
	}
	NSLog(@"iOSHelper::getJson NSArray convert error");
	FString tempStr;
	return tempStr;
}

TSharedPtr<FLCObject> LCObjcHelper::Convert(LCObject *object) {
	if (!object) {
		return nullptr;
	}
	FString ClassName = LCObjcHelper::Convert(object.className);
	TSharedPtr<FJsonObject> ServerData = LCObjcHelper::Convert([object dictionaryForObject]);
	TSharedPtr<FLCObject> Object_UE = MakeShareable(new FLCObject(ClassName, ServerData));
	return Object_UE;
}

TSharedPtr<FLCUser> LCObjcHelper::Convert(LCUser *user) {
	if (!user) {
		return nullptr;
	}
	TSharedPtr<FJsonObject> ServerData = LCObjcHelper::Convert([user dictionaryForObject]);
	TSharedPtr<FLCUser> User_UE = MakeShareable(new FLCUser(ServerData));
	return User_UE;
}
