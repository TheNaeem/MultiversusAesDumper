#include "pch.h"

#include "AES.h"

FAESKey::FAESKey()
{
	memset(Key, 0, KeySize);
}

FAESKey::FAESKey(std::string KeyHexString)
{
	if (KeyHexString.starts_with("0x"))
		KeyHexString.erase(0, 2);

	for (unsigned int i = 0; i < KeyHexString.length() / 2; i++)
	{
		Key[i] = static_cast<uint8_t>(strtol(KeyHexString.substr(i * 2, 2).c_str(), NULL, 16));
	}
}

bool FAESKey::IsValid() const
{
	auto Words = (uint32_t*)Key;
	for (auto Index = 0; Index < KeySize / 4; ++Index)
	{
		if (Words[Index] != 0)
		{
			return true;
		}
	}

	return false;
}

std::string FAESKey::ToString()
{
	std::ostringstream ret;
	ret << std::hex << std::uppercase << std::setfill('0');

	for (size_t i = 0; i < KeySize; i++)
	{
		ret << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(Key[i]);
	}

	return ret.str();
}

bool FAESKey::operator==(const FAESKey& Other) const
{
	return memcmp(Key, Other.Key, KeySize) == 0;
}