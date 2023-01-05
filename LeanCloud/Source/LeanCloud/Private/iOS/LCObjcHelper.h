#pragma once
#import <Foundation/Foundation.h>
#include "LCError.h"
#include "LCUser.h"
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
#import <LeanCloudObjc/LCUser.h>


class LCObjcHelper {
public:
	static FString Convert(NSString *String);
	static NSString * Convert(const FString& String);

	static TArray<FString> Convert(NSArray<NSString *> *strings);
	static NSArray<NSString *> * Convert(const TArray<FString>& Strings);

	static TSharedPtr<FJsonObject> Convert(NSDictionary *dic);
	static FLCError Convert(NSError *Error);

	static FString GetJson(NSDictionary *dic);
    
	static FString GetJson(NSArray *array);

	static TSharedPtr<FLCObject> Convert(LCObject *object);

	static TSharedPtr<FLCUser> Convert(LCUser *user);
};
