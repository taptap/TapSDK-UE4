#include "TUOpenSSL.h"

#include "TUCrypto.h"
#include "TUDebuger.h"

namespace TUOpenSSL_Space {
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

	typedef int (*RSAFunc)(int flen, const unsigned char *from, unsigned char *to, RSA *rsa, int padding);
	static bool RSAEncryptOrDecrypt(const TArray<uint8>& Content, TArray<uint8>& Result, RSA* RSA, const RSAFunc RSAFunc, bool IsEncrypt) {

		// 计算加密块大小
		int nKeySize = RSA_size(RSA);
		int nBlockSize = IsEncrypt ? (nKeySize - RSA_PKCS1_PADDING_SIZE) : nKeySize;

		const unsigned char* pIn = Content.GetData();
		int nInSize = Content.Num();

		unsigned char* pBuf = new unsigned char[nKeySize];
		Result.Empty();
		// 分组迭代加密
		for (int i = 0; i < nInSize; )
		{
			int nBlockLen = (nInSize - i > nBlockSize ? nBlockSize : nInSize - i);

			int ret = RSAFunc(nBlockLen, pIn + i, pBuf, RSA, RSA_PKCS1_PADDING);
			if (ret <= 0)
			{
				delete[] pBuf;
				return false;
			}
			Result.Append(pBuf, ret);
			i += nBlockLen;
		}

		delete[] pBuf;
		return true;
	}

	RSA* GenerateKey(const FString& Key, bool IsPublic) {
		RSA* _RSA = nullptr;
		FString KeyString = Key.Replace(TEXT("\n"), TEXT(""));
		if (IsPublic) {
			KeyString = KeyString.Replace(TEXT("-----BEGIN RSA PUBLIC KEY-----"), TEXT(""));
			KeyString = KeyString.Replace(TEXT("-----END RSA PUBLIC KEY-----"), TEXT(""));
			KeyString = KeyString.Replace(TEXT("-----BEGIN PUBLIC KEY-----"), TEXT(""));
			KeyString = KeyString.Replace(TEXT("-----END PUBLIC KEY-----"), TEXT(""));
		} else {
			KeyString = KeyString.Replace(TEXT("-----BEGIN RSA PRIVATE KEY-----"), TEXT(""));
			KeyString = KeyString.Replace(TEXT("-----END RSA PRIVATE KEY-----"), TEXT(""));
		}
		int Pos = 0;
		// int RSA_KEYSUB_LEN = 79;
		int RSA_KEYSUB_LEN = 64;
		FString Temp;
		
		while (Pos < KeyString.Len()) {
			Temp += KeyString.Mid(Pos, RSA_KEYSUB_LEN);
			Temp += "\n";
			Pos += RSA_KEYSUB_LEN;
		}

		if (IsPublic) {
			KeyString = "-----BEGIN RSA PUBLIC KEY-----\n" + Temp + "-----END RSA PUBLIC KEY-----";
			// TUDebuger::DisplayLog(KeyString);
			auto KeyData = TUCrypto::UTF8Encode(KeyString);
			BIO* _BIO = BIO_new_mem_buf(KeyData.GetData(), KeyData.Num());
			_RSA = PEM_read_bio_RSAPublicKey(_BIO, nullptr, nullptr, nullptr);
			BIO_free(_BIO);
			if (_RSA == nullptr) {
				KeyString = "-----BEGIN PUBLIC KEY-----\n" + Temp + "-----END PUBLIC KEY-----";
				// TUDebuger::DisplayLog(KeyString);
				KeyData = TUCrypto::UTF8Encode(KeyString);
				_BIO = BIO_new_mem_buf(KeyData.GetData(), KeyData.Num());
				_RSA = PEM_read_bio_RSA_PUBKEY(_BIO, nullptr, nullptr, nullptr);
				BIO_free(_BIO);
			}
		}
		else {
			KeyString = "-----BEGIN RSA PRIVATE KEY-----\n" + Temp + "-----END RSA PRIVATE KEY-----";
			// TUDebuger::DisplayLog(KeyString);
			auto KeyData = TUCrypto::UTF8Encode(KeyString);
			BIO* _BIO = BIO_new_mem_buf(KeyData.GetData(), KeyData.Num());
			_RSA = PEM_read_bio_RSAPrivateKey(_BIO, nullptr, nullptr, nullptr);
			BIO_free(_BIO);
		}
		return _RSA;
	}
}
#include <openssl/sha.h>



FString TUOpenSSL::LexToString(const TArray<uint8>& Content) {
	FString Result;
	for (TArray<uint8>::TConstIterator it(Content); it; ++it) {
		Result += FString::Printf(TEXT("%02x"), *it);
	}
	return Result;
}

TArray<uint8> TUOpenSSL::Sha256(const TArray<uint8>& Content) {
	TArray<uint8> Sign;
	Sign.SetNum(32); // sha256 固定32位
	TUOpenSSL_Space::SHA256(Content.GetData(), Content.Num(), Sign.GetData());
	return Sign;
}

TArray<uint8> TUOpenSSL::RSAEncryptPublic(const TArray<uint8>& Content, const FString& Key) {
	TArray<uint8> Result;
	TUOpenSSL_Space::RSA* RSA = TUOpenSSL_Space::GenerateKey(Key, true);

	if (RSA == nullptr) {
		TUDebuger::ErrorLog("RSA Public Key Load Fail");
		return Result;
	}
	TUOpenSSL_Space::RSAEncryptOrDecrypt(Content, Result, RSA, TUOpenSSL_Space::RSA_public_encrypt, true);
	TUOpenSSL_Space::RSA_free(RSA);
	return Result;
}

TArray<uint8> TUOpenSSL::RSAEncryptPrivate(const TArray<uint8>& Content, const FString& Key) {
	TArray<uint8> Result;
	TUOpenSSL_Space::RSA* RSA = TUOpenSSL_Space::GenerateKey(Key, false);

	if (RSA == nullptr) {
		TUDebuger::ErrorLog("RSA Private Key Load Fail");
		return Result;
	}
	TUOpenSSL_Space::RSAEncryptOrDecrypt(Content, Result, RSA, TUOpenSSL_Space::RSA_private_encrypt, true);
	TUOpenSSL_Space::RSA_free(RSA);
	return Result;
}

TArray<uint8> TUOpenSSL::RSADecryptPublic(const TArray<uint8>& Content, const FString& Key) {
	TArray<uint8> Result;
	TUOpenSSL_Space::RSA* RSA = TUOpenSSL_Space::GenerateKey(Key, true);

	if (RSA == nullptr) {
		TUDebuger::ErrorLog("RSA Public Key Load Fail");
		return Result;
	}
	TUOpenSSL_Space::RSAEncryptOrDecrypt(Content, Result, RSA, TUOpenSSL_Space::RSA_public_decrypt, false);
	TUOpenSSL_Space::RSA_free(RSA);
	return Result;
}

TArray<uint8> TUOpenSSL::RSADecryptPrivate(const TArray<uint8>& Content, const FString& Key) {
	TArray<uint8> Result;
	TUOpenSSL_Space::RSA* RSA = TUOpenSSL_Space::GenerateKey(Key, false);

	if (RSA == nullptr) {
		TUDebuger::ErrorLog("RSA Private Key Load Fail");
		return Result;
	}
	TUOpenSSL_Space::RSAEncryptOrDecrypt(Content, Result, RSA, TUOpenSSL_Space::RSA_private_decrypt, false);
	TUOpenSSL_Space::RSA_free(RSA);
	return Result;
}

// TArray<uint8> TduOpenSSL::HmacSha256(const TArray<uint8>& Content, const TArray<uint8>& Key) {
// 	TArray<uint8> Sign;
// 	Sign.SetNum(32); // sha256 固定32位
// 	unsigned int OutputLen = 0;
// 	HMAC(EVP_sha256(), Key.GetData(), Key.Num(), Content.GetData(), Content.Num(), Sign.GetData(), &OutputLen);
// 	return Sign;
// }

