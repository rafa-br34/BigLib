#pragma once

// STD
#include <iostream>
#include <fstream>
#include <string>

// Others
#include <Windows.h>
#include <winnt.h>



namespace Executables {

	class FileBuffer {
	public:
		void* AllocatedMemory = nullptr;
		size_t MemorySize = 0;

		std::fstream FileStream;

		bool LoadFile(std::string FileName) {
			if (this->FileStream.is_open()) {
				return false;
			}
			this->FileStream.open(FileName, std::ios::binary | std::ios::out | std::ios::in);


			auto Start = this->FileStream.tellg();
			this->FileStream.seekg(0, std::ios::end);
			auto End = this->FileStream.tellg();
			this->FileStream.seekg(0, std::ios::beg);

			this->MemorySize = (size_t)(End - Start);

			this->AllocatedMemory = new (std::nothrow) uint8_t[this->MemorySize];

			this->FileStream.read((char*)this->AllocatedMemory, this->MemorySize);

			return true;
		}
	};

	enum MachineType {
		X86 = 1,
		X86_X64
	};

	enum PEType {
		PE32 = 1,
		PE32_PLUS,
		ROM
	};


	class PortableExecutable {
	public:

		IMAGE_DOS_HEADER* DOSHeader = nullptr;
		IMAGE_FILE_HEADER* FileHeader = nullptr;

		IMAGE_NT_HEADERS32* NTHeaders32 = nullptr;
		IMAGE_OPTIONAL_HEADER32 OptionalHeader32 = {};

		IMAGE_NT_HEADERS64* NTHeaders64 = nullptr;
		IMAGE_OPTIONAL_HEADER64 OptionalHeader64 = {};

		IMAGE_SECTION_HEADER* SectionHeadersArray = nullptr;

		size_t NumberOfSections = 0;
		size_t SizeOfOptionalHeader = 0;

		MachineType FileType = (MachineType)0;
		PEType Type = (PEType)0;

		FileBuffer File = {};

		bool Parse(void* MemoryBase); // Parse Custom File
		bool Parse(); // Parse Loaded File
		bool LoadFile(std::string FilePath);

		void* RawOffsetToVirtual(uintptr_t Offset, bool AddImageBase=false);
		void* VirtualToRawOffset(uintptr_t VirtualOffset);

	private:
		IMAGE_NT_HEADERS* TempNTHeaders = nullptr;
	};

	const char* MachineTypeToText(uintptr_t MachineId);
}