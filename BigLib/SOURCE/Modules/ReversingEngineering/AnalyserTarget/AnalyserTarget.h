#pragma once
#include <Windows.h>
#include <DbgHelp.h>
#include <iostream>
#include <fstream>

#pragma comment(lib, "DbgHelp.lib")


class AnalyserTarget {
public:


	UINT64 ProgramSize = 0;
	BYTE* ProgramData = nullptr;

	WORD NumberOfSections = 0;
	UINT OffsetToSectionArray = 0;
	IMAGE_SECTION_HEADER* SectionsArray = nullptr;
	
	IMAGE_NT_HEADERS* ProgramNTHeaders = nullptr;
	IMAGE_DOS_HEADER* ProgramDOSHeader = nullptr;
	
	std::fstream LoadedFile;

	IMAGE_SECTION_HEADER GetAddressSection(UINT64 Address);
	UINT64 RawAddressToBaseRelative(UINT64 Address);
	bool OpenFile(const char* FilePath);
	bool LoadFile();
	bool Clear();
};

