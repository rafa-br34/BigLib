#include "AnalyserTarget.h"





bool AnalyserTarget::OpenFile(const char* FilePath) {
	if (this->LoadedFile.is_open()) {
		return false;
	}
	this->LoadedFile.open(FilePath, std::ios::binary | std::ios::out | std::ios::in);

	auto Start = this->LoadedFile.tellg();
	this->LoadedFile.seekg(0, std::ios::end);
	auto End = this->LoadedFile.tellg();
	this->LoadedFile.seekg(0, std::ios::beg);

	this->ProgramSize = (End - Start);
	return true;
}


bool AnalyserTarget::LoadFile() {
	BYTE* Buffer = new BYTE[(UINT)this->ProgramSize];
	this->LoadedFile.read((char*)Buffer, this->ProgramSize);

	// Headers
	this->ProgramNTHeaders = ImageNtHeader(Buffer);
	this->ProgramDOSHeader = (IMAGE_DOS_HEADER*)Buffer;

	// Sections
	IMAGE_SECTION_HEADER* SecArray = IMAGE_FIRST_SECTION(this->ProgramNTHeaders);
	this->NumberOfSections = this->ProgramNTHeaders->FileHeader.NumberOfSections;
	this->OffsetToSectionArray = (UINT)((BYTE*)SecArray - Buffer);
	this->SectionsArray = SecArray;


	this->ProgramData = Buffer;
	return true;
}


bool AnalyserTarget::Clear() {
	if (this->LoadedFile.is_open()) {
		this->LoadedFile.close();
	}
	delete[] this->ProgramData;
	this->ProgramData = nullptr;
	return false;
}


IMAGE_SECTION_HEADER AnalyserTarget::GetAddressSection(UINT64 Address) {
	IMAGE_SECTION_HEADER LastSection{};
	UINT CurrentAddress = 0;
	for (int i = 0; i < this->NumberOfSections; i++) {
		auto Section = this->SectionsArray[i];
		CurrentAddress += Section.PointerToRawData;
		if (CurrentAddress >= Address) {
			break;
		}
		LastSection = Section;
	}
	return LastSection;
}

UINT64 AnalyserTarget::RawAddressToBaseRelative(UINT64 Address) {
	auto Section = this->GetAddressSection(Address);
	return (Address - Section.PointerToRawData);
}