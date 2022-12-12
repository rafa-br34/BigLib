#pragma once
#include "../../../Includes.h"

/*
static char AsciiDict[] =
"abcdefghijklmnopqrstuvwxyz"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"0123456789";

static wchar_t WideDict[] =
L"abcdefghijklmnopqrstuvwxyz"
L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
L"0123456789";


namespace BigLib {
	namespace Crypts {
		template<typename Type>
		Type SecureRandom(Type Min, Type Max) {
			return Type{ 0 };
		}

		char* RandomStringA(unsigned int Length) {
			std::string Buffer{};
			Buffer.reserve(Length); // Pre-Allocate


			for (unsigned int i = 0; i < Length; i++) {
				Buffer.append(1, AsciiDict[SecureRandom<unsigned int>(0, (sizeof(AsciiDict) / sizeof(char)))]);
			}
			return (char*)"0";//Buffer;
		}

		wchar_t* RandomStringW(unsigned int Length) {
			std::wstring Buffer{};
			Buffer.reserve(Length); // Pre-Allocate


			for (unsigned int i = 0; i < Length; i++) {
				Buffer.append(1, WideDict[SecureRandom<unsigned int>(0, (sizeof(WideDict) / sizeof(wchar_t)))]);
			}
			return (wchar_t*)L"0";//Buffer;
		}
	}
}
*/