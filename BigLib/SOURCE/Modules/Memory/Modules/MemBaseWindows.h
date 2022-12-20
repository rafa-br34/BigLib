#pragma once
#include "../../../Includes.h"
#include "../../Data/BaseMemory.h"



// Not So Efficient As Storing A Variable But Works
#define ASLR(Address)							((void*)((uint8_t*)GetModuleHandle(0) + Address))
#define MODULE_ASLR(Address, Module)			((void*)((uint8_t*)GetModuleHandle(Module) + Address))

#define RET_FAIL(Statement, Return) if (Statement) return Return;
#define RET_FAILEX(Statement, Extras, Return) if (Statement) { Extras; return Return; }

namespace BigLib {
	namespace Memory {
		extern HANDLE DefaultProcess;

		class MemoryProtection {
		public:
			uintptr_t	Address = 0;
			size_t		Size = 0;

			MemoryProtection()
				: Address()
				, Size()
			{}

			MemoryProtection(void* InitialAddress, size_t InitialSize)
				: Address((uintptr_t)InitialAddress)
				, Size(InitialSize)
			{}

			MemoryProtection(uintptr_t InitialAddress, size_t InitialSize)
				: Address(InitialAddress)
				, Size(InitialSize)
			{}



			INLINE bool SetProtection(DWORD NewProtection, HANDLE Process = DefaultProcess) {
				return VirtualProtectEx(Process, (LPVOID)this->Address, this->Size, NewProtection, &this->LastProtection);
			}

			INLINE bool OldProtection(HANDLE Process = DefaultProcess) {
				return VirtualProtectEx(Process, (LPVOID)this->Address, this->Size, this->LastProtection, &this->LastProtection);
			}

		private:
			DWORD LastProtection = 0;
		};


		INLINE bool PatchAddress(void* Address, uint8_t* Data, size_t Size, bool OverwriteProtection) {
			Memory::MemoryProtection Protect(Address, Size);

			if (OverwriteProtection)
				RET_FAIL(!Protect.SetProtection(PAGE_EXECUTE_READWRITE), false);

			RET_FAILEX(
				!WriteProcessMemory(DefaultProcess, Address, Data, Size, nullptr),
				Protect.OldProtection(),
				false
			);

			if (OverwriteProtection)
				RET_FAIL(!Protect.OldProtection(), false);

			return true;
		}

		INLINE bool ReadAddress(void* Address, uint8_t* Buffer, size_t Size, bool OverwriteProtection) {
			Memory::MemoryProtection Protect(Address, Size);

			if (OverwriteProtection)
				RET_FAIL(!Protect.SetProtection(PAGE_EXECUTE_READWRITE), false);

			RET_FAILEX(
				!ReadProcessMemory(DefaultProcess, Address, Buffer, Size, nullptr),
				Protect.OldProtection(),
				false
			);

			if (OverwriteProtection)
				RET_FAIL(!Protect.OldProtection(), false);

			return true;
		}

		INLINE uint8_t* ReadAddress(void* Address, size_t Size, bool OverwriteProtection) {
			Memory::MemoryProtection Protect(Address, Size);

			if (OverwriteProtection)
				RET_FAIL(!Protect.SetProtection(PAGE_EXECUTE_READWRITE), nullptr);

			uint8_t* NewMemory = ALLOCATE(uint8_t, Size);
			RET_FAILEX(
				!ReadProcessMemory(DefaultProcess, Address, NewMemory, Size, nullptr),
				FREE(NewMemory);  Protect.OldProtection(),
				nullptr
			);

			if (OverwriteProtection)
				RET_FAILEX(!Protect.OldProtection(), FREE(NewMemory), nullptr);

			return NewMemory;
		}

		template <typename Class>
		INLINE Class ReadValue(void* Address, bool OverwriteProtection) {
			Class Return = {};
			ReadAddress(Address, &Return, sizeof(Return), OverwriteProtection);
			return Return;
		}


		NO_INLINE bool IsAddressReadable(void* Address, size_t Size);
	}

}


#undef RET_FAILEX
#undef RET_FAIL