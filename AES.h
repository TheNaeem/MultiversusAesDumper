#pragma once

typedef void (*_EncryptionKeyCallback)(void* DelegateData, struct FAESKey* OutKey);
typedef __int64 (*_rijndaelSetupEncryptObfuscated)(unsigned int* rk, const unsigned __int8* key, int keybits);
typedef struct EncryptionKeyDelegate* (*_GetPakEncryptionKeyDelegate)();

struct EncryptionKeyDelegate
{
	uintptr_t* Data;
	int DelegateSize;
};

struct FAESKey
{
	static constexpr int KeySize = 32;
	static constexpr int AESBlockSize = 16;

	uint8_t Key[128];

	FAESKey();
	FAESKey(std::string KeyString);

	bool operator==(const FAESKey& Other) const;
	bool IsValid() const;
	std::string ToString();
};