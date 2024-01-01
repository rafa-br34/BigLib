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

#define OFFSET(Address, Offset)	((uint8*)(Address) + (Offset)) // Offset memory location by Offset bytes
#define READ_AS(Type, Pointer)	((CONST Type*)Pointer) // Read memory as some constant Type pointer

namespace BigLib::Memory {

	template<typename IType = umax>
	MEM_INLINE void MemoryCopy(void* Target, CONST void* Source, IType Size) {
		// Set Memory Using Largest IType Possible
		for (IType i = 0; i < Size / sizeof(BIGGEST_DATATYPE); i++)
			((BIGGEST_DATATYPE*)Target)[i] = READ_AS(BIGGEST_DATATYPE, Source)[i];

		// Set Remaining Bytes
		CONST IType ExtraBytes = Size % sizeof(BIGGEST_DATATYPE);
		for (IType i = 0; i < ExtraBytes; i++) {
			IType TrueIType = (Size - ExtraBytes) + i;
			((uint8*)Target)[TrueIType] = READ_AS(uint8, Source)[TrueIType];
		}
	}

	template<typename IType = umax>
	MEM_INLINE bool MemoryCompare(CONST void* D1, CONST void* D2, IType Size) {
		if (D1 == D2) return true;

		// Check Memory Using Largest IType Possible
		for (IType i = 0; i < Size / sizeof(BIGGEST_DATATYPE); i++)
			if (READ_AS(BIGGEST_DATATYPE, D1)[i] != READ_AS(BIGGEST_DATATYPE, D2)[i])
				return false;

		// Check Remaining Bytes
		const IType ExtraBytes = Size % sizeof(BIGGEST_DATATYPE);
		for (IType i = 0; i < ExtraBytes; i++) {
			IType TrueIndex = (Size - ExtraBytes) + i;
			if (READ_AS(uint8, D1)[TrueIndex] != READ_AS(uint8, D2)[TrueIndex])
				return false;
		}
		return true;
	}

	template<typename Type = uint8, typename IType = umax>
	MEM_INLINE void MemoryFill(void* Target, Type Data, IType Size) {
		// Set Memory Using Largest Data Type Possible
		for (IType i = 0; i < Size; i++)
			((Type*)Target)[i] = Data;
	}

	template<typename IType = umax>
	MEM_INLINE void* GetMultiLevelPointer(void* Init, IType Offsets[], IType OffsetsSize) {
		auto Current = (uint8*)Init;

		for (IType i = 0; i < OffsetsSize; i++)
			Current = *(uint8**)(Current + Offsets[i]);

		return Current;
	}

	template<typename IType = umax>
	MEM_INLINE void* HookVTable(void* VTable, IType VTableSize) {
		auto NewVTable = ALLOCATE(uint8, VTableSize);

		MemoryCopy(NewVTable, *(void**)VTable, VTableSize);
		MemoryCopy(VTable, &NewVTable, sizeof(VTable));

		return (void*)NewVTable;
	}

	template<typename SizeType = umax, typename BufferType = uint8, CONST bool AutoDeallocate = true>
	struct MemoryBuffer {
		BufferType* Buffer = nullptr;
		SizeType Size = 0;

		void Deallocate() {
			if (this->Buffer) {
				FREE(this->Buffer);
				this->Size = 0;
				this->Buffer = nullptr;
			}
		}

		void Allocate(SizeType NewSize) {
			if (this->Size == NewSize) return;
			
			this->Deallocate();
			if (NewSize == 0) return;
			
			this->Buffer = (BufferType*)ALLOCATE(uint8, NewSize);
			this->Size = NewSize;
		}

		template<typename Type>
		Type Read(SizeType Offset = 0) {
			if (this->Size <= 0 || this->Buffer == nullptr) return;
			return *(Type*)OFFSET(this->Buffer, Offset);
		}

		template<typename Type>
		void Read(Type* Value, SizeType ReadSize, SizeType Offset = 0) {
			if (this->Size <= 0 || this->Buffer == nullptr) return;
			MemoryCopy(Value, this->Buffer, Math::Min(this->Size, ReadSize));
		}

		template<typename Type>
		void Write(Type Value, SizeType Offset = 0) {
			if (this->Size <= 0 || this->Buffer == nullptr) return;
			*(Type*)OFFSET(this->Buffer, Offset) = Value;
		}

		template<typename Type>
		void Write(CONST Type* Value, SizeType WriteSize, SizeType Offset = 0) {
			if (this->Size <= 0 || this->Buffer == nullptr) return;
			MemoryCopy(this->Buffer, Value, Math::Min(this->Size, WriteSize));
		}

		MemoryBuffer() = default;

		template<typename Type>
		MemoryBuffer(CONST Type* Value, SizeType ValueSize) {
			this->Allocate(ValueSize);
			this->Write(Value, ValueSize);
		}

		MemoryBuffer(CONST MemoryBuffer& From, bool CopyData = true) {
			this->Allocate(From.Size);
			this->Write(From.Buffer, From.Size);
		}

		explicit MemoryBuffer(SizeType InitialSize) {
			this->Allocate(InitialSize);
		}
		

		~MemoryBuffer() { if (AutoDeallocate) this->Deallocate(); }
	};
}

#undef MEM_INLINE