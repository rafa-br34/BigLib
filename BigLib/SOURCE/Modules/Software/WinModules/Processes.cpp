/*
* Name: Software Processes Library Source
* Desc: Software Sub Library For Processes Manipulation
* Status: Development In Progress
*/

#include "../WinSoftware.h"




static PEB* GetCurrentPEB() {
#ifdef PROGRAM_32
	return (PEB*)__readfsdword(0x30);
#else
	return (PEB*)__readgsqword(0x60);
#endif
}

#define UNLINK(Item)					\
	(Item).Flink->Blink = (Item).Blink;	\
	(Item).Blink->Flink = (Item).Flink;


namespace BigLib {
	namespace ReversingEngineering {
		namespace Software {
			bool Processes::UnlinkModuleFromPEB(const wchar_t* Name) {
				PROCESS_BASIC_INFORMATION ProcessBasicInfo{};
				ULONG Size{};


				typedef NTSTATUS(NTAPI* _NtQueryInformationProcess) (
					IN HANDLE ProcessHandle,
					IN PROCESSINFOCLASS ProcessInformationClass,
					OUT PVOID ProcessInformation,
					IN ULONG ProcessInformationLength,
					OUT PULONG ReturnLength OPTIONAL
					);

				//_NtQueryInformationProcess NtQueryInformationProcess = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationProcess");


				//NtQueryInformationProcess(GetCurrentProcess(), ProcessBasicInformation, &ProcessBasicInfo, sizeof(ProcessBasicInfo), &Size);

				PEB* ProcessPEB = GetCurrentPEB();

				typedef struct LDR_MODULE {
					LIST_ENTRY InLoadOrderModuleList;
					LIST_ENTRY InMemoryOrderModuleList;
					LIST_ENTRY InInitializationOrderModuleList;
					PVOID BaseAddress;
					PVOID EntryPoint;
					ULONG SizeOfImage;
					UNICODE_STRING FullDllName;
					UNICODE_STRING BaseDllName;
					ULONG Flags;
					SHORT LoadCount;
					SHORT TlsIndex;
					LIST_ENTRY HashTableEntry;
					ULONG TimeDateStamp;
				} LDR_MODULE, * PLDR_MODULE;


				for (auto Entry = ProcessPEB->Ldr->InMemoryOrderModuleList.Flink; Entry != &ProcessPEB->Ldr->InMemoryOrderModuleList; Entry = Entry->Flink) {
					LDR_DATA_TABLE_ENTRY* CurrentEntry = (LDR_DATA_TABLE_ENTRY*)Entry;

					if (wcscmp(CurrentEntry->FullDllName.Buffer, (const wchar_t*)Name) == 0) {
						UNLINK(((LDR_MODULE*)CurrentEntry)->InLoadOrderModuleList);
						UNLINK(((LDR_MODULE*)CurrentEntry)->InMemoryOrderModuleList);
						UNLINK(((LDR_MODULE*)CurrentEntry)->InInitializationOrderModuleList);
						UNLINK(((LDR_MODULE*)CurrentEntry)->HashTableEntry);
					}
				}


				return true;
			}

			bool Processes::Thread::SetAffinity(UINT64 AffinityMask) {
				return SetThreadAffinityMask(this->ThreadHandle, (DWORD_PTR)&AffinityMask);
			}


			bool Processes::Thread::SetPriority(int Priority) {
				return SetThreadPriority(this->ThreadHandle, Priority);
			}


			bool Processes::Thread::Suspend() {
				return SuspendThread(this->ThreadHandle);
			}


			bool Processes::Thread::Resume() {
				return ResumeThread(this->ThreadHandle);
			}


			Processes::Thread::Thread(DWORD TID, HANDLE Handle) {
				if (Handle == 0) {
					this->AllocatedHandle = true;
					this->ThreadHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, TID);
				}
				else {
					this->ThreadHandle = Handle;
				}

				if (TID == 0) {
					this->TID = GetThreadId(this->ThreadHandle);
				}
				else {
					this->TID = TID;
				}
			}


			void Processes::Thread::Delete() {
				if (this->AllocatedHandle) {
					CloseHandle(this->ThreadHandle);
				}
			}




			bool Processes::Process::UpdateThreads() {
				HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, this->PID);


				THREADENTRY32 ThreadEntry{};
				ThreadEntry.dwSize = sizeof(ThreadEntry);

				for (auto& ThreadObject : this->Threads) {
					ThreadObject.Delete();
				}
				this->Threads.clear();


				Thread32First(Snapshot, &ThreadEntry);

				do {
					HANDLE ThreadHandle = 0;

					if ((ThreadHandle = OpenThread(PROCESS_ALL_ACCESS, FALSE, ThreadEntry.th32ThreadID)) != 0) {

						auto ThreadObject = Processes::Thread(ThreadEntry.th32ThreadID, ThreadHandle);
						ThreadObject.AllocatedHandle = true;

						this->Threads.push_back(ThreadObject);

					}

				} while (Thread32Next(Snapshot, &ThreadEntry));


				CloseHandle(Snapshot);
				return true;
			}


			bool Processes::Process::ResumeThreads() {
				for (auto& ThreadObject : this->Threads) {
					ThreadObject.Resume();
				}
				return true;
			}


			bool Processes::Process::SuspendThreads() {
				for (auto& ThreadObject : this->Threads) {
					ThreadObject.Suspend();
				}
				return true;
			}
		}
	}
}
