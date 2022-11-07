#include "Executables.h"


bool Executables::PortableExecutable::LoadFile(std::string FilePath) {
	return this->File.LoadFile(FilePath);
}

bool Executables::PortableExecutable::Parse(void* MemoryBase) {
	if (*(uint16_t*)MemoryBase != IMAGE_DOS_SIGNATURE) {
		return false; // Invalid Signature
	}
	this->DOSHeader = (IMAGE_DOS_HEADER*)MemoryBase;
	this->TempNTHeaders = (IMAGE_NT_HEADERS*)((uint8_t*)MemoryBase + this->DOSHeader->e_lfanew);

	if (this->TempNTHeaders->Signature != IMAGE_NT_SIGNATURE) {
		return false; // Invalid Signature
	}

	this->FileHeader = &this->TempNTHeaders->FileHeader;
	
	switch (this->FileHeader->Machine) {
		case IMAGE_FILE_MACHINE_I386: {
			this->NTHeaders32 = (IMAGE_NT_HEADERS32*)this->TempNTHeaders;
			this->FileType = MachineType::X86;
			this->OptionalHeader32 = this->NTHeaders32->OptionalHeader;
			this->SectionHeadersArray = IMAGE_FIRST_SECTION(this->NTHeaders32);

			switch (this->OptionalHeader32.Magic) {
				case IMAGE_NT_OPTIONAL_HDR32_MAGIC: {
					this->Type = PEType::PE32;
					break;
				}
				case IMAGE_NT_OPTIONAL_HDR64_MAGIC: {
					this->Type = PEType::PE32_PLUS;
					break;
				}
				case IMAGE_ROM_OPTIONAL_HDR_MAGIC: {
					this->Type = PEType::ROM;
					break;
				}
			}


			break;
		}

		case IMAGE_FILE_MACHINE_AMD64: {
			this->NTHeaders64 = (IMAGE_NT_HEADERS64*)this->TempNTHeaders;
			this->FileType = MachineType::X86_X64;
			this->OptionalHeader64 = this->NTHeaders64->OptionalHeader;
			this->SectionHeadersArray = IMAGE_FIRST_SECTION(this->NTHeaders64);

			switch (this->OptionalHeader64.Magic) {
				case IMAGE_NT_OPTIONAL_HDR32_MAGIC: {
					this->Type = PEType::PE32;
					break;
				}
				case IMAGE_NT_OPTIONAL_HDR64_MAGIC: {
					this->Type = PEType::PE32_PLUS;
					break;
				}
				case IMAGE_ROM_OPTIONAL_HDR_MAGIC: {
					this->Type = PEType::ROM;
					break;
				}
			}

			break;
		}
	}



	this->NumberOfSections = this->FileHeader->NumberOfSections;
	this->SizeOfOptionalHeader = this->FileHeader->SizeOfOptionalHeader;
	
	return true;
}

bool Executables::PortableExecutable::Parse() {
	if (this->File.MemorySize <= 0) {
		return false;
	}
	return this->Parse(this->File.AllocatedMemory);
}


void* Executables::PortableExecutable::RawOffsetToVirtual(uintptr_t Offset, bool AddImageBase) {
	for (uintptr_t i = 0; i < this->NumberOfSections; i++) {
		auto Section = this->SectionHeadersArray[i];

		if (Section.PointerToRawData <= Offset && Offset < (uintptr_t)(Section.PointerToRawData + Section.SizeOfRawData)) {
			
			auto RVA = (Offset - Section.PointerToRawData) + Section.VirtualAddress;

			auto ImageBase = (this->Type == Executables::PEType::PE32) ? this->OptionalHeader32.ImageBase : this->OptionalHeader64.ImageBase;


			return (void*)(AddImageBase ? RVA + (ImageBase) : RVA);
		}
	}

	return nullptr;
}

void* Executables::PortableExecutable::VirtualToRawOffset(uintptr_t VirtualOffset) {
	for (uintptr_t i = 0; i < this->NumberOfSections; i++) {
		auto Section = this->SectionHeadersArray[i];
		if (Section.VirtualAddress <= VirtualOffset && VirtualOffset < (Section.VirtualAddress + Section.SizeOfRawData)) {
			return ((uint8_t*)VirtualOffset - Section.VirtualAddress) + Section.PointerToRawData;
		}
	}

	return nullptr;
}

const char* Executables::MachineTypeToText(uintptr_t MachineId) {
	switch (MachineId) {
		case IMAGE_FILE_MACHINE_UNKNOWN:
			return "IMAGE_FILE_MACHINE_UNKNOWN";
		case IMAGE_FILE_MACHINE_TARGET_HOST:
			return "IMAGE_FILE_MACHINE_TARGET_HOST";
		case IMAGE_FILE_MACHINE_I386:
			return "IMAGE_FILE_MACHINE_I386";
		case IMAGE_FILE_MACHINE_R3000:
			return "IMAGE_FILE_MACHINE_R3000";
		case IMAGE_FILE_MACHINE_R4000:
			return "IMAGE_FILE_MACHINE_R4000";
		case IMAGE_FILE_MACHINE_R10000:
			return "IMAGE_FILE_MACHINE_R10000";
		case IMAGE_FILE_MACHINE_WCEMIPSV2:
			return "IMAGE_FILE_MACHINE_WCEMIPSV2";
		case IMAGE_FILE_MACHINE_ALPHA:
			return "IMAGE_FILE_MACHINE_ALPHA";
		case IMAGE_FILE_MACHINE_SH3:
			return "IMAGE_FILE_MACHINE_SH3";
		case IMAGE_FILE_MACHINE_SH3DSP:
			return "IMAGE_FILE_MACHINE_SH3DSP";
		case IMAGE_FILE_MACHINE_SH3E:
			return "IMAGE_FILE_MACHINE_SH3E";
		case IMAGE_FILE_MACHINE_SH4:
			return "IMAGE_FILE_MACHINE_SH4";
		case IMAGE_FILE_MACHINE_SH5:
			return "IMAGE_FILE_MACHINE_SH5";
		case IMAGE_FILE_MACHINE_ARM:
			return "IMAGE_FILE_MACHINE_ARM";
		case IMAGE_FILE_MACHINE_THUMB:
			return "IMAGE_FILE_MACHINE_THUMB";
		case IMAGE_FILE_MACHINE_ARMNT:
			return "IMAGE_FILE_MACHINE_ARMNT";
		case IMAGE_FILE_MACHINE_AM33:
			return "IMAGE_FILE_MACHINE_AM33";
		case IMAGE_FILE_MACHINE_POWERPC:
			return "IMAGE_FILE_MACHINE_POWERPC";
		case IMAGE_FILE_MACHINE_POWERPCFP:
			return "IMAGE_FILE_MACHINE_POWERPCFP";
		case IMAGE_FILE_MACHINE_IA64:
			return "IMAGE_FILE_MACHINE_IA64";
		case IMAGE_FILE_MACHINE_MIPS16:
			return "IMAGE_FILE_MACHINE_MIPS16";
		case IMAGE_FILE_MACHINE_ALPHA64:
			return "IMAGE_FILE_MACHINE_ALPHA64";
		case IMAGE_FILE_MACHINE_MIPSFPU:
			return "IMAGE_FILE_MACHINE_MIPSFPU";
		case IMAGE_FILE_MACHINE_MIPSFPU16:
			return "IMAGE_FILE_MACHINE_MIPSFPU16";
		case IMAGE_FILE_MACHINE_TRICORE:
			return "IMAGE_FILE_MACHINE_TRICORE";
		case IMAGE_FILE_MACHINE_CEF:
			return "IMAGE_FILE_MACHINE_CEF";
		case IMAGE_FILE_MACHINE_EBC:
			return "IMAGE_FILE_MACHINE_EBC";
		case IMAGE_FILE_MACHINE_AMD64:
			return "IMAGE_FILE_MACHINE_AMD64";
		case IMAGE_FILE_MACHINE_M32R:
			return "IMAGE_FILE_MACHINE_M32R";
		case IMAGE_FILE_MACHINE_ARM64:
			return "IMAGE_FILE_MACHINE_ARM64";
		case IMAGE_FILE_MACHINE_CEE:
			return "IMAGE_FILE_MACHINE_CEE";

		default:
			return "UNKNOWN";
	}



}