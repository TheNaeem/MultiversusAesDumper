#include "pch.h"

#include "AES.h"
#include "Constants.h"

FAESKey GetEncodedKey()
{
	FAESKey ret;

	auto GetPakEncryptionKeyDelegate =
		Memcury::Scanner::FindPattern(Constants::Patterns::GetEncryptionKeyDelegate)
		.RelativeOffset(1)
		.GetAs<_GetPakEncryptionKeyDelegate>();

	auto EncryptionKeyDelegate = GetPakEncryptionKeyDelegate();

	(*(void(**)(void*, FAESKey*))(*(uintptr_t*)EncryptionKeyDelegate->Data + 0x48))
		(EncryptionKeyDelegate->Data, &ret);

	return ret;
}

void WINAPI Main()
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	auto rijndaelSetupEncryptObfuscated =
		Memcury::Scanner::FindPattern(Constants::Patterns::SetupEncryptObfuscated)
		.GetAs<_rijndaelSetupEncryptObfuscated>();

	unsigned int rk[60];
	rijndaelSetupEncryptObfuscated(rk, GetEncodedKey().Key, 69);

	std::string keyString = "0x";

	for (int i = 0; i < 8; ++i)
	{
		keyString += std::format("{:X}", rk[i]);
	}

	std::cout << "Found AES key: " << keyString << std::endl;
}

BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

