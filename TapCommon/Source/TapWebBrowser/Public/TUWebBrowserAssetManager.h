// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UObject/SoftObjectPtr.h"
#include "Materials/Material.h"
#include "TUWebBrowserAssetManager.generated.h"

class UMaterial;
/**
 * 
 */
UCLASS()
class TAPWEBBROWSER_API UTUWebBrowserAssetManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void LoadDefaultMaterials();

	UMaterial* GetDefaultMaterial(); 
	UMaterial* GetDefaultTranslucentMaterial(); 

protected:
	UPROPERTY()
	TSoftObjectPtr<UMaterial> DefaultMaterial;
	TSoftObjectPtr<UMaterial> DefaultTranslucentMaterial;
};
