#pragma once
#include "../../Includes.h"

#define BIT(Index)								(UI64(1) << (Index))
#define SIZEOF_BITS(Value)							(sizeof(Value) * 8)
#define HAS_BIT(Value, Index)						((Value) & BIT(Index))
#define GET_BIT(Value, Index)						(HAS_BIT(Value, Index) >> (Index))
#define SET_BIT(Value, Index, NewBit)				(GET_BIT(Value, Index) == (BitValue) ? (Value) : (Value) ^ BIT(Index))
#define GET_LOW_BITS(Value, Lowest)					(((Value) << (SIZEOF_BITS(Value) - (Lowest))) >> (SIZEOF_BITS(Value) - (Lowest)))
#define __GET_BITS_MAX(StartBit, EndBit)				((1 << ((EndBit) - (StartBit))) - 1)
#define SET_BITS(Value, StartBit, EndBit, NewVal)	(((Value) ^ ((Value) & (__GET_BITS_MAX(StartBit, EndBit) << (StartBit)))) | (((NewVal) & __GET_BITS_MAX(StartBit, EndBit)) << (StartBit)))
#define GET_BITS(Value, StartBit, EndBit)			(((Value) & (__GET_BITS_MAX(StartBit, EndBit) << (StartBit))) >> (StartBit))



namespace BigLib {
	namespace Bitwise {

		template<typename Type, typename RType=unsigned>
		FORCE_INLINE Type RotateRight(Type X, RType R) {
			return (X >> R) | (X << (-R & (SIZEOF_BITS(Type) - 1)));
		}

		template<typename Type, typename RType=unsigned>
		FORCE_INLINE Type RotateLeft(Type X, RType R) {
			return (X << R) | (X >> (-R & (SIZEOF_BITS(Type) - 1)));
		}

		template<typename Type>
		CONST_EXPRESSION Type SwapEndianness(Type Number) {
			Type Size = SIZEOF_BITS(Type);
			Type NewNumber = 0;

			while (Size > 0) {
				NewNumber |= (GET_BITS(Number, Size - 8, Size) << (SIZEOF_BITS(Type) - Size));
				Size -= 8;
			}
			return NewNumber;
		}

		// Converts a LE value to the native architecture or vice versa.
		template<typename Type>
		FORCE_INLINE Type InterpretAsLittleEndian(Type Value) {
			if (ENDIANNESS_BE)
				return SwapEndianness(Value);
			return Value;
		}

		// Converts a BE value to the native architecture or vice versa.
		template<typename Type>
		FORCE_INLINE Type InterpretAsBigEndian(Type Value) {
			if (ENDIANNESS_LE)
				return SwapEndianness(Value);
			return Value;
		}

		template<typename Type, const size_t Size=SIZEOF_BITS(Type)>
		CONST_EXPRESSION Type BinaryReflect(Type X) {
			Type Result = 0;
			// TODO: Room For Optimization.
			
			for (size_t i = 0; i < Size; i++)
#if PREFS_BINARYREFLECT == 1
				if (X & (Type(1) << i))
					Result |= (Type(1) << ((Size - Type(1)) - i));
#elif PREFS_BINARYREFLECT == 2
				Result |= (Type(1) << ((Size - Type(1)) - i)) * ((X & (Type(1) << i)) > 0);
#endif
			return Result;
		}

		template<typename MaskType, typename SizeType>
		CONST_EXPRESSION MaskType MakeBinaryMask(SizeType Size) {
			MaskType Output = 0;
			for (SizeType i = 0; i < Size; i++)
				Output = (Output << 1) + 1;
			return Output;
		}
	}
}
