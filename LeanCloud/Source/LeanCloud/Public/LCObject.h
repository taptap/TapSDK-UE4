#pragma once

struct LEANCLOUD_API FLCObject : public TSharedFromThis<FLCObject>{
public:
	
	FLCObject(const FString& InClassName, TSharedPtr<FJsonObject> InServerData = nullptr);
	virtual ~FLCObject();
	
	static FLCObject CreateWithoutData(const FString& InClassName, const FString& ObjectId);

	FString GetClassName() const;
	FDateTime GetCreatedAt() const;
	FDateTime GetUpdatedAt() const;
	FString GetObjectId() const;
	void SetObjectId(const FString& ObjectId);
	const TSharedRef<FJsonObject> GetServerData() const;

	bool ParseTime(const FString& InTimeString, FDateTime& OutTime) const;

protected:

	static FString KeyCreatedAt;
	static FString KeyUpdateAt;
	static FString KeyObjectID;
	
	FString ClassName;
	TSharedPtr<FJsonObject> ServerData;
};
