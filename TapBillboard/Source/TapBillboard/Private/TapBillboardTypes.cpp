// Fill out your copyright notice in the Description page of Project Settings.


#include "TapBillboardTypes.h"

ETapBillboardUrlType GetBillboardUrlType(const FString& Url)
{
	int32 Index = INDEX_NONE;
	Url.FindChar(TEXT('#'), Index);
	if (Index != INDEX_NONE)
	{
		const FString& SubStr = Url.Right(Url.Len() - Index - 1);
		if (SubStr == TEXT("com_taptap_tdsbillboard_audible"))
		{
			return ETapBillboardUrlType::AudioOn;
		}
		else if(SubStr == TEXT("com_taptap_tdsbillboard_mute"))
		{
			return ETapBillboardUrlType::AudioOff;
		}
	}

	if(Url.Left(11) != TEXT("javascript:") && Url.Left(7) != TEXT("http://") && Url.Left(8) != TEXT("https://"))
	{
		return ETapBillboardUrlType::Custom;
	}
	
	if (Url.Right(17) == TEXT("tdsTarget=browser"))
	{
		return ETapBillboardUrlType::LaunchSystemBrowser;
	}
	return ETapBillboardUrlType::LaunchInnerBrowser;
}

FString GetLCSyncString()
{
	const int64 Ticks = FDateTime::UtcNow().ToUnixTimestamp() * 1000;
	const FString TimeStr = FString::Printf(TEXT("%010lld"), Ticks);
	return FMD5::HashAnsiString(*(TimeStr + FTUConfig::Get()->ClientToken)) + TEXT(",") + TimeStr;
}