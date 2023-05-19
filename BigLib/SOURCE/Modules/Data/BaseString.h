#pragma once
#include "../../Includes.h"

namespace BigLib {
	namespace Strings {
		template<typename StringType=char, typename LimiterType=umax>
		FORCE_INLINE LimiterType StringLength(const StringType* Pointer, LimiterType Limit=LimiterType(0)) {
			const StringType* Initial = Pointer;
			LimiterType Counter = Limit;

			while (*Pointer != StringType(0)) {
				if (Limit > 0) {
					if (Counter <= 0) break;
					Counter--;
				}
			
				Pointer++;
			}
			
			return Pointer - Initial;
		}
		
		// Converts Wide String To C String
		FORCE_INLINE umax StringConvert(const wchar_t* From, char* Output, umax* MissedChars = nullptr, char Missing = '?') {
			umax StringSize = StringLength(From);
			umax MissedCharsLiteral = 0;

			for (umax i = 0; i < StringSize; i++) {
				if (From[i] > wchar_t(0xFF)) {
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
		FORCE_INLINE void StringConvert(const char* From, wchar_t* Output) {
			for (umax i = 0; i < StringLength(From); i++)
				Output[i] = (wchar_t)From[i];
		}

		template <typename Class>
		FORCE_INLINE Class ToNumber(const char* String, umax Base=10) {
			umax Position = StringLength(String) - 1;
			uint8 Digit = 0;
			Class Power = 1;
			Class Value = 0;

			while (Position != 0) {
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

			return Value;
		}

		template <typename Class>
		FORCE_INLINE Class ToNumber(const wchar_t* String, umax Base = 10) {
			umax Position = StringLength(String) - 1;
			uint8 Digit = 0;
			Class Power = 1;
			Class Value = 0;

			while (Position != 0) {
				Digit = 0;
				if (L'0' <= String[Position] && String[Position] <= L'9')
					Digit = uint8(String[Position] - L'0');

				else if (L'a' <= String[Position] && String[Position] <= L'z')
					Digit = uint8((String[Position] - L'a') + 10);
				else if (L'A' <= String[Position] && String[Position] <= L'Z')
					Digit = uint8((String[Position] - L'A') + 10);

				Value += Digit * Power;
				Power *= (Class)Base;
				Position--;
			}
			return Value;
		}
	}
}