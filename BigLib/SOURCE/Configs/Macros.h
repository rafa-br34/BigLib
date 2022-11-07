#pragma once

#define DEBUG_PRINT(Printer) Printer

// Replace These To Custom Allocate/Deallocate Methods
#define ALLOCATE(Type, Amount) new Type[Amount]
#define FREE(Pointer) delete[] Pointer

#define OFFSET(Address, Offset)			((uint8_t*)(Address) + (Offset))
#define HAS_BIT(Value, Index)			((Value) & BIT(Index))
#define GET_BIT(Value, Index)			(HAS_BIT(Value, Index) >> (Index))
#define SET_BIT(Value, Index, NewBit)	(GET_BIT(Value, Index) == (BitValue) ? (Value) : (Value) ^ BIT(Index))

#define BIT_MASK(Size)                          (0xFF >> (8 - (Size)))
#define GET_BITCHUNK(Value, StartIndex, Size)   (((Value) >> ((StartIndex) - 1)) & BIT_MASK(Size))


#define BIGGEST_DATATYPE long long
