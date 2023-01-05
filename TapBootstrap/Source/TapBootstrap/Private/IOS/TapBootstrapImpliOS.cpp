#include "TapBootstrapImpliOS.h"
#include "TUError.h"
#include "TUType.h"
#include "IOSHelper.h"
#include "TapBootstrapObjcHelper.h"
#include "TUHelper.h"

#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
#import <TapBootstrapSDK/TapBootstrapSDK.h>

#define TAP_BOOTSTRAP_SERVICE "TapBootstrapService"

FTapBootstrapImpliOS::~FTapBootstrapImpliOS() {
}

void FTapBootstrapImpliOS::Init(const FTUConfig& InConfig) {

	TapConfig* tapConfig = [TapConfig new];
	tapConfig.clientId = IOSHelper::Convert(InConfig.ClientID);
	if (InConfig.RegionType == ERegionType::CN) {
		tapConfig.region = TapSDKRegionTypeCN;
	} else {
		tapConfig.region = TapSDKRegionTypeIO;
	}
	tapConfig.clientToken = IOSHelper::Convert(InConfig.ClientToken);
	tapConfig.serverURL = IOSHelper::Convert(InConfig.ServerURL);
	if (InConfig.DBConfig.Enable) {
		TapDBConfig* dbConfig = [TapDBConfig new];
		dbConfig.channel = IOSHelper::Convert(InConfig.DBConfig.Channel);
		dbConfig.gameVersion = IOSHelper::Convert(InConfig.DBConfig.GameVersion);
		dbConfig.enable = InConfig.DBConfig.Enable;
		dbConfig.advertiserIDCollectionEnabled = InConfig.DBConfig.AdvertiserIDCollectionEnabled;
		tapConfig.dbConfig = dbConfig;
	}
	[TapBootstrap initWithConfig:tapConfig];
	
}

TSharedPtr<FTDSUser> FTapBootstrapImpliOS::GetCurrentUser() {
	return TapBootstrapObjcHelper::Convert([TDSUser currentUser]);
}

void FTapBootstrapImpliOS::Logout() {
	[TDSUser logOut];
}

void FTapBootstrapImpliOS::LoginAnonymously(FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {

	dispatch_async(dispatch_get_main_queue(), ^{
		[TDSUser loginAnonymously:^(TDSUser *_Nullable user, NSError *_Nullable error) {
			if (error)
			{
				FTUError Error = IOSHelper::Convert(error);
				TUHelper::PerformOnGameThread([=]() {
					OnError.ExecuteIfBound(Error);
				});
			}
			else 
			{
				TSharedPtr<FTDSUser> UserPtr = TapBootstrapObjcHelper::Convert(user);
				TUHelper::PerformOnGameThread([=]() {
					if (UserPtr.IsValid()) {
						OnSuccess.ExecuteIfBound(*UserPtr.Get());
					} else {
						OnError.ExecuteIfBound(FTUError(-1, "OC TDSUser is null"));
					}
				});
			}
		}];
	});
}

void FTapBootstrapImpliOS::LoginWithTapTap(const TArray<FString>& Permissions, FTDSUser::FDelegate OnSuccess,
	FTUError::FDelegate OnError) {
	NSArray *Permissions_iOS = IOSHelper::Convert(Permissions);
	dispatch_async(dispatch_get_main_queue(), ^{
		[TDSUser loginByTapTapWithPermissions:Permissions_iOS callback:^(TDSUser *_Nullable user, NSError *_Nullable error) {
			if (error)
			{
				FTUError Error = IOSHelper::Convert(error);
				TUHelper::PerformOnGameThread([=]() {
					OnError.ExecuteIfBound(Error);
				});
			}
			else 
			{
				TSharedPtr<FTDSUser> UserPtr = TapBootstrapObjcHelper::Convert(user);
				TUHelper::PerformOnGameThread([=]() {
					if (UserPtr.IsValid()) {
						OnSuccess.ExecuteIfBound(*UserPtr.Get());
					} else {
						OnError.ExecuteIfBound(FTUError(-1, "OC TDSUser is null"));
					}
				});
			}
		}];
	});
}

void FTapBootstrapImpliOS::BecomeWithSessionToken(const FString& SessionToken, FTDSUser::FDelegate OnSuccess,
	FTUError::FDelegate OnError) {
	
	[TDSUser becomeWithSessionTokenInBackground:IOSHelper::Convert(SessionToken) block:^(LCUser * _Nullable user, NSError * _Nullable error) {
		if (error)
		{
			FTUError Error = IOSHelper::Convert(error);
			TUHelper::PerformOnGameThread([=]() {
				OnError.ExecuteIfBound(Error);
			});
		}
		else 
		{
			TSharedPtr<FTDSUser> UserPtr = TapBootstrapObjcHelper::Convert(user);
			TUHelper::PerformOnGameThread([=]() {
				if (UserPtr.IsValid()) {
					OnSuccess.ExecuteIfBound(*UserPtr.Get());
				} else {
					OnError.ExecuteIfBound(FTUError(-1, "OC TDSUser is null"));
				}
			});
		}
	}];
}

void FTapBootstrapImpliOS::LoginWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData,
	FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	TDSUser * user = [TDSUser user];
	NSDictionary *authData = IOSHelper::Convert(AuthData);
	if (!authData) {
		authData = @{};
	}
	[user loginWithAuthData:authData platformId:IOSHelper::Convert(Platform) options:nil callback:^(BOOL succeeded, NSError * _Nullable error) {
		if (succeeded) {
			TSharedPtr<FTDSUser> UserPtr = TapBootstrapObjcHelper::Convert(user);
			TUHelper::PerformOnGameThread([=]() {
				if (UserPtr.IsValid()) {
					OnSuccess.ExecuteIfBound(*UserPtr.Get());
				} else {
					OnError.ExecuteIfBound(FTUError(-1, "OC TDSUser is null"));
				}
			});
		} else {
			FTUError Error = IOSHelper::Convert(error);
			TUHelper::PerformOnGameThread([=]() {
				OnError.ExecuteIfBound(Error);
			});
		}
	}];
}

void FTapBootstrapImpliOS::AssociateWithAuthData(const FTDSUser& InUser, const FString& Platform,
	TSharedPtr<FJsonObject> AuthData, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	TDSUser * user = TapBootstrapObjcHelper::Convert(InUser);
	NSDictionary *authData = IOSHelper::Convert(AuthData);
	if (!authData) {
		authData = @{};
	}
	[user associateWithAuthData:authData platformId:IOSHelper::Convert(Platform) options:nil callback:^(BOOL succeeded, NSError * _Nullable error) {
		if (succeeded) {
			TSharedPtr<FTDSUser> UserPtr = TapBootstrapObjcHelper::Convert(user);
			TUHelper::PerformOnGameThread([=]() {
				if (UserPtr.IsValid()) {
					OnSuccess.ExecuteIfBound(*UserPtr.Get());
				} else {
					OnError.ExecuteIfBound(FTUError(-1, "OC TDSUser is null"));
				}
			});
		} else {
			FTUError Error = IOSHelper::Convert(error);
			TUHelper::PerformOnGameThread([=]() {
				OnError.ExecuteIfBound(Error);
			});
		}
	}];
}

void FTapBootstrapImpliOS::DisassociateAuthData(const FTDSUser& InUser, const FString& Platform,
	FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	TDSUser * user = TapBootstrapObjcHelper::Convert(InUser);
	[user disassociateWithPlatformId:IOSHelper::Convert(Platform) callback:^(BOOL succeeded, NSError * _Nullable error) {
		if (succeeded) {
			TSharedPtr<FTDSUser> UserPtr = TapBootstrapObjcHelper::Convert(user);
			TUHelper::PerformOnGameThread([=]() {
				if (UserPtr.IsValid()) {
					OnSuccess.ExecuteIfBound(*UserPtr.Get());
				} else {
					OnError.ExecuteIfBound(FTUError(-1, "OC TDSUser is null"));
				}
			});
		} else {
			FTUError Error = IOSHelper::Convert(error);
			TUHelper::PerformOnGameThread([=]() {
				OnError.ExecuteIfBound(Error);
			});
		}
	}];
}

void FTapBootstrapImpliOS::SaveUser(const FTDSUser& InUser) {
	TDSUser * user = TapBootstrapObjcHelper::Convert(InUser);
	[user saveInBackground];
}

void FTapBootstrapImpliOS::QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
	FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess, FTUError::FDelegate OnError) {

	[TDSLeaderBoardRanking queryTapFriendsLeaderBoardWithName:IOSHelper::Convert(Name) from:From limit:Limit callback:^(NSArray<TDSLeaderBoardRanking *> * _Nullable leaderboards, NSError * _Nullable error) {
		if (error) {
			FTUError Error = IOSHelper::Convert(error);
			TUHelper::PerformOnGameThread([=]() {
				OnError.ExecuteIfBound(Error);
			});
		} else {
			TArray<FTDSLeaderBoardRanking> UE_Rankings;
			for (TDSLeaderBoardRanking *ranking in leaderboards) {
				FTDSLeaderBoardRanking UE_Ranking;
				UE_Ranking.Rank = ranking.rank;
				UE_Ranking.StatisticValue = ranking.statisticValue;
				UE_Ranking.StatisticName = IOSHelper::Convert(ranking.statisticName);
				UE_Ranking.User = TapBootstrapObjcHelper::Convert(ranking.user);
				UE_Rankings.Add(UE_Ranking);
			}
			TUHelper::PerformOnGameThread([=]() {
				OnSuccess.ExecuteIfBound(UE_Rankings);
			});
		}
	}];
}
