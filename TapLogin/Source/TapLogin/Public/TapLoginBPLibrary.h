// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TapLoginBPLibrary.generated.h"

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
class UTapLoginBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "init TapTap Login"),
		Category = "TapLogin")
	static void Init(FString clientID);

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "init TapTap Login with params"),
		Category = "TapLogin")
	static void InitWithParams(FString clientID, bool bIsCN, bool bRoundCorner);

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Fetch profile"), Category = "TapLogin")
	static void FetchProfile();

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Get profile"), Category = "TapLogin")
	static void GetProfile();

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Get AccessToken"), Category = "TapLogin")
	static void GetAccessToken();

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Login"), Category = "TapLogin")
	static void Login(TArray<FString> permissions);

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Logout"), Category = "TapLogin")
	static void Logout();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Test Qualification"), Category = "TapLogin")
	static void GetTestQualification();

	static void RegisterLoginCallback();

	static void UnRegisterLoginCallback();
	
	void OnBridgeCallback(const FString& result);


	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Query Mutual List"), Category = "Login TapFriend")
	static void QueryMutualList(FString cursor, int size);
};
