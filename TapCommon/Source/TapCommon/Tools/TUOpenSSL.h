#pragma once

class TAPCOMMON_API TUOpenSSL {
public:
	
	static FString LexToString(const TArray<uint8>& Content);
	
	static TArray<uint8> Sha256(const TArray<uint8>& Content);

	// static TArray<uint8> HmacSha256(const TArray<uint8>& Content, const TArray<uint8>& Key);

	static TArray<uint8> RSAEncryptPublic(const TArray<uint8>& Content,  const FString& Key);
	static TArray<uint8> RSAEncryptPrivate(const TArray<uint8>& Content,  const FString& Key);
	static TArray<uint8> RSADecryptPublic(const TArray<uint8>& Content,  const FString& Key);
	static TArray<uint8> RSADecryptPrivate(const TArray<uint8>& Content,  const FString& Key);
	
	
};
