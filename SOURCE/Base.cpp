#include "Base.h"

namespace BigLib {
	AllocateMethod Allocate = nullptr;
	DeallocateMethod Free = nullptr;
}

STATIC uint16 X = 1;

bool ENDIANNESS_BE = ((uint8*)&X)[sizeof(X) - 1];
bool ENDIANNESS_LE = ((uint8*)&X)[0];