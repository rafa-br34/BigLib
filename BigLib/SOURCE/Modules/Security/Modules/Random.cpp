#include "../Crypts.h"

static char AsciiDict[] =
"abcdefghijklmnopqrstuvwxyz"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"0123456789";

static wchar_t WideDict[] =
L"abcdefghijklmnopqrstuvwxyz"
L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
L"0123456789";


namespace BigLib {
	namespace Security {
		std::string Crypts::RandomStringA(unsigned int Length) {
			std::string Buffer{};
			Buffer.reserve(Length); // Pre-Allocate


			for (unsigned int i = 0; i < Length; i++) {
				Buffer.append(1, AsciiDict[Crypts::Random<unsigned int>(0, (sizeof(AsciiDict) / sizeof(char)))]);
			}
			return Buffer;
		}

		std::wstring Crypts::RandomStringW(unsigned int Length) {
			std::wstring Buffer{};
			Buffer.reserve(Length); // Pre-Allocate


			for (unsigned int i = 0; i < Length; i++) {
				Buffer.append(1, WideDict[Crypts::Random<unsigned int>(0, (sizeof(WideDict) / sizeof(wchar_t)))]);
			}
			return Buffer;
		}
	}
}

