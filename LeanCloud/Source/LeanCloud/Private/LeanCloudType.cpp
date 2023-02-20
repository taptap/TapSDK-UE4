#include "LeanCloudType.h"

FLCConfig FLCConfig::Instance;
FLCConfig& FLCConfig::Get()
{
	return Instance;
}

TSharedPtr<FLCError> FLCConfig::CheckConfig() const
{
	if (ClientID.IsEmpty())
	{
		return MakeShared<FLCError>(-1, TEXT("ClientID is empty."));
	}
	if (ClientToken.IsEmpty())
	{
		return MakeShared<FLCError>(-1, TEXT("ClientToken is empty."));
	}
	if (ServerURL.IsEmpty())
	{
		return MakeShared<FLCError>(-1, TEXT("ServerURL is empty."));
	}
	return nullptr;
}
