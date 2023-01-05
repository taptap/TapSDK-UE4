#include "IOSHelper.h"

#include "TUDebuger.h"
#include "TUJsonHelper.h"

NSDictionary<NSString *, NSString *> * IOSHelper::Convert(const TMap<FString, FString>& map)
{
	
	NSMutableDictionary<NSString *, NSString *> *dic = [NSMutableDictionary dictionaryWithCapacity:map.Num()];
	for (TTuple<FString, FString> element : map)
	{
		dic[element.Key.GetNSString()] = element.Value.GetNSString();
	}
	return [dic copy];
}

NSArray<NSString *> * IOSHelper::Convert(const TArray<FString>& array)
{
	NSMutableArray<NSString *> *tempArr = [NSMutableArray arrayWithCapacity:array.Num()];
	for (FString element : array)
	{
		NSString *string = element.GetNSString();
		if (string)
		{
			[tempArr addObject:string];
		}
	}
	return [tempArr copy];
}

FString IOSHelper::Convert(NSString* string) {
	return FString(string);
}

NSString * IOSHelper::Convert(const FString& string) {
	return string.GetNSString();
}

FTUError IOSHelper::Convert(NSError *error)
{
	FTUError tapError;
	tapError.code = (int)error.code;
	tapError.error_description = IOSHelper::Convert(error.localizedDescription);
	return tapError;
}


FString IOSHelper::GetJson(NSDictionary *dic) {
	if ([dic isKindOfClass:[NSDictionary class]]) {
		NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:nil];
		if (jsonData) {
			NSString *dataStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
			return IOSHelper::Convert(dataStr);
		}
	}
	NSLog(@"iOSHelper::getJson NSDictionary convert error");
	FString tempStr;
	return tempStr;
}

FString IOSHelper::GetJson(NSArray *array) {
	if ([array isKindOfClass:[NSArray class]]) {
		NSData *jsonData = [NSJSONSerialization dataWithJSONObject:array options:NSJSONWritingPrettyPrinted error:nil];
		if (jsonData) {
			NSString *dataStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
			return IOSHelper::Convert(dataStr);
		}
	}
	NSLog(@"iOSHelper::getJson NSArray convert error");
	FString tempStr;
	return tempStr;
}

TArray<FString> IOSHelper::Convert(NSArray<NSString *> *strings) {
	TArray<FString> UE_Strings;
	for (NSString *string in strings) {
		UE_Strings.Add(IOSHelper::Convert(string));
	}
	return UE_Strings;
}

TSharedPtr<FJsonObject> IOSHelper::Convert(NSDictionary *dic) {
	FString JsonStr = IOSHelper::GetJson(dic);
	return TUJsonHelper::GetJsonObject(JsonStr);
}

NSDictionary * IOSHelper::Convert(TSharedPtr<FJsonObject> JsonObject) {
	if (!JsonObject.IsValid()) {
		return nil;
	}
	FString JsonStr = TUJsonHelper::GetJsonString(JsonObject);
	NSString *jsonString = IOSHelper::Convert(JsonStr);
	NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
	NSError *err;
	NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData
														options:NSJSONReadingMutableContainers
														  error:&err];
	return dic;
}



