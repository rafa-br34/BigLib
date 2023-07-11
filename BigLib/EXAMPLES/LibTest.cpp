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

			std::cout
				<< "IDX:" << i
				<< " SNAP: " << v
				<< " NEWSIZE:" << Allocated
				<< " SIZE:" << NewList.AllocatedCount()
				<< " DIFF:" << Difference << '(' << Difference / v << ')'
				<< " MODRES:" << ModuloResult
				<< "    ";

			if (ModuloResult == 0)
				Passed++;
			else
				std::cout << " Failed\n";
			Tests++;
			std::cout << "\r";

			NewList.AllocateTotal(0); // Reallocate to zero items, otherwise the current allocation could mess the next test iteration.
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

	BigLib::Types::List<uint8_t> NewList();

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
void _TEST_CRC(BigLib::DataIntegrity::CRC::CRCEngineStatic<Type, Polynomial, ReflectIn, ReflectOut, XORIn, XOROut, Width, TableLen>* CRCClass, const char* AlgoName, uint64 ExpectedResult) {
	const char* Data = "123456789";
	size_t DataLen = 9;

	CRCClass->Update(Data, DataLen);
	auto CRC = (uint64_t)CRCClass->Get();
	std::cout << "[(" << AlgoName << "[" << _TEST_CRC_I << "]) => "; HexPrintInteger(CRC); std::cout << ']';
	if (CRC != ExpectedResult) {
		std::cout << " Failed, Expected: ";  HexPrintInteger(ExpectedResult); std::cout << '\n';
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
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_3_GSM(), "3 GSM", 0x4);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_3_ROHC(), "3 ROHC", 0x6);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_4_G_704(), "4 G-704", 0x7);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_4_INTERLAKEN(), "4 INTERLAKEN", 0xB);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_5_EPC_C1G2(), "5 EPC-C1G2", 0x00);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_5_G_704(), "5 G-704", 0x07);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_5_USB(), "5 USB", 0x19);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_6_CDMA2000_A(), "6 CDMA2000-A", 0x0D);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_6_CDMA2000_B(), "6 CDMA2000-B", 0x3B);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_6_DARC(), "6 DARC", 0x26);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_6_G_704(), "6 G-704", 0x06);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_6_GSM(), "6 GSM", 0x13);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_7_MMC(), "7 MMC", 0x75);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_7_ROHC(), "7 ROHC", 0x53);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_7_UMTS(), "7 UMTS", 0x61);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_CDMA2000(), "8 CDMA2000", 0xDA);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_WCDMA(), "8 WCDMA", 0x25);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_DVB_S2(), "8 DVB-S2", 0xBC);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_MAXIM_DOW(), "8 MAXIM-DOW", 0xA1);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_DARC(), "8 DARC", 0x15);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_I_CODE(), "8 ICODE", 0x7E);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_TECH_3250(), "8 TECH-3250", 0x97);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_ROHC(), "8 ROHC", 0xD0);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_I_432_1(), "8 I-432-1", 0xA1);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_SMBUS(), "8 SMBUS", 0xF4);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_BLUETOOTH(), "8 BLUETOOTH", 0x26);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_AUTOSAR(), "8 AUTOSAR", 0xDF);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_GSM_A(), "8 GSM-A", 0x37);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_GSM_B(), "8 GSM-B", 0x94);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_HITAG(), "8 HITAG", 0xB4);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_MIFARE_MAD(), "8 MIFARE-MAD", 0x99);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_LTE(), "8 LTE", 0xEA);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_NRSC_5(), "8 NRSC-5", 0xF7);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_OPENSAFETY(), "8 OPENSAFETY", 0x3E);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_8_SAE_J1850(), "8 SAE-J1850", 0x4B);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_10_ATM(), "10 ATM", 0x199);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_10_CDMA2000(), "10 CDMA2000", 0x233);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_10_GSM(), "10 GSM", 0x12A);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_11_FLEXRAY(), "11 FLEXRAY", 0x5A3);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_11_UMTS(), "11 UMTS", 0x061);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_12_CDMA2000(), "12 CDMA2000", 0xD4D);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_12_DECT(), "12 DECT", 0xF5B);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_12_GSM(), "12 GSM", 0xB34);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_12_UMTS(), "12 UMTS", 0xDAF);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_13_BBC(), "13 BBC", 0x04FA);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_14_DARC(), "14 DARC", 0x082D);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_14_GSM(), "14 GSM", 0x30AE);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_15_CAN(), "15 CAN", 0x059E);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_15_MPT1327(), "15 MPT1327", 0x2566);
	
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_ARC(), "16 ARC", 0xBB3D);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_CDMA2000(), "16 CDMA2000", 0x4C06);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_CMS(), "16 CMS", 0xAEE7);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_DDS_110(), "16 DDS 110", 0x9ECF);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_DECT_R(), "16 DECT R", 0x007E);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_DECT_X(), "16 DECT X", 0x007F);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_DNP(), "16 DNP", 0xEA82);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_EN_13757(), "16 EN 13757", 0xC2B7);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_GENIBUS(), "16 GENIBUS", 0xD64E);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_GSM(), "16 GSM", 0xCE3C);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_IBM_3740(), "16 IBM 3740", 0x29B1);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_IBM_SDLC(), "16 IBM SDLC", 0x906E);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_ISO_IEC_14443_3_A(), "16 ISO-IEC-14443-3-A", 0xBF05);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_KERMIT(), "16 KERMIT", 0x2189);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_LJ1200(), "16 LJ1200", 0xBDF4);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_M17(), "16 M17", 0x772B);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_MAXIM_DOW(), "16 MAXIM-DOW", 0x44C2);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_MCRF4XX(), "16 MCRF4XX", 0x6F91);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_MODBUS(), "16 MODBUS", 0x4B37);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_NRSC_5(), "16 NRSC-5", 0xA066);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_OPENSAFETY_A(), "16 OPENSAFETY-A", 0x5D38);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_OPENSAFETY_B(), "16 OPENSAFETY-B", 0x20FE);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_PROFIBUS(), "16 PROFIBUS", 0xA819);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_RIELLO(), "16 RIELLO", 0x63D0);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_SPI_FUJITSU(), "16 SPI-FUJITSU", 0xE5CC);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_T10_DIF(), "16 T10-DIF", 0xD0DB);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_TELEDISK(), "16 TELEDISK", 0x0FB3);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_TMS37157(), "16 TMS37157", 0x26B1);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_UMTS(), "16 UMTS", 0xFEE8);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_USB(), "16 USB", 0xB4C8);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_16_XMODEM(), "16 XMODEM", 0x31C3);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_17_CAN_FD(), "17 CAN-FD", 0x04F03);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_21_CAN_FD(), "21 CAN-FD", 0x0ED841);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_BLE(), "24 BLE", 0xC25A56);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_FLEXRAY_A(), "24 FLEXRAY A", 0x7979BD);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_FLEXRAY_B(), "24 FLEXRAY B", 0x1F23B8);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_INTERLAKEN(), "24 INTERLAKEN", 0xB4F3E6);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_LTE_A(), "24 LTE-A", 0xCDE703);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_LTE_B(), "24 LTE-B", 0x23EF52);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_OPENPGP(), "24 OPENPGP", 0x21CF02);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_24_OS_9(), "24 OS-9", 0x200FA5);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_30_CDMA(), "30 CDMA", 0x04C34ABF);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_31_PHILIPS(), "31 PHILIPS", 0x0CE9E46C);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_AIXM(), "32 AIXM", 0x3010BF7F);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_AUTOSAR(), "32 AUTOSAR", 0x1697D06A);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_BASE91_D(), "32 BASE91-D", 0x87315576);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_BZIP2(), "32 BZIP2", 0xFC891918);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_CD_ROM_EDC(), "32 CD-ROM-EDC", 0x6EC2EDC4);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_CKSUM(), "32 CKSUM", 0x765E7680);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_ISCSI(), "32 ISCSI", 0xE3069283);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_ISO_HDLC(), "32 ISO-HDLC", 0xCBF43926);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_JAMCRC(), "32 JAMCRC", 0x340BC6D9);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_MEF(), "32 MEF", 0xD2C22F51);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_MPEG_2(), "32 MPEG-2", 0x0376E6E7);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_32_XFER(), "32 XFER", 0xBD0BE338);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_40_GSM(), "40 GSM", 0xD4164FC646);

	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_ECMA_182(), "64 ECMA-182", 0x6C40DF5F0B497347);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_GO_ISO(), "64 GO-ISO", 0xB90956C775A41001);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_MS(), "64 MS", 0x75D4B74F024ECEEA);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_REDIS(), "64 REDIS", 0xE9C6D914C4B8D9CA);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_WE(), "64 WE", 0x62EC59E3F1A4F00A);
	_TEST_CRC(new BigLib::DataIntegrity::CRC::CRC_64_XZ(), "64 XZ", 0x995DC9BBDF1939FA);



	if (_TEST_CRC_PASS != _TEST_CRC_I)
		std::cout << _TEST_CRC_PASS << " Out Of " << _TEST_CRC_I << " CRCs Passed" << std::endl;
	else
		std::cout << "All " << _TEST_CRC_PASS << " CRCs Passed" << std::endl;

	std::cout << "CRC TEST FINISH\n\n";
	return ((float)_TEST_CRC_PASS / (float)_TEST_CRC_I) * 100.f;
}


float TEST_MD2_6() {
	std::cout << "MD2-6 TEST BEGIN\n";

	const uint8_t AvailableTests = 7;

	const char* TestStrings[] = {
		"",
		"a",
		"abc",
		"message digest",
		"abcdefghijklmnopqrstuvwxyz",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		"12345678901234567890123456789012345678901234567890123456789012345678901234567890"
	};

	const char* ExpectationsMD2[] = {
		"\x83\x50\xE5\xA3\xE2\x4C\x15\x3D\xF2\x27\x5C\x9F\x80\x69\x27\x73",
		"\x32\xEC\x01\xEC\x4A\x6D\xAC\x72\xC0\xAB\x96\xFB\x34\xC0\xB5\xD1",
		"\xDA\x85\x3B\x0D\x3F\x88\xD9\x9B\x30\x28\x3A\x69\xE6\xDE\xD6\xBB",
		"\xAB\x4F\x49\x6B\xFB\x2A\x53\x0B\x21\x9F\xF3\x30\x31\xFE\x06\xB0",
		"\x4E\x8D\xDF\xF3\x65\x02\x92\xAB\x5A\x41\x08\xC3\xAA\x47\x94\x0B",
		"\xDA\x33\xDE\xF2\xA4\x2D\xF1\x39\x75\x35\x28\x46\xC3\x03\x38\xCD",
		"\xD5\x97\x6F\x79\xD8\x3D\x3A\x0D\xC9\x80\x6C\x3C\x66\xF3\xEF\xD8"
	};
	const char* ExpectationsMD4[] = {
		"\x31\xD6\xCF\xE0\xD1\x6A\xE9\x31\xB7\x3C\x59\xD7\xE0\xC0\x89\xC0",
		"\xBD\xE5\x2C\xB3\x1D\xE3\x3E\x46\x24\x5E\x05\xFB\xDB\xD6\xFB\x24",
		"\xA4\x48\x01\x7A\xAF\x21\xD8\x52\x5F\xC1\x0A\xE8\x7A\xA6\x72\x9D",
		"\xD9\x13\x0A\x81\x64\x54\x9F\xE8\x18\x87\x48\x06\xE1\xC7\x01\x4B",
		"\xD7\x9E\x1C\x30\x8A\xA5\xBB\xCD\xEE\xA8\xED\x63\xDF\x41\x2D\xA9",
		"\x04\x3F\x85\x82\xF2\x41\xDB\x35\x1C\xE6\x27\xE1\x53\xE7\xF0\xE4",
		"\xE3\x3B\x4D\xDC\x9C\x38\xF2\x19\x9C\x3E\x7B\x16\x4F\xCC\x05\x36"
	};
	const char* ExpectationsMD5[] = {
		"\xD4\x1D\x8C\xD9\x8F\x00\xB2\x04\xE9\x80\x09\x98\xEC\xF8\x42\x7E",
		"\x0C\xC1\x75\xB9\xC0\xF1\xB6\xA8\x31\xC3\x99\xE2\x69\x77\x26\x61",
		"\x90\x01\x50\x98\x3C\xD2\x4F\xB0\xD6\x96\x3F\x7D\x28\xE1\x7F\x72",
		"\xF9\x6B\x69\x7D\x7C\xB7\x93\x8D\x52\x5A\x2F\x31\xAA\xF1\x61\xD0",
		"\xC3\xFC\xD3\xD7\x61\x92\xE4\x00\x7D\xFB\x49\x6C\xCA\x67\xE1\x3B",
		"\xD1\x74\xAB\x98\xD2\x77\xD9\xF5\xA5\x61\x1C\x2C\x9F\x41\x9D\x9F",
		"\x57\xED\xF4\xA2\x2B\xE3\xC9\x55\xAC\x49\xDA\x2E\x21\x07\xB6\x7A"
	};
	

	uint8_t Tests = 0;
	uint8_t Failed = 0;
	const uint8_t* MDResult;

	// MD2
	auto MD2 = BigLib::DataIntegrity::MessageDigest::MD2();
	auto MD4 = BigLib::DataIntegrity::MessageDigest::MD4();
	const uint8_t* Expectation = nullptr;
	const uint8_t* String = nullptr;

	for (uint8_t i = 0; i < AvailableTests; i++) {
		String = (uint8*)TestStrings[i];

		Expectation = (uint8*)ExpectationsMD2[i];
		MDResult = MD2.Update(String, BigLib::Strings::StringLength(String)).Finalize(); Tests++;
		if (!BigLib::Memory::MemoryCompare(MDResult, Expectation, 16)) {
			std::cout << "MD2 String(" << (const char*)String << ") Failed\n\tValue: "; HexPrint(MDResult, 16); std::cout << "\tExpected: "; HexPrint(Expectation, 16);
			Failed++;
			G_TOTAL_FAILS++;
		}
		else {
			std::cout << "MD2 String IDX:" << (int)i << " Passed.\n";
		}
		MD2.Reset();

		Expectation = (uint8*)ExpectationsMD4[i];
		MDResult = MD4.Update(String, BigLib::Strings::StringLength(String)).Finalize(); Tests++;
		if (!BigLib::Memory::MemoryCompare(MDResult, Expectation, 16)) {
			std::cout << "MD4 String(" << (const char*)String << ") Failed\n\tValue: "; HexPrint(MDResult, 16); std::cout << "\tExpected: "; HexPrint(Expectation, 16);
			Failed++;
			G_TOTAL_FAILS++;
		}
		else {
			std::cout << "MD4 String IDX:" << (int)i << " Passed.\n";
		}
		MD4.Reset();
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