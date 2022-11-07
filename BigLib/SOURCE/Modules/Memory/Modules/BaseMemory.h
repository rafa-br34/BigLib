#pragma once
#include "../../../Includes.h"


#ifdef MEMORY_BASE_UTILS_FORCEINLINE
#define MEM_INLINE FORCE_INLINE
#else
#define MEM_INLINE INLINE
#endif // MEMORY_BASE_UTILS_FORCEINLINE


namespace BigLib {
	namespace Memory {
		// Namespace With All Basic Memory Utilities
		namespace MemoryBase {
			MEM_INLINE void MemorySet(void* Target, void* Source, size_t Size) {
				const size_t ExtraBytes = Size % sizeof(BIGGEST_DATATYPE);

				// Set Memory Using Largest Type Possible
				for (size_t i = 0; i < Size / sizeof(BIGGEST_DATATYPE); i++) {
					((BIGGEST_DATATYPE*)Target)[i] = ((BIGGEST_DATATYPE*)Source)[i];
				}
				// Set Remaining
				for (size_t i = 0; i < ExtraBytes; i++) {
					size_t TrueIndex = (Size - ExtraBytes) + i;
					((uint8_t*)Target)[TrueIndex] = ((uint8_t*)Source)[TrueIndex];
				}
			}
			
			MEM_INLINE void MemoryFill(void* Target, uint8_t Source, size_t Size) {
				//typedef uint32_t MDT; // Mask DataType
				//MDT Mask = (Source << 24) | (Source << 16) | (Source << 8) | Source;


				for (size_t i = 0; i < Size; i++) {
					((uint8_t*)Target)[i] = Source;
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

				MemoryBase::MemorySet(NewVTable, *(void**)VTable, VTableByteSize);
				MemoryBase::MemorySet(VTable, &NewVTable, sizeof(VTable));

				return (uintptr_t*)NewVTable;
			}
		}
	}
}

#undef MEM_INLINE