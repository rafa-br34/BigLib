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
		MEM_INLINE void MemorySet(void* Target, const void* Source, size_t Size) {
			// Set Memory Using Largest Type Possible
			for (size_t i = 0; i < Size / sizeof(BIGGEST_DATATYPE); i++) {
				((BIGGEST_DATATYPE*)Target)[i] = ((const BIGGEST_DATATYPE*)Source)[i];
			}

			// Set Remaining Bytes
			const size_t ExtraBytes = Size % sizeof(BIGGEST_DATATYPE);
			for (size_t i = 0; i < ExtraBytes; i++) {
				size_t TrueIndex = (Size - ExtraBytes) + i;
				((uint8_t*)Target)[TrueIndex] = ((const uint8_t*)Source)[TrueIndex];
			}
		}

		MEM_INLINE bool MemoryCompare(const void* D1, const void* D2, size_t Size) {
			if (D1 == D2) return true;

			// Check Memory Using Largest Type Possible
			for (size_t i = 0; i < Size / sizeof(BIGGEST_DATATYPE); i++) {
				if (((const BIGGEST_DATATYPE*)D1)[i] != ((const BIGGEST_DATATYPE*)D2)[i])
					return false;
			}

			// Check Remaining Bytes
			const size_t ExtraBytes = Size % sizeof(BIGGEST_DATATYPE);
			for (size_t i = 0; i < ExtraBytes; i++) {
				size_t TrueIndex = (Size - ExtraBytes) + i;
				if (((const uint8_t*)D1)[TrueIndex] != ((const uint8_t*)D2)[TrueIndex])
					return false;
			}
			return true;
		}

		MEM_INLINE void MemoryFill(void* Target, uint8_t Data, size_t Size) {
			typedef uint32_t MDT; // Mask DataType
			const MDT Mask = (Data << 24) | (Data << 16) | (Data << 8) | Data;


			// Set Memory Using Largest Type Possible
			for (size_t i = 0; i < Size / sizeof(MDT); i++) {
				((MDT*)Target)[i] = Mask;
			}

			// Set Remaining Bytes
			const size_t ExtraBytes = Size % sizeof(MDT);
			for (size_t i = 0; i < ExtraBytes; i++) {
				((uint8_t*)Target)[(Size - ExtraBytes) + i] = Data;
			}
		}

		MEM_INLINE void* GetMultiLevelPointer(void* Init, size_t Offsets[], size_t OffsetsSize) {
			auto Current = (uint8_t*)Init;

			for (size_t i = 0; i < OffsetsSize; i++) {
				Current = *(uint8_t**)(Current + Offsets[i]);
			}

			return Current;
		}

		MEM_INLINE uintptr_t* HookVTable(void* VTable, size_t VTableByteSize) {
			auto NewVTable = ALLOCATE(uint8_t, VTableByteSize);

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