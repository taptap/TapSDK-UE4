// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapBillboardTypes.h"
#include "JsonWrapper.generated.h"

USTRUCT()
struct FAnnouncementGeneralDataList
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FAnnouncementGeneralData> list;
};

USTRUCT()
struct FAnnouncementDetailDataList
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FAnnouncementDetailData> list;
};

USTRUCT()
struct FAnnouncementMarkRead
{
	GENERATED_BODY()

	UPROPERTY()
	FString msg;
};

USTRUCT()
struct FMarqueeContentChild
{
	GENERATED_BODY()

	UPROPERTY()
	FString text;

	UPROPERTY()
	bool bold;

	UPROPERTY()
	bool highlight;

	UPROPERTY()
	bool italic;

	UPROPERTY()
	bool underline;
};

USTRUCT()
struct FMarqueeContentElement
{
	GENERATED_BODY()

	UPROPERTY()
	FString type;

	UPROPERTY()
	TArray<FMarqueeContentChild> children;

	UPROPERTY()
	TMap<FString, FString> info;
};
