#include "AAUChinaIOSImpl.h"

#include "AntiAddictionUE.h"
#include "IOSHelper.h"
#include "TUDebuger.h"
#include "Server/AAUHelper.h"
//#pragma clang diagnostic push 
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
// #pragma clang diagnostic ignored "-Werror"

#import <AntiAddictionUI/AntiAddictionUI.h>

@interface AAUChinaIOSImplDelegate: NSObject <AntiAddictionDelegate>

@end

@implementation AAUChinaIOSImplDelegate

- (void)antiAddictionCallbackWithCode:(AntiAddictionResultHandlerCode)code extra:(NSString * _Nullable)extra {
	FString Msg = IOSHelper::convertString(extra);
	AsyncTask(ENamedThreads::GameThread, [=]() {
		AntiAddictionUE::OnCallBack.ExecuteIfBound((AntiAddictionUE::ResultHandlerCode)code, Msg);
	});
}

@end

static AAUChinaIOSImplDelegate* IOSImplDelegate = nil;

AAUChinaIOSImpl::AAUChinaIOSImpl() {
	IOSImplDelegate = [[AAUChinaIOSImplDelegate alloc] init];
}

AAUChinaIOSImpl::~AAUChinaIOSImpl() {
}

void AAUChinaIOSImpl::InitImpl(const FAAUConfig& _Config) {
	AntiAddictionConfig *config = [[AntiAddictionConfig alloc] init];
	config.clientID = IOSHelper::convertString(_Config.ClientID);
	config.useTapLogin = _Config.UseTapLogin;
	config.showSwitchAccount = _Config.ShowSwitchAccount;

	dispatch_async(dispatch_get_main_queue(), ^{
		[AntiAddiction initWithConfig:config delegate:IOSImplDelegate];
	});
}

void AAUChinaIOSImpl::Startup(const FString& UserID) {
	NSString *userID = IOSHelper::convertString(UserID);
	dispatch_async(dispatch_get_main_queue(), ^{
		[AntiAddiction startupWithUserID:userID];
	});
}

void AAUChinaIOSImpl::Exit() {
	dispatch_async(dispatch_get_main_queue(), ^{
		[AntiAddiction exit];
	});
}

void AAUChinaIOSImpl::EnterGame() {
	dispatch_async(dispatch_get_main_queue(), ^{
		[AntiAddiction enterGame];
	});
}

void AAUChinaIOSImpl::LeaveGame() {
	dispatch_async(dispatch_get_main_queue(), ^{
		[AntiAddiction leaveGame];
	});
}

EAAUAgeLimit AAUChinaIOSImpl::GetAgeRange() {
	return AAUHelper::MakeAgeLimit([AntiAddiction getAgeRange]);
}

int AAUChinaIOSImpl::GetRemainingTime() {
	return [AntiAddiction getRemainingTime];
}

void AAUChinaIOSImpl::CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	dispatch_async(dispatch_get_main_queue(), ^{
		[AntiAddiction checkPayLimit:Amount resultBlock:^(bool status) {
			AsyncTask(ENamedThreads::GameThread, [=]() {
				if (CallBack) {
					CallBack(status);
				}
			});
		} failureHandler:^(NSString *Msg) {
			FString Message = IOSHelper::convertString(Msg);
			AsyncTask(ENamedThreads::GameThread, [=]() {
				if (FailureHandler) {
					FailureHandler(Message);
				}
			});
		}];
	});
}

void AAUChinaIOSImpl::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	dispatch_async(dispatch_get_main_queue(), ^{
		[AntiAddiction submitPayResult:Amount callBack:^(bool status) {
			AsyncTask(ENamedThreads::GameThread, [=]() {
				if (CallBack) {
					CallBack(status);
				}
			});
		} failureHandler:^(NSString *Msg) {
			FString Message = IOSHelper::convertString(Msg);
			AsyncTask(ENamedThreads::GameThread, [=]() {
				if (FailureHandler) {
					FailureHandler(Message);
				}
			});
		}];
	});
}

FString AAUChinaIOSImpl::CurrentToken() {
	return IOSHelper::convertString([AntiAddiction currentToken]);
}
