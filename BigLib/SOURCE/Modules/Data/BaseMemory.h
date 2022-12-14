#pragma once
#include "../../Includes.h"
#include "BaseString.h"
#include "Bitwise.h"


#if APPROACH__MEMBASE_INLINE_TYPE == 1
#define MEM_INLINE FORCE_INLINE
#elif APPROACH__MEMBASE_INLINE_TYPE == 2
#define MEM_INLINE INLINE
#elif APPROACH__MEMBASE_INLINE_TYPE == 3
#define MEM_INLINE
#else
#error APPROACH__MEMBASE_INLINE_TYPE Invalid Value
#endif // APPROACH__MEMBASE_INLINE_TYPE


namespace BigLib {
	namespace Memory {
		MEM_INLINE void MemorySet(void* Target, void* Source, size_t Size) {
			// Set Memory Using Largest Type Possible
			for (size_t i = 0; i < Size / sizeof(BIGGEST_DATATYPE); i++) {
				((BIGGEST_DATATYPE*)Target)[i] = ((BIGGEST_DATATYPE*)Source)[i];
			}

			const size_t ExtraBytes = Size % sizeof(BIGGEST_DATATYPE);
			// Set Remaining
			for (size_t i = 0; i < ExtraBytes; i++) {
				size_t TrueIndex = (Size - ExtraBytes) + i;
				((uint8_t*)Target)[TrueIndex] = ((uint8_t*)Source)[TrueIndex];
			}
		}

		MEM_INLINE bool MemoryCompare(void* D1, void* D2, size_t Size) {
			for (size_t i = 0; i < Size; i++)
				if (((uint8_t*)D1)[i] != ((uint8_t*)D2)[i])
					return false;
			return true;
		}

		MEM_INLINE void MemoryFill(void* Target, uint8_t Data, size_t Size) {
			typedef uint32_t MDT; // Mask DataType
			const MDT Mask = (Data << 24) | (Data << 16) | (Data << 8) | Data;


			// Set Memory Using Largest Type Possible
			for (size_t i = 0; i < Size / sizeof(MDT); i++) {
				((MDT*)Target)[i] = Mask;
			}

			const size_t ExtraBytes = Size % sizeof(MDT);
			// Set Remaining
			for (size_t i = 0; i < Size; i++) {
				((uint8_t*)Target)[i] = Data;
			}
		}

		MEM_INLINE void* GetMultiLevelPointer(void* Init, size_t Offsets[], size_t OffsetsSize) {
			uint8_t* Current = (uint8_t*)Init;

			for (size_t i = 0; i < OffsetsSize; i++) {
				Current = *(uint8_t**)(Current + Offsets[i]);
			}

			return Current;
		}

		MEM_INLINE uintptr_t* HookVTable(void* VTable, size_t VTableByteSize) {
			uint8_t* NewVTable = ALLOCATE(uint8_t, VTableByteSize);

			MemorySet(NewVTable, *(void**)VTable, VTableByteSize);
			MemorySet(VTable, &NewVTable, sizeof(VTable));

			return (uintptr_t*)NewVTable;
		}
		

		// 8 Bits Used For The Actual Descriptor, 8 Bits For Wildcard Descriptor
		typedef uint16_t MemoryScanByte;

		// Makes A MemoryScanByte List From Characters, Use '?' For Wildcards
		// Ex: "81 F? ?0 D8 FF ?? 7E"
		//INLINE size_t MaskFromStringA(const char* String, MemoryScanByte* Output) {
		//	for (size_t i = 0; i < Strings::StringLength(String); i++) {
		//
		//	}
		//}
	}
}

#undef MEM_INLINE