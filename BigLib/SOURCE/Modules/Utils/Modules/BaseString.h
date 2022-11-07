#pragma once
#include "../../../Includes.h"

namespace BigLib {
	namespace Utils {
		// Reads The Length Of A C String
		FORCE_INLINE CONST_EXPRESSION size_t StringLength(const char* Pointer) {
			const char* Initial = Pointer;
			while (*Pointer != char(0))
				Pointer++;
			
			return Pointer - Initial;
		}
		
		// Reads The Length Of A Wide String
		FORCE_INLINE CONST_EXPRESSION size_t StringLength(const wchar_t* Pointer) {
			const wchar_t* Initial = Pointer;
			while (*Pointer != wchar_t(0))
				Pointer++;

			return Pointer - Initial;
		}

		// Converts Wide String To C String
		FORCE_INLINE CONST_EXPRESSION size_t StringConvert(const wchar_t* From, char* Output, size_t* MissedChars = nullptr, char Missing = '?') {
			size_t StringSize = StringLength(From);
			size_t MissedCharsLiteral = 0;

			for (size_t i = 0; i < StringSize; i++) {				
				if ((uint8_t)From[i] > uint8_t(0xFF)) {
					Output[i] = Missing;
					MissedCharsLiteral++;
				}
				else
					Output[i] = (char)From[i];

			}
			if (MissedChars != nullptr)
				*MissedChars = MissedCharsLiteral;

			return StringSize - MissedCharsLiteral; // Calculate How Many Characters Were Converted
		}

		// Convert C String To Wide String
		FORCE_INLINE CONST_EXPRESSION void StringConvert(const char* From, wchar_t* Output) {
			size_t StringSize = StringLength(From);

			for (size_t i = 0; i < StringSize; i++)
				Output[i] = (wchar_t)From[i];
		}

		template <typename Class>
		FORCE_INLINE CONST_EXPRESSION Class ToNumber(const char* String, size_t Base=10) {
			size_t Position = StringLength(String) - 1;
			uint8_t Digit = 0;
			Class Power = 1;
			Class Value = 0;

			while (Position >= 0) {
				Digit = 0;
				if ('0' <= String[Position] && String[Position] <= '9')
					Digit = String[Position] - '0';

				else if ('a' <= String[Position] && String[Position] <= 'z')
					Digit = (String[Position] - 'a') + 10;
				else if ('A' <= String[Position] && String[Position] <= 'Z')
					Digit = (String[Position] - 'A') + 10;

				Value += Digit * Power;
				Power *= (Class)Base;
				Position--;
			}
		}

		template <typename Class>
		FORCE_INLINE CONST_EXPRESSION Class ToNumber(const wchar_t* String, size_t Base = 10) {
			size_t Position = StringLength(String) - 1;
			uint8_t Digit = 0;
			Class Power = 1;
			Class Value = 0;

			while (Position >= 0) {
				Digit = 0;
				if (L'0' <= String[Position] && String[Position] <= L'9')
					Digit = String[Position] - L'0';

				else if (L'a' <= String[Position] && String[Position] <= L'z')
					Digit = (String[Position] - L'a') + 10;
				else if (L'A' <= String[Position] && String[Position] <= L'Z')
					Digit = (String[Position] - L'A') + 10;

				Value += Digit * Power;
				Power *= (Class)Base;
				Position--;
			}
		}
	}
}