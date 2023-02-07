#pragma once
#include "../../Includes.h"

#define BIT(Index)								(UI64(1) << (Index))
#define SIZEOF_BITS(Value)							(sizeof(Value) * 8)
#define OFFSET(Address, Offset)						((uint8_t*)(Address) + (Offset))
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
		CONST_EXPRESSION FORCE_INLINE Type RotateRight(Type X, RType R) {
			return (X >> R) | (X << (-R & (SIZEOF_BITS(Type) - 1)));
		}

		template<typename Type, typename RType=unsigned>
		CONST_EXPRESSION FORCE_INLINE Type RotateLeft(Type X, RType R) {
			return (X << R) | (X >> (-R & (SIZEOF_BITS(Type) - 1)));
		}


		template<typename Type, const size_t Size=SIZEOF_BITS(Type)>
		CONST_EXPRESSION FORCE_INLINE Type BinaryReflect(Type X) {
			Type Result = 0;
			// TODO: Room For Optimization.
			
			for (size_t i = 0; i < Size; i++)
#if APPROACH__BINARYREFLECT == 1
				if (X & (Type(1) << i)) Result |= (Type(1) << ((Size - Type(1)) - i));
#elif APPROACH__BINARYREFLECT == 2
				Result |= (Type(1) << ((Size - Type(1)) - i)) * ((X & (Type(1) << i)) > 0);
#endif
			return Result;
		}

		template<typename Type>
		CONST_EXPRESSION FORCE_INLINE Type MakeBinaryMask(size_t Size) {
			Type Output = 0;
			for (size_t i = 0; i < Size; i++)
				Output = (Output << 1) + 1;
			return Output;
		}
	}
}
