#include "TULoginProfileModel.h"
#if PLATFORM_MAC || PLATFORM_WINDOWS

#include "TULoginStorage.h"

void FTULoginProfileModel::SaveToLocal()
{
	TUDataStorage<FTULoginStorage>::SaveStruct(FTULoginStorage::Profile, *this, true);
}
	
TSharedPtr<FTULoginProfileModel> FTULoginProfileModel::GetLocalModel()
{
	return  TUDataStorage<FTULoginStorage>::LoadStruct<FTULoginProfileModel>(FTULoginStorage::Profile);
}

void FTULoginProfileModel::ClearLocalModel()
{
	TUDataStorage<FTULoginStorage>::Remove(FTULoginStorage::Profile);
}

#endif