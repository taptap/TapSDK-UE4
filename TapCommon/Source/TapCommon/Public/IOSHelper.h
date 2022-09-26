#pragma once


// #if PLATFORM_IOS
#include "TUError.h"
#import <Foundation/Foundation.h>

class IOSHelper
{
public:
	static NSDictionary<NSString *, NSString *> * convertMap(const TMap<FString, FString>& map);

	static NSArray<NSString *> * convertArray(const TArray<FString>& array);

	static FString convertString(NSString *string);

	static NSString * convertString(const FString& string);

	static FTUError convertError(NSError *error);

	static FString getJson(NSDictionary *dic);
    
	static FString getJson(NSArray *array);
};

// #endif

// class IOSHelper
// {
// public:
// 	
// };