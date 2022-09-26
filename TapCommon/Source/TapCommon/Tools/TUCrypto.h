#pragma once

class TAPCOMMON_API TUCrypto
{
public:

	static TArray<uint8> UTF8Encode(const FString& content);
	static FString UTF8Encode(const TArray<uint8>& data);

	static FString Base64Encode(const TArray<uint8>& data);
	static TArray<uint8> Base64Decode(const FString& content);

	// 会对base64字符串中引起Url歧义的部分进行处理
	static FString UrlBase64Encode(const TArray<uint8>& data);
	static TArray<uint8> UrlBase64Decode(const FString& content);

	static TArray<uint8> HmacSHA1(const TArray<uint8>& content,  const TArray<uint8>& key);

	// 这里用的是PKCS7Padding，大多数padding都是通用的。
	// key会默认设置成32字节，多了会截断，少了补零（框架好像支持32的倍数的，但是一般不会超过）。
	static TArray<uint8> AesEncode(const TArray<uint8>& content,  const TArray<uint8>& key, bool isPadding = true);
	static TArray<uint8> AesDecode(const TArray<uint8>& content,  const TArray<uint8>& key, bool isPadding = true);
	
};
