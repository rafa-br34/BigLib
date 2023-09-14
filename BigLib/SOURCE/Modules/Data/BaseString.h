#pragma once
#include "../../Includes.h"

namespace BigLib {
	namespace Strings {
		// TODO: Add optimization options

		template<typename StringType=char, typename LimiterType=umax>
		CONST_EXPRESSION LimiterType StringLength(CONST StringType* Pointer, LimiterType Limit = LimiterType(0)) {
			CONST StringType* Initial = Pointer;
			LimiterType Counter = Limit;

			while (*Pointer != StringType(0)) {
				if ((Limit != 0) && (Counter-- <= 0)) break;
			
				Pointer++;
			}
			
			return LimiterType(Pointer - Initial);
		}
		
		// Converts wide string to C string
		CONST_EXPRESSION umax StringConvert(CONST wchar_t* From, char* Output, umax* MissedChars = nullptr, char Missing = '?') {
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

		// Convert C string to wide string
		FORCE_INLINE void StringConvert(CONST char* From, wchar_t* Output) {
			for (umax i = 0; i < StringLength(From); i++)
				Output[i] = (wchar_t)From[i];
		}

		template<typename Type = char, typename CounterType = umax>
		CONST_EXPRESSION CounterType FindCharacter(CONST Type* String, CounterType Length, Type TargetCharacter) {
			for (CounterType i = 0; i < Length; i++)
				if (String[i] == TargetCharacter)
					return i;

			return CounterType(-1);
		}

		template <typename Class, typename CounterType = umax>
		CONST_EXPRESSION Class ToNumber(CONST char* String, CounterType Size, CounterType Base=10) {
			uint8 Digit = 0;
			Class Power = 1;
			Class Value = 0;
			
			while (Size > 0) {
				Digit = 0;
				auto Char = String[Size - 1];

				if ('0' <= Char && Char <= '9')
					Digit = Char - '0';
				else if ('a' <= Char && Char <= 'z')
					Digit = (Char - 'a') + 10;
				else if ('A' <= Char && Char <= 'Z')
					Digit = (Char - 'A') + 10;
				else
					break;

				Value += Digit * Power;
				Power *= (Class)Base;
				Size--;
			}

			return Value;
		}

		template <typename Class=int, typename CharType=char, typename CounterType = umax>
		CONST_EXPRESSION Class ToNumber(CONST CharType* String, CounterType Size, CONST CharType* Dictionary="0123456789") {
			CounterType Base = StringLength<CharType, CounterType>(Dictionary);

			Class Digit = 0;
			Class Power = 1;
			Class Value = 0;
			while (Size > 0) {
				Digit = FindCharacter(Dictionary, Base, String[Size - 1]);
				if (Digit == Class(-1))
					break;
				
				Value += Digit * Power;
				Power *= (Class)Base;

				Size--;
			}

			return Value;
		}

		template <typename Class, typename CounterType = umax, typename CharType = char>
		CONST_EXPRESSION Class ToNumber(CONST CharType* String, CounterType Base = 10) {
			return ToNumber(String, StringLength(String), Base);
		}

		template<typename Type = char, typename CounterType = umax>
		CONST_EXPRESSION CounterType ReplaceCharacters(Type* String, CounterType Length, Type TargetCharacter, Type NewCharacter) {
			CounterType Occurences = 0;
			CounterType Offset = 0;

			while ((Offset = FindCharacter<Type, CounterType>(String + Offset, Length - Offset, TargetCharacter)) != CounterType(-1)) {
				String[Offset] = NewCharacter;
				Occurences++;
				Offset++;
			}
			
			return Occurences;
		}

		template<typename Type=char, typename CounterType=umax>
		CONST_EXPRESSION CounterType ReplaceCharacters(Type* String, Type TargetCharacter, Type NewCharacter) {
			return ReplaceCharacters(String, StringLength(String), TargetCharacter, NewCharacter);
		}


	}
}