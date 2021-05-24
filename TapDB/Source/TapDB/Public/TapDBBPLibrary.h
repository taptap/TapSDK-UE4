// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreMinimal.h"
#include "TapDBDynamicProperties.h"
#include "TapDBBPLibrary.generated.h"

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
class UTapDBBPLibrary : public UBlueprintFunctionLibrary
{

public:
	
	GENERATED_UCLASS_BODY()	

	static TapDBDynamicProperties* dbDynamicProperties;
	
	static void RegisterDynamicProperties(TapDBDynamicProperties* properties);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void Init(FString clientID,FString channel,FString gameVersion,bool isCN);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void SetUser(FString userId);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void SetUserWithLoginType(FString userId,FString loginType);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void SetName(FString name);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void SetLevel(int level);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void SetServer(FString server);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void OnCharge(FString orderId,FString product,int32 amount,FString currencyType,FString payment);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void TrackEvent(FString eventName,FString properties);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void RegisterStaticProperties(FString properties);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void UnregisterStaticProperty(FString key);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void ClearStaticProperties();

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void DeviceInitialize(FString properties);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void DeviceUpdate(FString properties);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void DeviceAdd(FString properties);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void UserInitialize(FString properties);
	
	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void UserUpdate(FString properties);
	
	UFUNCTION(BlueprintCallable,Category = "TapDB")
 	static void UserAdd(FString properties);

	UFUNCTION(BlueprintCallable,Category = "TapDB")
 	static void ClearUser();
	
	UFUNCTION(BlueprintCallable,Category = "TapDB")
	static void AdvertiserIDCollectionEnabled(bool enable);



};


