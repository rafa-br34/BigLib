/*
* Name: Software Library
* Desc: Software Library For Software Manipulation In Windows
* Status: Work In Progress
*/

#pragma once
#include "../../../Includes.h"
#include "../../Logging/Console.h"






namespace BigLib {
	namespace ReversingEngineering {
		namespace Software {
			namespace Memory {
				class TrampolineHook {
				public:
					void* Gateway = nullptr;
					void* Target = nullptr;
					size_t PaddingSize = 0;

					// WARNING: Requires The Thread To Be Locked Or The Program Might Crash
					bool Unhook(bool OverwritePageProtection = true);

					void* Hook32(void* TargetFunction, void* NewFunction, size_t Padding = 5);
					void* Hook64(void* TargetFunction, void* NewFunction, size_t Padding = 9);

#if defined(PROGRAM_64)
					inline bool Hook(void* TargetFunction, void* NewFunction, size_t Padding = 9) { return Hook64(TargetFunction, NewFunction, Padding); }
#else
					inline bool Hook(void* TargetFunction, void* NewFunction, size_t Padding = 5) { return Hook32(TargetFunction, NewFunction, Padding); }
#endif
				};


				bool Detour32(void* FlowStart, void* FlowTarget, size_t PadSize = 5);
				bool Detour64(void* FlowStart, void* FlowTarget, size_t PadSize = 9);

#if defined(PROGRAM_64)
				inline bool Detour(void* FlowStart, void* FlowTarget, size_t PadSize = 9) { return Detour64(FlowStart, FlowTarget, PadSize); }
#else
				inline bool Detour(void* FlowStart, void* FlowTarget, size_t PadSize = 5) { return Detour32(FlowStart, FlowTarget, PadSize); }
#endif

				void* HookFunction32(void* Target, void* NewFunction, size_t PaddingSize = 5);
				void* HookFunction64(void* Target, void* NewFunction, size_t PaddingSize = 9);

#if defined(PROGRAM_64)
				inline bool HookFunction(void* Target, void* NewFunction, size_t PaddingSize = 9) { return HookFunction64(Target, NewFunction, PaddingSize); }
#else
				inline bool HookFunction(void* Target, void* NewFunction, size_t PaddingSize = 5) { return HookFunction32(Target, NewFunction, PaddingSize); }
#endif
			}

			namespace Memory {
				

				bool IsAddressReadable(void* Address, size_t Size);

				/// Hooks A VTable With A Copied One, Returns The New Copied VTable
				uintptr_t* HookVTable(void* ClassVTable, size_t VTableByteSize);

				/// Gets A Multi Level Pointer
				void* GetMultiLevelPointer(void* Init, const std::vector<size_t>& Offsets);

				/// Checks If The String Is Fully Readable
				bool IsReadableString(void* Address);

				size_t GetAllPagesSize(size_t From, size_t End = UINT_MAX);

				enum MemoryScanMode {
					PATTERN_MATCH,
					PATTERN_CLOSEMATCH
				};

				class MemoryScan {
				public:
					const char* Pattern = "";

					// Scan Info
					UINT64 ScanInit = 0x0000;
					UINT64 ScanEnd = Software::Memory::GetAllPagesSize(0);
					std::vector<BYTE*>* Storage;
					UINT MaxItems = UINT_MAX;

					// Threading
					UINT MaxThreads = 0;
					int ThreadsPriority = THREAD_PRIORITY_NORMAL;
				};


				UINT64 MemoryPatternScan(MemoryScan* ScanInfo);

			}


			namespace Processes {
				bool UnlinkModuleFromPEB(const wchar_t* ModuleName);
				//bool UnlinkModuleFromPEB(const void* ModuleBase);


				class Thread {
				public:
					DWORD TID = 0;
					HANDLE ThreadHandle = 0;
					bool AllocatedHandle = false;

					bool SetAffinity(UINT64 AffinityMask);
					bool SetPriority(int Priority);
					bool Suspend();
					bool Resume();


					Thread(DWORD TID, HANDLE Handle);
					void Delete();
				};





				class Process {
				public:
					DWORD PID = 0;
					HANDLE ProcessHandle = nullptr;

					std::vector<Thread> Threads{};


					bool SuspendThreads();
					bool ResumeThreads();
					bool UpdateThreads();
				};



			}
		}
	}

}




