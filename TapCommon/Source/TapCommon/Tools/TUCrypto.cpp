#include "TUCrypto.h"


TArray<uint8> TUCrypto::UTF8Encode(const FString& content)
{
	FTCHARToUTF8 cnv(*content);
	int32 Len = cnv.Length();
	TArray<uint8> OutArray;
	OutArray.AddUninitialized(Len);
	FMemory::Memcpy(OutArray.GetData(), cnv.Get(), Len);
	return OutArray;
}

FString TUCrypto::UTF8Encode(const TArray<uint8>& data)
{
	FUTF8ToTCHAR cnv((const ANSICHAR*)data.GetData(), data.Num());
	FString UTF8Text(cnv.Length(), cnv.Get());
	//return (TCHAR*)FUTF8ToTCHAR((const ANSICHAR*)InArray.GetData(), InArray.Num()).Get();
	//FString UTF8Text(InArray.Num(), (TCHAR*)FUTF8ToTCHAR((const ANSICHAR*)InArray.GetData(), InArray.Num()).Get());
	return MoveTemp(UTF8Text);
}


FString TUCrypto::Base64Encode(const TArray<uint8>& data)
{
	return FBase64::Encode(data);
}

TArray<uint8> TUCrypto::Base64Decode(const FString& content)
{
	TArray<uint8> outData;
	FBase64::Decode(content, outData);
	return outData;
}

FString TUCrypto::UrlBase64Encode(const TArray<uint8>& data) {
	FString Result = Base64Encode(data);
	Result.ReplaceInline(TEXT("+"), TEXT("-"));
	Result.ReplaceInline(TEXT("/"), TEXT("_"));
	Result.ReplaceInline(TEXT("="), TEXT(""));
	return Result;
}

TArray<uint8> TUCrypto::UrlBase64Decode(const FString& content) {
	FString Result = content;
	Result.ReplaceInline(TEXT("-"), TEXT("+"));
	Result.ReplaceInline(TEXT("_"), TEXT("/"));
	int Mod4 = Result.Len() % 4;
	if (Mod4 > 0) {
		for (int i = 0; i < 4 - Mod4; i++) {
			Result += "=";
		}
	}
	return Base64Decode(Result);
}

TArray<uint8> TUCrypto::HmacSHA1(const TArray<uint8>& content, const TArray<uint8>& key)
{
	TArray<uint8> sign;
	sign.SetNum(20); // sha1 固定20位
	FSHA1::HMACBuffer(key.GetData(), key.Num(), content.GetData(), content.Num(), sign.GetData());
	return sign;
}

TArray<uint8> TUCrypto::AesEncode(const TArray<uint8>& content, const TArray<uint8>& key, bool isPadding)
{
	if (content.Num() == 0) {
		return content;
	}
	FAES::FAESKey AesKey;
	FMemory::Memcpy(AesKey.Key, key.GetData(), FMath::Min(FAES::FAESKey::KeySize, key.Num()));
	
	auto ContentSize = content.Num();
	auto ContentData = content;
	auto Alignment = FAES::AESBlockSize;

	uint32 Padding = Alignment - (ContentSize % Alignment);

	if (Padding == Alignment && isPadding == false)
	{
		Padding = 0;
	}

	ContentData.AddUninitialized(Padding);

	for (int32 FillIndex = ContentSize; FillIndex < ContentData.Num(); FillIndex++)
	{
		ContentData[FillIndex] = isPadding ? Padding : 0;
	}

	FAES::EncryptData(ContentData.GetData(), ContentData.Num(), AesKey);
	return ContentData;
}

TArray<uint8> TUCrypto::AesDecode(const TArray<uint8>& content, const TArray<uint8>& key, bool isPadding)
{
	if (content.Num() == 0) {
		return content;
	}
	FAES::FAESKey AesKey;
	FMemory::Memcpy(AesKey.Key, key.GetData(), FMath::Min(FAES::FAESKey::KeySize, key.Num()));
	
	auto ContentData = content;
	auto Alignment = FAES::AESBlockSize;
	auto dirtyCount = content.Num() % Alignment;
	ContentData.SetNum(content.Num() - dirtyCount);

	FAES::DecryptData(ContentData.GetData(), ContentData.Num(), AesKey);
	if (ContentData.Num() == 0) {
		return ContentData;
	}

	if (isPadding)
	{
		auto Padding = ContentData.Last();
		if (ContentData.Num() >= Padding) {
			ContentData.SetNum(ContentData.Num() - Padding);
		}
	}
	return ContentData;
	
}






