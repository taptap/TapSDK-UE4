// Fill out your copyright notice in the Description page of Project Settings.

#include "TULoginReflection.h"
#include "TapUELogin.h"
#include "TULoginProfileModel.h"


FString UTULoginReflection::GetOpenID()
{
	auto Profile = TapUELogin::GetProfile();
	if (Profile.IsValid())
	{
		return Profile->openid;
	} else
	{
		return "";
	}
}
