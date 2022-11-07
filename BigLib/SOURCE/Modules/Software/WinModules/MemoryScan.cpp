#include "../WinSoftware.h"

#define OFFSET(Address, Offset)	((uint8_t*)(Address) + (Offset))

/// Pattern To Byte Array, Dont Forget To Deallocate Result
static INT16* s_PatternToByteArray(const char* Pattern, UINT* BufferSize) {
	std::vector<INT16> PatternBuffer{};

	char StringBytesBuffer[3];
	UINT StringBytesBufferIndex = 0;

	for (UINT i = 0; i < strlen(Pattern); i++) {
		BYTE CharByte = (BYTE)tolower((int)Pattern[i]);

		// Check If Is Valid Hex Character
		if (CharByte == '?' || (CharByte >= 48 && CharByte <= 57) || (CharByte >= 97 && CharByte <= 102)) {
			StringBytesBuffer[StringBytesBufferIndex] = CharByte;
			StringBytesBufferIndex++;

			// Just Do Needed Checks, Even If This Function Only Runs Once
			if (StringBytesBufferIndex >= 2) {
				StringBytesBuffer[2] = '\0'; // Byte 3 Always Needs To Be 0
				// To Hex Value And Push Result

				if (strcmp(StringBytesBuffer, "??") == 0) {
					PatternBuffer.push_back(-1i16);
				}
				else {
					PatternBuffer.push_back((INT16)strtol(StringBytesBuffer, nullptr, 16));
				}

				StringBytesBufferIndex = 0;
			}
		}
	}

	INT16* ResultBuffer = nullptr;

	if ((*BufferSize = PatternBuffer.size()) != 0) {
		ResultBuffer = new (std::nothrow) INT16[PatternBuffer.size()];
		for (UINT i = 0; i < PatternBuffer.size(); i++) {
			ResultBuffer[i] = PatternBuffer[i];
		}
	}

	return ResultBuffer;
}

FORCE_INLINE static UINT64 s_MatchPattern(BYTE* Start, UINT64 MatchSize, INT16* Pattern, UINT64 PatternSize) {
	UINT64 Ending = (UINT64)(Start + MatchSize);

	UINT64 ByteOffset = 0;
	UINT64 OffsetStart = 0;

	for (UINT64 Current = (UINT64)Start; Current < Ending; Current++) {
		BYTE CurrentByte = *(BYTE*)Current;

		if (Pattern[ByteOffset] == -1i16 || CurrentByte == Pattern[ByteOffset]) {
			if (!OffsetStart) {
				OffsetStart = Current;
			}

			ByteOffset++;

			if (ByteOffset >= PatternSize) {
				return OffsetStart;
			}
		}
		else {
			if (OffsetStart) {
				OffsetStart = 0;
			}
			break;
		}
	}

	return 0;
}

FORCE_INLINE static bool s_UnsafeScanMemoryBlock(void* Start, void* End, INT16* Pattern, UINT64 PatternSize, std::vector<BYTE*>* Storage, std::mutex* VectorMutex, UINT64* CountingVar, UINT MaxItems) {
	UINT64 ScanStart = (UINT64)Start;
	UINT64 ScanEnd = (UINT64)End;

	for (UINT64 Current = ScanStart; Current < (ScanEnd - PatternSize); Current++) {
		UINT64 FoundAt = s_MatchPattern((BYTE*)Current, PatternSize, Pattern, PatternSize);

		if (FoundAt) {
			if (VectorMutex != nullptr)
				VectorMutex->lock();

			if (Storage->size() < MaxItems) {
				Storage->push_back((BYTE*)FoundAt);
				(*CountingVar)++;
			}
			else {
				return false;
			}

			if (VectorMutex != nullptr)
				VectorMutex->unlock();
		}
	}

	return true;
}

static bool s_SafeScanMemory(UINT64 ScanStart, UINT64 ScanEnd, INT16* PatternBytes, UINT PatternSize, std::vector<BYTE*>* Storage, std::mutex* VectorMutex, UINT64* FoundVars, UINT MaxItems) {
	UINT64 NextMeasure = 0x0000ui64;
	MEMORY_BASIC_INFORMATION MemBasicInfo = {};

	// We Need To Align The Starting Address
	if (ScanStart > 0) {
		SIZE_T ReadedBytes = VirtualQuery(
			(LPCVOID)ScanStart,
			&MemBasicInfo,
			sizeof(MemBasicInfo)
		);

		if (ReadedBytes == 0) {
			return false;
		}

		ScanStart = (UINT64)MemBasicInfo.BaseAddress;
	}

	for (UINT64 Location = ScanStart; Location < ScanEnd; Location++) {
		if (NextMeasure == 0x0000ui64) {
			SIZE_T ReadedBytes = VirtualQuery(
				(LPCVOID)Location,
				&MemBasicInfo,
				sizeof(MemBasicInfo)
			);

			if (ReadedBytes == 0) {
				return false;
			}

			// If Bad Page, Skip To Next Page And Re-Verify
			if (MemBasicInfo.State != MEM_COMMIT || MemBasicInfo.Protect == PAGE_NOACCESS || MemBasicInfo.Protect & PAGE_GUARD || MemBasicInfo.Protect == 0) {
				Location += MemBasicInfo.RegionSize - 1; // - 1 Since it will iterate again
				continue;
			}
			else {
				// Else Set Next Measure To The Page Region Size
				NextMeasure = MemBasicInfo.RegionSize;
			}
		}

		// We Can Use Unsafe Scan Here Since We Are Sure The Page Is Readable
		bool Result = s_UnsafeScanMemoryBlock(
			(void*)Location,
			(void*)(Location + (NextMeasure - 0)),
			PatternBytes,
			PatternSize,
			Storage,
			VectorMutex,
			FoundVars,
			MaxItems
		);

		if (!Result) {
			return false;
		}

		// Set Location To Next Page Since We Scanned The Entire Page Block
		Location += (NextMeasure - 1);
		NextMeasure = 0;

		if (NextMeasure > 0ui64) {
			NextMeasure--;
		}
	}

	return true;
}


namespace BigLib {
	namespace ReversingEngineering {
		namespace Software {
			UINT64 Memory::MemoryPatternScan(MemoryScan* ScanInfo) {
				UINT64 ScanStart = ScanInfo->ScanInit;
				UINT64 ScanEnd = ScanInfo->ScanEnd;

				// Convert Pattern To Byte Array
				UINT PatternSize = 0;
				INT16* PatternBytes = s_PatternToByteArray(ScanInfo->Pattern, &PatternSize);

				// Return Info
				UINT64 ItemsFound = 0;

				if (PatternSize == 0) {
					return 0;
				}

				if (ScanInfo->MaxThreads > 0) {
					UINT64 ScanSize = (ScanEnd - ScanStart);
					UINT64 ChunkSizes = ScanSize / ScanInfo->MaxThreads;
					UINT64 Remainder = ScanSize % ScanInfo->MaxThreads;

					if (Remainder != 0) {
						ChunkSizes += Remainder;
					}

					std::condition_variable Condition{};
					std::mutex Mutex{};

					std::mutex ListLock{};

					std::atomic<UINT64> AtomicFoundVars = 0;
					std::atomic<UINT> RunningThreads = 0;
					std::atomic<bool> IsThreaded = false;

					for (UINT i = 0; i < ScanInfo->MaxThreads; i++) {
						std::thread(
							[=, &AtomicFoundVars = AtomicFoundVars, &ScanInfo = ScanInfo, &FoundVars = ItemsFound, &ListLock = ListLock, &RunningThreads = RunningThreads, &Condition = Condition, &IsThreaded = IsThreaded]() {
								SetThreadPriority(GetCurrentThread(), ScanInfo->ThreadsPriority);
								RunningThreads++;
								IsThreaded = true;

								auto ScanFrom = ScanStart + (ChunkSizes * i);
								auto ScanTo = ScanFrom + ChunkSizes;

								UINT64 Found = 0;

								s_SafeScanMemory(
									ScanFrom,
									ScanTo,
									PatternBytes,
									PatternSize,
									ScanInfo->Storage,
									&ListLock,
									&Found,
									ScanInfo->MaxItems
								);
								AtomicFoundVars += Found;

								RunningThreads--;
								Condition.notify_one();
							}
						).detach();
					}

					std::unique_lock<std::mutex> Lock(Mutex);

					while (RunningThreads > 0 || !IsThreaded) {
						Condition.wait(Lock);
					}
				}
				else {
					SetThreadPriority(GetCurrentThread(), ScanInfo->ThreadsPriority);
					s_SafeScanMemory(
						ScanInfo->ScanInit,
						ScanInfo->ScanEnd,
						PatternBytes,
						PatternSize,
						ScanInfo->Storage,
						nullptr,
						&ItemsFound,
						ScanInfo->MaxItems
					);
				}

				delete[] PatternBytes;
				return ItemsFound;
			}



			bool Memory::IsAddressReadable(void* Address, size_t Size) {
				return s_IsPageReadable(Address, Size);
			}

			uintptr_t* Memory::HookVTable(void* VTable, size_t VTableByteSize) {
				uint8_t* NewVTable = new (std::nothrow) uint8_t[VTableByteSize];

				memcpy_s(NewVTable, VTableByteSize, *(void**)VTable, VTableByteSize);
				memcpy_s(VTable, sizeof(VTable), &NewVTable, sizeof(VTable));

				return (uintptr_t*)NewVTable;
			}

			bool Memory::IsReadableString(void* Address) {
				MEMORY_BASIC_INFORMATION MemBasicInfo = {};
				BYTE* Current = (BYTE*)Address;

				while (true) {
					// TODO: Make This Better And More Eficient
					VirtualQuery(
						(LPCVOID)Current,
						&MemBasicInfo,
						sizeof(MemBasicInfo)
					);

					if (MemBasicInfo.State != MEM_COMMIT || MemBasicInfo.Protect == PAGE_NOACCESS || (MemBasicInfo.Protect & PAGE_GUARD) > 0) {
						return false;
					}
					if (*Current == '\0') {
						break;
					}
					Current++;
				}

				return true;
			}

			size_t Memory::GetAllPagesSize(size_t From, size_t End) {
				size_t TotalSize = 0;

				MEMORY_BASIC_INFORMATION MemBasicInfo = {};

				BYTE* CurrentPage = (BYTE*)From;
				BYTE* FinalPage = (BYTE*)End;

				while (CurrentPage < FinalPage) {
					auto Return = VirtualQuery(
						(LPCVOID)CurrentPage,
						&MemBasicInfo,
						sizeof(MemBasicInfo)
					);

					if (Return == 0) {
						break;
					}
					if (MemBasicInfo.State == MEM_COMMIT && MemBasicInfo.Protect != PAGE_NOACCESS) {
						TotalSize += MemBasicInfo.RegionSize;
					}
					CurrentPage += MemBasicInfo.RegionSize + 1;
				}

				return TotalSize;
			}
		}
	}
}


