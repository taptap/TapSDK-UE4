// Fill out your copyright notice in the Description page of Project Settings.


#include "TUDBReflection.h"

#include "TapUEDB.h"
#include "TUDBType.h"

void UTUDBReflection::Init() {
	FTUConfig Config = *FTUConfig::Get().Get();
	FTUDBConfig DBConfig;
	DBConfig.ClientId = Config.ClientID;
	DBConfig.Channel = Config.DBConfig.Channel;
	DBConfig.GameVersion = Config.DBConfig.GameVersion;
	DBConfig.RegionType = Config.RegionType;
	TapUEDB::Init(DBConfig);
}
