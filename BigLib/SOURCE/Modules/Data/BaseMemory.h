#pragma once
#include "../../Includes.h"
#include "BaseString.h"
#include "Bitwise.h"


#if PREFS_MEMBASE_INLINE_TYPE == 1
#define MEM_INLINE FORCE_INLINE
#elif PREFS_MEMBASE_INLINE_TYPE == 2
#define MEM_INLINE INLINE
#elif PREFS_MEMBASE_INLINE_TYPE == 3
#define MEM_INLINE
#else
#error PREFS_MEMBASE_INLINE_TYPE Invalid Value
#define MEM_INLINE
#endif // PREFS_MEMBASE_INLINE_TYPE

#define OFFSET(Address, Offset)						((uint8*)(Address) + (Offset))
#define READ_AS(Type, Pointer)						((CONST Type*)Pointer)

namespace BigLib::Memory {
	MEM_INLINE void MemoryCopy(void* Target, CONST void* Source, umax Size) {
		// Set Memory Using Largest Type Possible
		for (umax i = 0; i < Size / sizeof(BIGGEST_DATATYPE); i++) {
			((BIGGEST_DATATYPE*)Target)[i] = READ_AS(BIGGEST_DATATYPE, Source)[i];
		}

		// Set Remaining Bytes
		CONST umax ExtraBytes = Size % sizeof(BIGGEST_DATATYPE);
		for (umax i = 0; i < ExtraBytes; i++) {
			umax TrueIndex = (Size - ExtraBytes) + i;
			((uint8*)Target)[TrueIndex] = READ_AS(uint8, Source)[TrueIndex];
		}
	}

	MEM_INLINE bool MemoryCompare(CONST void* D1, CONST void* D2, umax Size) {
		if (D1 == D2) return true;

		// Check Memory Using Largest Type Possible
		for (umax i = 0; i < Size / sizeof(BIGGEST_DATATYPE); i++) {
			if (READ_AS(BIGGEST_DATATYPE, D1)[i] != READ_AS(BIGGEST_DATATYPE, D2)[i])
				return false;
		}

		// Check Remaining Bytes
		const umax ExtraBytes = Size % sizeof(BIGGEST_DATATYPE);
		for (umax i = 0; i < ExtraBytes; i++) {
			umax TrueIndex = (Size - ExtraBytes) + i;
			if (READ_AS(uint8, D1)[TrueIndex] != READ_AS(uint8, D2)[TrueIndex])
				return false;
		}
		return true;
	}

	MEM_INLINE void MemoryFill(void* Target, uint8 Data, umax Size) {
		using MDT = uint32; // Mask DataType
		const MDT Mask = (Data << 24) | (Data << 16) | (Data << 8) | Data;

		// Set Memory Using Largest Type Possible
		for (umax i = 0; i < Size / sizeof(MDT); i++) {
			((MDT*)Target)[i] = Mask;
		}

		// Set Remaining Bytes
		const umax ExtraBytes = Size % sizeof(MDT);
		for (umax i = 0; i < ExtraBytes; i++) {
			((uint8*)Target)[(Size - ExtraBytes) + i] = Data;
		}
	}

	MEM_INLINE void* GetMultiLevelPointer(void* Init, umax Offsets[], umax OffsetsSize) {
		auto Current = (uint8*)Init;

		for (umax i = 0; i < OffsetsSize; i++) {
			Current = *(uint8**)(Current + Offsets[i]);
		}

		return Current;
	}

	MEM_INLINE void* HookVTable(void* VTable, umax VTableByteSize) {
		auto NewVTable = ALLOCATE(uint8, VTableByteSize);

		MemoryCopy(NewVTable, *(void**)VTable, VTableByteSize);
		MemoryCopy(VTable, &NewVTable, sizeof(VTable));

		return (void*)NewVTable;
	}

	template<typename SizeType = umax, typename BufferType = uint8, CONST bool AutoDeallocate = true>
	struct MemoryBufferCustom {
		BufferType* Buffer = nullptr;
		SizeType BufferSize = 0;

		void Deallocate() {
			if (this->Buffer) {
				FREE(this->Buffer);
				this->BufferSize = 0;
				this->Buffer = nullptr;
			}
		}

		void Allocate(SizeType Size) {
			if (this->BufferSize == Size) return;
			this->Deallocate();

			if (Size == 0) return;
			this->BufferSize = Size;
			this->Buffer = (BufferType*)ALLOCATE(uint8, Size);
		}

		~MemoryBufferCustom() { if (AutoDeallocate) this->Deallocate(); }
	};

	using MemoryBuffer = MemoryBufferCustom<umax>;
}

#undef MEM_INLINE