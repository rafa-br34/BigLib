#include <stdint.h>
#include <iostream>
#include <iomanip>
#include "../SOURCE/BigLib.h"
#include "Example.h"

size_t G_TOTAL_TESTS = 0;
size_t G_TOTAL_FAILS = 0;

template<typename Type>
void PrintList(Type* List, size_t ListSize, size_t Separate = 8, size_t Padding = (sizeof(Type) * 2)) {
	for (size_t i = 0; i < ListSize; i++) {
		if (i % Separate == 0)
			std::cout << '\n';

		std::cout << std::hex << "0x" << std::uppercase << std::setfill('0') << std::setw(Padding) << (size_t)(List[i]) << ", ";
	}
	std::cout << std::dec << '\n';
}

template<typename Type>
void HexPrint(Type* Data, size_t DataSize, bool NewLine=true, size_t Padding=(sizeof(Type) * 2)) {
	for (size_t i = 0; i < DataSize; i++) {
		std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(Padding) << (size_t)(Data[i]);
	}
	std::cout << std::dec;
	if (NewLine) std::cout << '\n';
}

template<typename Type>
void HexPrintInteger(Type Data, size_t Padding=(sizeof(Type) * 2)) {
	std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(Padding) << Data << std::dec;
}

double ByteCountToMeasurement(double Count, const char** Output) {
	const char* Measurements[] = {
		"B",
		"KB",
		"MB",
		"GB",
		"TB",
		"EB",
		"ZB",
		"YB",
		"RB",
		"QB",
		"UNK"
	};

	uint8_t Iteration = 0;
	while (Count >= 1024.0) {
		Count /= 1024.0;
		Iteration++;
	}
	

	if (const auto ListSize = sizeof(Measurements) / sizeof(void*); Iteration > ListSize)
		*Output = Measurements[ListSize - 1];
	else
		*Output = Measurements[Iteration];

	return Count;
}

bool _TEST_MemoryCompare(const void* A0, const void* A1, size_t Len) {
	for (size_t i = 0; i < Len; i++) {
		if (((const uint8_t*)A0)[i] != ((const uint8_t*)A1)[i])
			return false;
	}
	return true;
}

void _TEST_MemorySet(void* A0, uint8_t Byte, size_t Len) {
	for (size_t i = 0; i < Len; i++) {
		((uint8_t*)A0)[i] = Byte;
	}
}

float TEST_LIST() {
	size_t Tests = 0, Passed = 0;
	const char* TempString = nullptr;
	std::cout << "LIST TEST BEGIN\n";

	std::cout << "Starting Allocation Snap Test\n";
	{
		BigLib::Types::List<uint8_t> NewList;

		for (size_t i = 0, v = 0; i < 800; i++, v += 4) {
			if (i == 0) continue;
			size_t Allocated = (0x9BD18D18112681B8 * i) % (v + 1);

			NewList.ItemCountSnap = v;
			NewList.AllocateTotal(Allocated);
			size_t ModuloResult = NewList.AllocatedCount() % v;
			size_t Difference = NewList.AllocatedCount() - Allocated;

			std::cout << "IDX:" << i << " SNAP: " << v << " NEWSIZE:" << Allocated << " SIZE:" << NewList.AllocatedCount() << " DIFF:" << Difference << '(' << Difference / v << ')' << " MODRES:" << ModuloResult << "    ";
			if (ModuloResult == 0)
				Passed++;
			else
				std::cout << " Failed\n";
			Tests++;
			std::cout << "\r";

			NewList.AllocateTotal(0);
		}
		std::cout << '\n';
	}
	std::cout << "Finished Allocation Snap Test\n";

	std::cout << "Starting Initializers Test\n";
	{
		Tests++;
		const uint8_t TestBuffer[] = "\x42\x93\x68\x69\x55\x9E\x31\xC0\xCC\x18\x58\x36\xAD\xE0\x73\xEC\xDE\x74\xDA\x9B\x20\x4E\x19\xE7\xA6\x88\x7A\xFE\x7E\xAA\xA5\xCB\xE8\xF0\x36\x3C\x6F\xC1\x27\xAD\x2D\x6F\xE0\x3C\x59\x3F\x81\xE0\x0B\x6A";
		BigLib::Types::List<uint8_t> NewList(TestBuffer, sizeof(TestBuffer));
		if (!_TEST_MemoryCompare(NewList.AllocationPointer(), TestBuffer, sizeof(TestBuffer)))
			std::cout << "List Initializer Failed\n";
		else
			Passed++;
	}
	// TODO: Other Initializers



	std::cout << "LIST TEST END\n";

	return (float(Passed) / float(Tests)) * 100.f;
}


float TEST_MEM_UTILS() {
	std::cout << "MEMORY UTILITIES TEST BEGIN\n";
	bool CompleteFailure = false;
	size_t Tests = 0, Passed = 0;

	// MemoryCompare Test
	{
		const char* TestData = "01234567890123456789";
		const char* TestDataChunk = "0123456789\0\0\0\0\0\0\0\0\0\0"; // Padding Is Added So MemoryCompare Doesn't Access Data It Shouldn't

		bool TrueTestResults[] = {
			BigLib::Memory::MemoryCompare(TestData, TestData, 20), // Pointer Comparation
			BigLib::Memory::MemoryCompare(TestData, TestData + 10, 10), // Data Comparation First
			BigLib::Memory::MemoryCompare(TestData + 10, TestData, 10), // Data Comparation Second
		};
		bool FalseTestResults[] = {
			BigLib::Memory::MemoryCompare(TestData + 1, TestData, 20), // Unaligned Data First
			BigLib::Memory::MemoryCompare(TestData, TestData + 1, 20), // Unaligned Data Second
			BigLib::Memory::MemoryCompare(TestData, TestDataChunk, 20), // Invalid Data First
			BigLib::Memory::MemoryCompare(TestDataChunk, TestData, 20), // Invalid Data Second
		};

		std::cout << "Starting MemoryCompare Test\n";
		std::cout << "TRUE:\n";
		for (size_t i = 0; i < sizeof(TrueTestResults) / sizeof(bool); i++) {
			Tests++; G_TOTAL_TESTS++;
			if (TrueTestResults[i] != true) {
				std::cout << "\tTest " << i << " Failed\n";
				G_TOTAL_FAILS++;
			}
			else {
				std::cout << "\tPassed\n";
				Passed++;
			}
		}
		std::cout << "FALSE:\n";
		for (size_t i = 0; i < sizeof(FalseTestResults) / sizeof(bool); i++) {
			Tests++; G_TOTAL_TESTS++;
			if (FalseTestResults[i] != false) {
				std::cout << "\tTest " << i << " Failed\n";
				G_TOTAL_FAILS++;
			}
			else {
				std::cout << "\tPassed\n";
				Passed++;
			}
		}
	}


	const uint16_t BufferSize = 512;
	uint8_t ExpectationBuffer[BufferSize]{};
	uint8_t FilledBuffer[BufferSize]{};

	// MemoryFill Test
	{
		std::cout << "\nStarting MemoryFill Test\n";
		bool FailedOnce = false;
		for (size_t i = 1; i <= BufferSize; i++) {
			bool Passes = true;
			for (size_t b = 0; b < 255; b++) {
				// Clear And Fill Buffers
				for (size_t clr = 0; clr < BufferSize; clr++) {
					ExpectationBuffer[clr] = '\0';
					FilledBuffer[clr] = '\0';
				}
				for (size_t a = 0; a < i; a++) {
					ExpectationBuffer[a] = (uint8_t)b;
				}

				BigLib::Memory::MemoryFill(FilledBuffer, (uint8_t)b, i);
				if (_TEST_MemoryCompare(FilledBuffer, ExpectationBuffer, i) != true) {
					std::cout << "\tMemoryFill Failed Buffer Fill Size: " << i << " Byte: " << b << '\n';
					Passes = false;
					FailedOnce = true;
					break;
				}

			}
			Tests++;
			if (Passes) {
				Passed++;
				std::cout << "\tPassed i:" << i << '\r';
			}
		}
		G_TOTAL_TESTS++;
		if (FailedOnce) G_TOTAL_FAILS++;
		std::cout << '\n';
	}

	// MemoryCopy Test
	{
		std::cout << "\nStarting MemoryCopy Test\n";
		bool FailedOnce = false;
		for (size_t i = 1; i <= BufferSize; i++) {
			bool Passes = true;
			for (size_t b = 0; b < 255; b++) {
				// Clear And Fill Buffers
				for (size_t clr = 0; clr < BufferSize; clr++) {
					ExpectationBuffer[clr] = '\0';
					FilledBuffer[clr] = '\0';
				}
				for (size_t a = 0; a < i; a++) {
					ExpectationBuffer[a] = (uint8_t)((b + a) * size_t(0x2545F4914F6CDD1D)); // Adding Some Very Low Quality Randomness
				}

				BigLib::Memory::MemoryCopy(FilledBuffer, ExpectationBuffer, i);
				if (_TEST_MemoryCompare(FilledBuffer, ExpectationBuffer, i) != true) {
					std::cout << "\tMemorySet Failed Buffer Fill Size: " << i << " Expected: "; HexPrint(ExpectationBuffer, i, false); std::cout << " Got: "; HexPrint(FilledBuffer, i);
					Passes = false;
					FailedOnce = true;
					break;
				}

			}
			Tests++;
			if (Passes) {
				Passed++;
				std::cout << "\tPassed i:" << i << '\r';
			}
		}
		G_TOTAL_TESTS++;
		if (FailedOnce) G_TOTAL_FAILS++;
		std::cout << '\n';
	}



	std::cout << "MEMORY UTILITIES TEST FINISH\n\n";
	return CompleteFailure ? 0.f : (float(Passed) / float(Tests)) * 100.f;
}

float TEST_MATH() {
	std::cout << "MATH UTILITIES TEST BEGIN\n";
	size_t Tests = 0;
	size_t Fails = 0;

	// Test Modulo
	{
		std::cout << "\nStarting Integer Modulo Test\n";
		size_t TotalTests = 0;
		size_t FP32Fails = 0;
		size_t FP64Fails = 0;

		for (int M = -500; M < 500; M++) {
			if (M == 0) continue;
			if (FP32Fails + FP64Fails > 100) break;

			for (int X = -500; X < 500; X++) {
				TotalTests++;
				int IntModulo = (X % M);
				auto FP32Res = BigLib::Math::Modulo<float>((float)X, (float)M);
				auto FP64Res = BigLib::Math::Modulo<double>((double)X, (double)M);

				Tests += 2;
				if (!(FP32Res == (float)IntModulo)) {
					std::cout << X << '%' << M << " FP32 Failed, Expected: " << IntModulo << " Got: " << FP32Res << '\n';
					FP32Fails++; Fails++;
				}
				if (!(FP64Res == (double)IntModulo)) {
					std::cout << X << '%' << M << " FP64 Failed, Expected: " << IntModulo << " Got: " << FP64Res << '\n';
					FP64Fails++; Fails++;
				}

				if (FP32Fails + FP64Fails > 10) {
					std::cout << "Total Fails > 10, Ending Test.\n";
					break;
				}
			}
		}
		G_TOTAL_TESTS += 2;

		if (FP32Fails > TotalTests / 2) G_TOTAL_FAILS++;
		if (FP64Fails > TotalTests / 2) G_TOTAL_FAILS++;
	}

	std::cout << "MATH UTILITIES TEST FINISH\n\n";
	return (float(Tests - Fails) / float(Tests)) * 100.f;
}


size_t _TEST_CRC_I = 0;
size_t _TEST_CRC_PASS = 0;
template<
	typename Type,
	const Type Polynomial,

	const bool ReflectIn = false,
	const bool ReflectOut = false,
	const Type XORIn = 0,
	const Type XOROut = 0,

	const size_t Width = SIZEOF_BITS(Type),
	const size_t TableLen = 256
>
void _TEST_CRC(BigLib::DataIntegrity::CRC::CRCEngineStatic<Type, Polynomial, ReflectIn, ReflectOut, XORIn, XOROut, Width, TableLen>* CRCClass) {
	const char* Data = "123456789";
	size_t DataLen = 9;


	uint64_t Results[] = {
		0x4, // 3 GSM
		0x6, // 3 ROHC

		0x7, // 4 G-704
		0xB, // 4 INTERLAKEN

		0x00, // 5 EPC-C1G2
		0x07, // 5 G-704
		0x19, // 5 USB

		0x0D, // 6 CDMA2000-A
		0x3B, // 6 CDMA2000-B
		0x26, // 6 DARC
		0x06, // 6 G-704
		0x13, // 6 GSM

		0x75, // 7 MMC
		0x53, // 7 ROHC
		0x61, // 7 UMTS

		0xDA, // 8 CDMA2000
		0x25, // 8 WCDMA
		0xBC, // 8 DVB-S2
		0xA1, // 8 MAXIM-DOW
		0x15, // 8 DARC
		0x7E, // 8 ICODE
		0x97, // 8 TECH-3250
		0xD0, // 8 ROHC
		0xA1, // 8 I-432-1
		0xF4, // 8 SMBUS
		0x26, // 8 BLUETOOTH
		0xDF, // 8 AUTOSAR
		0x37, // 8 GSM-A
		0x94, // 8 GSM-B
		0xB4, // 8 HITAG
		0x99, // 8 MIFARE-MAD
		0xEA, // 8 LTE
		0xF7, // 8 NRSC-5
		0x3E, // 8 OPENSAFETY
		0x4B, // 8 SAE-J1850

		0x199, // 10 ATM
		0x233, // 10 CDMA2000
		0x12A, // 10 GSM

		0x5A3, // 11 FLEXRAY
		0x061, // 11 UMTS

		0xD4D, // 12 CDMA2000
		0xF5B, // 12 DECT
		0xB34, // 12 GSM
		0xDAF, // 12 UMTS

		0x04FA, // 13 BBC

		0x082D, // 14 DARC
		0x30AE, // 14 GSM

		0x059E, // 15 CAN
		0x2566, // 15 MPT1327

		0xBB3D, // 16 ARC
		0x4C06, // 16 CDMA2000
		0xAEE7, // 16 CMS
		0x9ECF, // 16 DDS 110
		0x007E, // 16 DECT R
		0x007F, // 16 DECT X
		0xEA82, // 16 DNP
		0xC2B7, // 16 EN 13757
		0xD64E, // 16 GENIBUS
		0xCE3C, // 16 GSM
		0x29B1, // 16 IBM 3740
		0x906E, // 16 IBM SDLC
		0xBF05, // 16 ISO-IEC-14443-3-A
		0x2189, // 16 KERMIT
		0xBDf4, // 16 LJ1200
		0x772B, // 16 M17
		0x44C2, // 16 MAXIM-DOW
		0x6F91, // 16 MCRF4XX
		0x4B37, // 16 MODBUS
		0xA066, // 16 NRSC-5
		0x5D38, // 16 OPENSAFETY-A
		0x20FE, // 16 OPENSAFETY-B
		0xA819, // 16 PROFIBUS
		0x63D0, // 16 RIELLO
		0xE5CC, // 16 SPI-FUJITSU
		0xD0DB, // 16 T10-DIF
		0x0FB3, // 16 TELEDISK
		0x26B1, // 16 TMS37157
		0xFEE8, // 16 UMTS
		0xB4C8, // 16 USB
		0x31C3, // 16 XMODEM

		0x04F03, // 17 CAN-FD

		0x0ED841, // 21 CAN-FD

		0xC25A56, // 24 BLE
		0x7979BD, // 24 FLEXRAY A
		0x1F23B8, // 24 FLEXRAY B
		0xB4F3E6, // 24 INTERLAKEN
		0xCDE703, // 24 LTE-A
		0x23EF52, // 24 LTE-B
		0x21CF02, // 24 OPENPGP
		0x200FA5, // 24 OS-9

		0x04C34ABF, // 30 CDMA
		
		0x0CE9E46C, // 31 PHILIPS

		0x3010BF7F, // 32 AIXM
		0x1697D06A, // 32 AUTOSAR
		0x87315576, // 32 BASE91-D
		0xFC891918, // 32 BZIP2
		0x6EC2EDC4, // 32 CD-ROM-EDC
		0x765E7680, // 32 CKSUM
		0xE3069283, // 32 ISCSI
		0xCBF43926, // 32 ISO-HDLC
		0x340BC6D9, // 32 JAMCRC
		0xD2C22F51, // 32 MEF
		0x0376E6E7, // 32 MPEG-2
		0xBD0BE338, // 32 XFER

		0xD4164FC646, // 40 GSM

		0x6C40DF5F0B497347, // 64 ECMA-182
		0xB90956C775A41001, // 64 GO-ISO
		0x75D4B74F024ECEEA, // 64 MS
		0xE9C6D914C4B8D9CA, // 64 REDIS
		0x62EC59E3F1A4F00A, // 64 WE
		0x995DC9BBDF1939FA, // 64 XZ
	};
	const char* AlgoNames[] = {
		"3 GSM",
		"3 ROHC",

		"4 G-704",
		"4 INTERLAKEN",

		"5 EPC-C1G2",
		"5 G-704",
		"5 USB",

		"6 CDMA2000-A",
		"6 CDMA2000-B",
		"6 DARC",
		"6 G-704",
		"6 GSM",

		"7 MMC",
		"7 ROHC",
		"7 UMTS",

		"8 CDMA2000",
		"8 WCDMA",
		"8 DVB-S2",
		"8 MAXIM-DOW",
		"8 DARC",
		"8 ICODE",
		"8 TECH-3250",
		"8 ROHC",
		"8 I-432-1",
		"8 SMBUS",
		"8 BLUETOOTH",
		"8 AUTOSAR",
		"8 GSM-A",
		"8 GSM-B",
		"8 HITAG",
		"8 MIFARE-MAD",
		"8 LTE",
		"8 NRSC-5",
		"8 OPENSAFETY",
		"8 SAE-J1850",

		"10 ATM",
		"10 CDMA2000",
		"10 GSM",
		
		"11 FLEXRAY",
		"11 UMTS",

		"12 CDMA2000",
		"12 DECT",
		"12 GSM",
		"12 UMTS",

		"13 BBC",

		"14 DARC",
		"14 GSM",

		"15 CAN",
		"15 MPT1327",

		"16 ARC",
		"16 CDMA2000",
		"16 CMS",
		"16 DDS 110",
		"16 DECT R",
		"16 DECT X",
		"16 DNP",
		"16 EN 13757",
		"16 GENIBUS",
		"16 GSM",
		"16 IBM 3740",
		"16 IBM SDLC",
		"16 ISO-IEC-14443-3-A",
		"16 KERMIT",
		"16 LJ1200",
		"16 M17",
		"16 MAXIM-DOW",
		"16 MCRF4XX",
		"16 MODBUS",
		"16 NRSC-5",
		"16 OPENSAFETY-A",
		"16 OPENSAFETY-B",
		"16 PROFIBUS",
		"16 RIELLO",
		"16 SPI-FUJITSU",
		"16 T10-DIF",
		"16 TELEDISK",
		"16 TMS37157",
		"16 UMTS",
		"16 USB",
		"16 XMODEM",

		"17 CAN-FD",

		"21 CAN-FD",

		"24 BLE",
		"24 FLEXRAY A",
		"24 FLEXRAY B",
		"24 INTERLAKEN",
		"24 LTE-A",
		"24 LTE-B",
		"24 OPENPGP",
		"24 OS-9",

		"30 CDMA",

		"31 PHILIPS",
		
		"32 AIXM",
		"32 AUTOSAR",
		"32 BASE91-D",
		"32 BZIP2",
		"32 CD-ROM-EDC",
		"32 CKSUM",
		"32 ISCSI",
		"32 ISO-HDLC",
		"32 JAMCRC",
		"32 MEF",
		"32 MPEG-2",
		"32 XFER",

		"40 GSM",

		"64 ECMA-182",
		"64 GO-ISO",
		"64 MS",
		"64 REDIS",
		"64 WE",
		"64 XZ"
	};


	CRCClass->UpdateCRC(Data, DataLen);
	auto CRC = (uint64_t)CRCClass->GetCRC();
	std::cout << "[(" << AlgoNames[_TEST_CRC_I] << "[" << _TEST_CRC_I << "]) => "; HexPrintInteger(CRC); std::cout << ']';
	if (CRC != Results[_TEST_CRC_I]) {
		std::cout << " Failed, Expected: ";  HexPrintInteger(Results[_TEST_CRC_I]); std::cout << '\n';
		PrintList(CRCClass->GetLookupTable(), TableLen);
		G_TOTAL_FAILS++;
	}
	else {
		std::cout << " Passed\n";
		_TEST_CRC_PASS++;
	}

	_TEST_CRC_I++;
	G_TOTAL_TESTS++;

	delete[] CRCClass;
}

float TEST_CRCs() {
	std::cout << "CRC TEST BEGIN\n";
	_TEST_CRC_I = 0;
	_TEST_CRC_PASS = 0;
	// Not Optimal But Will Do The Job.
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_3_GSM());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_3_ROHC());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_4_G_704());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_4_INTERLAKEN());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_5_EPC_C1G2());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_5_G_704());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_5_USB());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_6_CDMA2000_A());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_6_CDMA2000_B());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_6_DARC());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_6_G_704());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_6_GSM());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_7_MMC());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_7_ROHC());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_7_UMTS());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_CDMA2000());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_WCDMA());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_DVB_S2());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_MAXIM_DOW());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_DARC());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_I_CODE());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_TECH_3250());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_ROHC());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_I_432_1());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_SMBUS());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_BLUETOOTH());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_AUTOSAR());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_GSM_A());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_GSM_B());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_HITAG());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_MIFARE_MAD());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_LTE());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_NRSC_5());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_OPENSAFETY());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_SAE_J1850());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_10_ATM());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_10_CDMA2000());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_10_GSM());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_11_FLEXRAY());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_11_UMTS());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_12_CDMA2000());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_12_DECT());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_12_GSM());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_12_UMTS());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_13_BBC());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_14_DARC());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_14_GSM());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_15_CAN());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_15_MPT1327());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_ARC());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_CDMA2000());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_CMS());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_DDS_110());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_DECT_R());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_DECT_X());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_DNP());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_EN_13757());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_GENIBUS());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_GSM());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_IBM_3740());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_IBM_SDLC());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_ISO_IEC_14443_3_A());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_KERMIT());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_LJ1200());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_M17());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_MAXIM_DOW());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_MCRF4XX());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_MODBUS());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_NRSC_5());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_OPENSAFETY_A());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_OPENSAFETY_B());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_PROFIBUS());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_RIELLO());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_SPI_FUJITSU());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_T10_DIF());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_TELEDISK());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_TMS37157());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_UMTS());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_USB());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_XMODEM());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_17_CAN_FD());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_21_CAN_FD());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_BLE());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_FLEXRAY_A());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_FLEXRAY_B());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_INTERLAKEN());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_LTE_A());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_LTE_B());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_OPENPGP());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_OS_9());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_30_CDMA());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_31_PHILIPS());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_AIXM());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_AUTOSAR());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_BASE91_D());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_BZIP2());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_CD_ROM_EDC());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_CKSUM());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_ISCSI());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_ISO_HDLC());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_JAMCRC());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_MEF());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_MPEG_2());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_XFER());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_40_GSM());

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_ECMA_182());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_GO_ISO());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_MS());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_REDIS());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_WE());
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_XZ());

	if (_TEST_CRC_PASS != _TEST_CRC_I)
		std::cout << _TEST_CRC_PASS << " Out Of " << _TEST_CRC_I << " CRCs Passed" << std::endl;
	else
		std::cout << "All " << _TEST_CRC_PASS << " CRCs Passed" << std::endl;

	std::cout << "CRC TEST FINISH\n\n";
	return ((float)_TEST_CRC_PASS / (float)_TEST_CRC_I) * 100.f;
}

float TEST_MD2_6() {
	std::cout << "MD2-6 TEST BEGIN\n";

	const uint8_t AvailableTests = 3;

	const uint8_t* TestStrings[] = {
		(const uint8_t*)"",
		(const uint8_t*)"0123456789ABCDEF",
		(const uint8_t*)"abcdefghijklmnopqrstuvwxyz"
	};

	const uint8_t* ExpectationsMD2[] = {
		(const uint8_t*)"\x83\x50\xE5\xA3\xE2\x4C\x15\x3D\xF2\x27\x5C\x9F\x80\x69\x27\x73",
		(const uint8_t*)"\xC3\x1D\x79\x45\xAA\xFB\x1D\x69\x48\x20\xB7\x1A\xA7\xEA\xE7\x2B",
		(const uint8_t*)"\x4E\x8D\xDF\xF3\x65\x02\x92\xAB\x5A\x41\x08\xC3\xAA\x47\x94\x0B"
	};
	const uint8_t* ExpectationsMD4[] = {
		(const uint8_t*)"\x31\xD6\xCF\xE0\xD1\x6A\xE9\x31\xB7\x3C\x59\xD7\xE0\xC0\x89\xC0",
		(const uint8_t*)"\x02\x4B\x50\x28\x7D\x63\xEB\x62\xDC\x29\xB4\x7C\x6A\x9C\xE0\x03",
		(const uint8_t*)"\xD7\x9E\x1C\x30\x8A\xA5\xBB\xCD\xEE\xA8\xED\x63\xDF\x41\x2D\xA9"
	};
	

	uint8_t Tests = 0;
	uint8_t Failed = 0;
	const uint8_t* MDResult;

	// MD2
	auto MD2 = BigLib::DataIntegrity::MD2_6::MD2();
	auto MD4 = BigLib::DataIntegrity::MD2_6::MD4();
	const uint8_t* Expectation = nullptr;
	const uint8_t* String = nullptr;

	for (uint8_t i = 0; i < AvailableTests; i++) {
		String = TestStrings[i];

		Expectation = ExpectationsMD2[i];
		MDResult = MD2.Update(String, BigLib::Strings::StringLength(String)).Finalize(); Tests++;
		if (!BigLib::Memory::MemoryCompare(MDResult, Expectation, 16)) {
			std::cout << "MD2 String(" << (const char*)String << ") Failed\n\tValue: "; HexPrint(MDResult, 16); std::cout << "\tExpected : "; HexPrint(Expectation, 16);
			Failed++;
			G_TOTAL_FAILS++;
		}
		else {
			std::cout << "MD2 String IDX:" << (int)i << " Passed.\n";
		}
		MD2.Reset();
	}
	
	G_TOTAL_TESTS += AvailableTests;


	std::cout << "MD2-6 TEST FINISH\n\n";
	return (float(Tests - Failed) / (float)Tests) * 100.f;
}



void LIB_TEST() {
	float Stability[] = {
		TEST_LIST(),
		TEST_MATH(),
		TEST_MEM_UTILS(),
		TEST_CRCs(),
		TEST_MD2_6(),
	};
	size_t TestCount = sizeof(Stability) / sizeof(float);

	size_t Fails = 0;
	size_t Unsuccessful = 0;
	for (size_t i = 0; i < TestCount; i++) {
		if (Stability[i] == 0.f) Fails++;
		else if (Stability[i] != 100.f) Unsuccessful++;
	}
	std::cout << "\n\n\n----------------TEST RESULTS----------------";
	
	// Compile Time
	{
		std::cout << "\n\n----Compile Time----\n";
		std::cout << "Target Architecture: " << BigLib::Architectures::ArchitectureName[BigLib::c_MachineArchitecture] << " (IDX:" << BigLib::c_MachineArchitecture << ")\n";
		std::cout << "Compiler Data Types:";
		std::cout
			<< "\n\tLP32: " << DT_LP32
			<< "\n\tILP32: " << DT_ILP32
			<< "\n\tLP64: " << DT_LP64
			<< "\n\tLLP64: " << DT_LLP64
			<< "\n\tILP64: " << DT_ILP64
			<< "\n\tSILP64: " << DT_SILP64
			<< "\n";

		std::cout << "Required Virtualizations:";
		std::cout
			<< "\n\tDT8: " << VIRTUALIZE_DT8
			<< "\n\tDT16: " << VIRTUALIZE_DT16
			<< "\n\tDT32: " << VIRTUALIZE_DT32
			<< "\n\tDT64: " << VIRTUALIZE_DT64
			<< "\n";
	}

	// Results
	{
		std::cout << "\n\n----Results----\n";
		std::cout << "Ran " << TestCount << " Tests";
		if (Fails != 0)
			std::cout << " " << Fails << " Tests Completely Failed";
		if (Unsuccessful != 0)
			std::cout << " " << Unsuccessful << " Tests Were Not 100% Successful.";

		std::cout << "\nTest Percentage Average: " << BigLib::Math::Average(Stability, TestCount) << "%\n";
		std::cout << "Library Stability(" << (G_TOTAL_TESTS - G_TOTAL_FAILS) << '/' << G_TOTAL_TESTS << " Tests Passed): " << (float(G_TOTAL_TESTS - G_TOTAL_FAILS) / (float)G_TOTAL_TESTS) * 100.f << "%\n";
	}
}

#if EXAMPLE_SELECTOR == 0
int main() {
	LIB_TEST();
}
#endif