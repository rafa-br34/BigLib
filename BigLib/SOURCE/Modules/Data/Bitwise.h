#pragma once
#include "../../Includes.h"

#define BIT(Index)						(1 << (Index))
#define SIZEOF_BITS(Value)				(sizeof(Value) * 8)
#define OFFSET(Address, Offset)			((uint8_t*)(Address) + (Offset))
#define HAS_BIT(Value, Index)			((Value) & BIT(Index))
#define GET_BIT(Value, Index)			(HAS_BIT(Value, Index) >> (Index))
#define SET_BIT(Value, Index, NewBit)	(GET_BIT(Value, Index) == (BitValue) ? (Value) : (Value) ^ BIT(Index))
#define LOWEST_BITS(Value, Lowest)		(((Value) << (SIZEOF_BITS(Value) - (Lowest))) >> (SIZEOF_BITS(Value) - (Lowest)))


#define BIT_MASK(Size)                          (0xFF >> (8 - (Size)))
#define GET_BITCHUNK(Value, StartIndex, Size)   (((Value) >> ((StartIndex) - 1)) & BIT_MASK(Size))

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
			// TODO: Alot Of Room For Optimization.
			
#if APPROACH__BINARYREFLECT == 1 || APPROACH__BINARYREFLECT == 2
			for (size_t i = 0; i < Size; i++)
	#if APPROACH__BINARYREFLECT == 1
				if (X & (Type(1) << i)) Result |= (Type(1) << ((Size - Type(1)) - i));
	#elif APPROACH__BINARYREFLECT == 2
				Result |= (Type(1) << ((Size - Type(1)) - i)) * ((X & (Type(1) << i)) > 0);
	#endif
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
