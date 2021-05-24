// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapBootstrap.h"
#include "TapJson.h"
#include "TapCommon.h"
#include "TapBridge.h"
#include "TapBootstrapBPLibrary.h"

#if PLATFORM_IOS

#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"

#include "IOSAppDelegate.h"

#import <TapBootstrapSDK/TapBootstrapSDK.h>
#import <TapBootstrapSDK/TapBootstrap.h>

#pragma mark - AppDelegate
@interface IOSAppDelegate (TapBootstrap)

@end

@implementation IOSAppDelegate (TapBootstrap)

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
   return [TapBootstrap handleOpenURL:url];
}

- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options {
   return [TapBootstrap handleOpenURL:url];
}

@end

#endif

#define LOCTEXT_NAMESPACE "FTapBootstrapModule"

void FTapBootstrapModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if PLATFORM_ANDROID || PLATFORM_IOS

	FString clientID;
	FString clientSecret;
	bool bIsCN = true;
	bool bTapDBEnable = true;
	bool bAdvertiserIDCollectionEnabled = false;
	FString gameVersion;
	FString gameChannel;

	if(GConfig)
	{
		GConfig->GetString(TEXT("/Script/TapBootstrap.TapBootstrapSettings"), TEXT("ClientId"), clientID, GGameIni);
		GConfig->GetString(TEXT("/Script/TapBootstrap.TapBootstrapSettings"), TEXT("clientSecret"), clientSecret, GGameIni);
		GConfig->GetString(TEXT("/Script/TapBootstrap.TapBootstrapSettings"), TEXT("gameVersion"), gameVersion, GGameIni);
		GConfig->GetString(TEXT("/Script/TapBootstrap.TapBootstrapSettings"), TEXT("gameChannel"), gameChannel, GGameIni);
		GConfig->GetBool(TEXT("/Script/TapBootstrap.TapBootstrapSettings"), TEXT("bIsCN"), bIsCN, GGameIni);
		GConfig->GetBool(TEXT("/Script/TapBootstrap.TapBootstrapSettings"), TEXT("bTapDBEnable"), bTapDBEnable, GGameIni);
		GConfig->GetBool(TEXT("/Script/TapBootstrap.TapBootstrapSettings"), TEXT("bAdvertiserIDCollectionEnabled"), bAdvertiserIDCollectionEnabled, GGameIni);

		UE_LOG(LogTemp,Log,TEXT("TapBootstrap ClientID from UE Editor %s"),*clientID);
		UE_LOG(LogTemp,Log,TEXT("TapBootstrap clientSecret from UE Editor %s"),*clientSecret);
		UE_LOG(LogTemp,Log,TEXT("TapBootstrap gameVersion from UE Editor %s"),*gameVersion);
		UE_LOG(LogTemp,Log,TEXT("TapBootstrap gameChannel from UE Editor %s"),*gameChannel);

		FString configJSON;
		TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> configWriter = TJsonWriterFactory< TCHAR, TCondensedJsonPrintPolicy<TCHAR> >::Create(&configJSON);
		configWriter->WriteObjectStart();
		configWriter->WriteValue("clientID",clientID);
		configWriter->WriteValue("clientSecret",clientSecret);
		configWriter->WriteValue("isCN",bIsCN);
		if(bTapDBEnable){
			configWriter->WriteObjectStart("dbConfig");
			configWriter->WriteValue("enable",bTapDBEnable);
#if PLATFORM_IOS
			configWriter->WriteValue("advertiserIDCollectionEnabled",bAdvertiserIDCollectionEnabled);
#endif
			configWriter->WriteValue("gameVersion",gameVersion);
			configWriter->WriteValue("channel",gameChannel);
			configWriter->WriteObjectEnd();
		}
		configWriter->WriteObjectEnd();
		configWriter->Close();
		
		FString bootstrapConfigJSON;
		TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> bootstrapWriter = TJsonWriterFactory< TCHAR, TCondensedJsonPrintPolicy<TCHAR> >::Create(&bootstrapConfigJSON);
		bootstrapWriter->WriteObjectStart();
		bootstrapWriter->WriteValue("initWithConfig",configJSON);
		bootstrapWriter->WriteObjectEnd();
		bootstrapWriter->Close();
		Init(bootstrapConfigJSON);

		UTapBootstrapBPLibrary::RegisterLoginResultListener();
		UTapBootstrapBPLibrary::RegisterUserStatusChangedListener();

	}
#endif
}

void FTapBootstrapModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

void FTapBootstrapModule::Init(FString tapConfig)
{
UE_LOG(LogTemp,Display,TEXT("TapBoostrap Init Config:%s"),*tapConfig);
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString command = TapJson::ConstructorCommand(TEXT("TapBootstrapService"),TEXT("initWithConfig"),tapConfig,false,TEXT(""),false);
	GetBridge()->CallHandler(command);
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTapBootstrapModule, TapBootstrap)


FTapBootstrapModule::FLoginSuccess FTapBootstrapModule::OnLoginSuccess;
FTapBootstrapModule::FLoginCancel FTapBootstrapModule::OnLoginCancel;
FTapBootstrapModule::FLoginError FTapBootstrapModule::OnLoginError;
FTapBootstrapModule::FLogout FTapBootstrapModule::OnLogout;
FTapBootstrapModule::FGetAccessToken FTapBootstrapModule::OnGetAccessToken;
FTapBootstrapModule::FGetUserSuccess FTapBootstrapModule::OnGetUserSuccess;
FTapBootstrapModule::FGetUserError FTapBootstrapModule::OnGetUserError;
FTapBootstrapModule::FGetUserDetailSuccess FTapBootstrapModule::OnGetUserDetailSuccess;
FTapBootstrapModule::FGetUserDetailError FTapBootstrapModule::OnGetUserDetailError;
FTapBootstrapModule::FGetTestQualification FTapBootstrapModule::OnGetTestQualification;
FTapBootstrapModule::FGetTestQualificationError FTapBootstrapModule::OnGetTestQualificationError;