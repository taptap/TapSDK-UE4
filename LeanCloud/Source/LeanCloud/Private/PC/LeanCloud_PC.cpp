// Fill out your copyright notice in the Description page of Project Settings.


#include "LeanCloud_PC.h"


FLeanCloud_PC::FLeanCloud_PC()
{
}

void FLeanCloud_PC::Init(const FLCConfig& InConfig)
{
	FLCConfig::Get() = InConfig;
}

