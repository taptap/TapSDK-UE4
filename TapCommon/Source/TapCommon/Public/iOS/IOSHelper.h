#pragma once

#include "TUError.h"
#import <Foundation/Foundation.h>

class IOSHelper
{
public:
	static NSDictionary<NSString *, NSString *> * Convert(const TMap<FString, FString>& map);

	static NSArray<NSString *> * Convert(const TArray<FString>& array);

	static FString Convert(NSString *string);

	static NSString * Convert(const FString& string);

	static FTUError Convert(NSError *error);

	static FString GetJson(NSDictionary *dic);
    
	static FString GetJson(NSArray *array);

	static TArray<FString> Convert(NSArray<NSString *> *strings);

	static TSharedPtr<FJsonObject> Convert(NSDictionary *dic);

	static NSDictionary * Convert(TSharedPtr<FJsonObject> JsonObject);
	
};
