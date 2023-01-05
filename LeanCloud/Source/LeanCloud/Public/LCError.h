#pragma once

class FLCError {
public:
	DECLARE_DELEGATE_OneParam(FDelegate, const FLCError& Error);
	int64 Code;
	
	FString Message;
	
	FLCError() = default;
	FLCError(int64 InCode, const FString& InMsg): Code(InCode), Message(InMsg) {};
	
};
