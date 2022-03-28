#include "IOSHelper.h"

NSDictionary<NSString *, NSString *> * IOSHelper::convertMap(const TMap<FString, FString>& map)
{
	
	NSMutableDictionary<NSString *, NSString *> *dic = [NSMutableDictionary dictionaryWithCapacity:map.Num()];
	for (TTuple<FString, FString> element : map)
	{
		dic[element.Key.GetNSString()] = element.Value.GetNSString();
	}
	return [dic copy];
}

NSArray<NSString *> * IOSHelper::convertArray(const TArray<FString>& array)
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

FString IOSHelper::convertString(NSString *string)
{
	return FString(UTF8_TO_TCHAR([string UTF8String]));
}

NSString * IOSHelper::convertString(FString& string) {
	return string.GetNSString();
}

FTapError IOSHelper::convertError(NSError *error)
{
	FTapError tapError;
	tapError.code = (int)error.code;
	tapError.error_description = IOSHelper::convertString(error.localizedDescription);
	return tapError;
}


FString IOSHelper::getJson(NSDictionary *dic) {
	if ([dic isKindOfClass:[NSDictionary class]]) {
		NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:nil];
		if (jsonData) {
			NSString *dataStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
			return IOSHelper::convertString(dataStr);
		}
	}
	NSLog(@"iOSHelper::getJson NSDictionary convert error");
	FString tempStr;
	return tempStr;
}

FString IOSHelper::getJson(NSArray *array) {
	if ([array isKindOfClass:[NSArray class]]) {
		NSData *jsonData = [NSJSONSerialization dataWithJSONObject:array options:NSJSONWritingPrettyPrinted error:nil];
		if (jsonData) {
			NSString *dataStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
			return IOSHelper::convertString(dataStr);
		}
	}
	NSLog(@"iOSHelper::getJson NSArray convert error");
	FString tempStr;
	return tempStr;
}



