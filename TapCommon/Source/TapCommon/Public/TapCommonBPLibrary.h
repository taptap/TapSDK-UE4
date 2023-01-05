// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TUError.h"
#include "TapResult.h"
#include "TapCommonBPLibrary.generated.h"

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
class TAPCOMMON_API UTapCommonBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "TapCommon")
	static void Register(FString serviceClz, FString serviceImpl);

	UFUNCTION(BlueprintCallable, Category = "TapCommon")
	static void CallHandler(FString command);

	// UFUNCTION(BlueprintCallable, Category="TapCommon")
	// static void GetRegionCode();
	//
	// UFUNCTION(BlueprintCallable,Category = "TapCommon")
	// static void IsTapTapInstalled();
	//
	// UFUNCTION(BlueprintCallable,Category = "TapCommon")
	// static void IsTapGlobalInstalled();
	//
	// UFUNCTION(BlueprintCallable,Category = "TapCommon")
	// static void UpdateGameInTapTap(FString appId);
	//
	// UFUNCTION(BlueprintCallable,Category = "TapCommon")
	// static void UpdateGameInTapGlobal(FString appId);
	//
	// UFUNCTION(BlueprintCallable, Category = "TapCommon")
	// static void OpenReviewInTapTap(FString appId);
	//
	// UFUNCTION(BlueprintCallable,Category = "TapCommon")
	// static void OpenReviewInTapGlobal(FString appId);

	UFUNCTION(BlueprintCallable,Category = "TapCommon")
	static TMap<FString, FString> RemoveEmptyKey(TMap<FString, FString> stringMap);

	UFUNCTION(BlueprintCallable,Category = "TapCommon")
	static TArray<FString> RemoveEmptyString(TArray<FString> stringArray);

	static bool CheckResult(const FTapResult result);

	void OnBridgeCallback(const FString &result);
	
};


USTRUCT()
struct FTapCommonRegionCode
{
    GENERATED_BODY()
	
	UPROPERTY()
	bool isMainland = false;

};