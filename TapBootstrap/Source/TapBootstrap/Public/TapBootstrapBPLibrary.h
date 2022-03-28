// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TapLanguage.h"
#include "TapBootstrapBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class TAPBOOTSTRAP_API UTapBootstrapBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	UFUNCTION(BlueprintCallable, meta=(DisplayName = "TapBootstrap init"), Category = "TapBootstrap")
	static void Init(FString clientID, FString clientToken, FString serverUrl, bool bIsCN, bool bTapDBEnable,
	                 bool bAdvertiserIDCollectionEnabled, FString gameVersion, FString gameChannel);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Login by TapTap"), Category = "TapBootstrap")
	static void Login(TArray<FString> permissions);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Anonymously Login by TapTap"), Category = "TapBootstrap")
	static void AnonymouslyLogin();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Register UserStatus Listener"), Category = "TapBootstrap")
	static void RegisterUserStatusChangedListener();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Register Login Result Listener"), Category = "TapBootstrap")
	static void RegisterLoginResultListener();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get TapTap Userinfo"), Category = "TapBootstrap")
	static void GetUser();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get TapTap Detail Userinfo"), Category = "TapBootstrap")
	static void GetDetailUser();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get TapTap AccessToken"), Category = "TapBootstrap")
	static void GetAccessToken();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Open User Center"), Category = "TapBootstrap")
	static void OpenUserCenter();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TapTap Logout"), Category = "TapBootstrap")
	static void Logout();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TapTap Language config"), Category = "TapBootstrap")
	static void SetPreferLanguage(FTapLanguage language);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TapTap Test Qualification"), Category = "TapBootstrap")
	static void GetTestQualification();

	void OnBridgeCallback(const FString& result);
};
