// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeanCloudImpl.h"

/**
 * 
 */
class LEANCLOUD_API FLeanCloud_PC : public FLeanCloudImpl
{
public:
	FLeanCloud_PC();
	virtual void Init(const FLCConfig& InConfig) override;

};
