#include "MemBaseWindows.h"


namespace BigLib {
	namespace Memory {
		HANDLE DefaultProcess = GetCurrentProcess();

		NO_INLINE bool IsAddressReadable(void* Address, size_t Size) {
			MEMORY_BASIC_INFORMATION MemBasicInfo = {};
			SIZE_T ReadedBytes = VirtualQuery(
				(LPCVOID)Address,
				&MemBasicInfo,
				sizeof(MemBasicInfo)
			);


			if (ReadedBytes == 0 || MemBasicInfo.State != MEM_COMMIT || MemBasicInfo.Protect == PAGE_NOACCESS || MemBasicInfo.Protect & PAGE_GUARD || MemBasicInfo.Protect == 0)
				return false;

			auto ReadEnd = OFFSET(Address, Size);
			auto PageEnd = OFFSET(MemBasicInfo.BaseAddress, MemBasicInfo.RegionSize);

			// Use Recursion To Check Next Page If Needed
			if (ReadEnd > PageEnd)
				return IsAddressReadable(PageEnd, (size_t)ReadEnd);

			return true;
		}
	}
}
