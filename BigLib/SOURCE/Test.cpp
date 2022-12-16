#include <iostream>
#include <iomanip>
#include "BigLib.h"



void TestThreadPool() {
	const size_t Threads = std::thread::hardware_concurrency();

	struct Task {
		size_t Type = 0;
		uintptr_t* Data = nullptr;
		size_t DataSize = 0;
	};

	BigLib::MultiThreading::ThreadPool<Task> ThreadPool = {};

	for (size_t i = 0; i < Threads; i++) {
		ThreadPool.InitializeThread([](Task& Tsk) -> void {
			while (Tsk.Type > 0) {
				Tsk.Type--;
			}
			std::cout << "Done";
			});
	}

	if (ThreadPool.RunningThreads() != Threads)
		__debugbreak();

	for (size_t i = 0; i < 50; i++) {
		Task Tsk = {};
		Tsk.Type = 0xffffffff;
		ThreadPool.TaskList.PushTask(Tsk);
	}

	

	while (ThreadPool.TaskList.TaskCount() > 0) {
		Sleep(1);
	}
}

void TestList() {
	BigLib::Types::List<uint8_t> List = {};
	List.PreAllocate(10);

	for (uint8_t i = 1; i <= 10; i++)
		List.PushFront(i * 10);
	
	//List.Move(0, 10, 1, true);
	for (uint8_t i = 1; i <= 10; i++)
		List.PushBack(i * 10);

	for (size_t i = 0; i < List.Size(); i++) {
		std::cout << i + 1 << " : " << (size_t)List[i] << '\n';
	}
}

void TestPRNGs() {
	// TODO: I Couldn't Find Any Reference For XorShift Generators Output Data.

	{
		BigLib::Random::MersenneTwister19937 MersenneTwister19937 = {};
		MersenneTwister19937.Seed();

		int Supposed[] = { -795755684, 581869302, -404620562, -708632711, 545404204, -133711905, -372047867, 949333985, -1579004998, 1323567403 };
		for (size_t i = 0; i < sizeof(Supposed) / sizeof(int); i++) {
			int Result = MersenneTwister19937.Next();
			if (Supposed[i] != Result) {
				std::cout << "BigLib::Random::MersenneTwister19937 Failed At Index " << i << " Expected: " << Supposed[i] << " Got: " << Result << std::endl;
				break;
			}

		}
	}
	{
		BigLib::Random::MersenneTwister19937_64 MersenneTwister19937 = {};
		MersenneTwister19937.Seed();

		int64_t Supposed[] = { -795755684, 581869302, -404620562, -708632711, 545404204, -133711905, -372047867, 949333985, -1579004998, 1323567403 };
		for (size_t i = 0; i < sizeof(Supposed) / sizeof(int); i++) {
			int64_t Result = MersenneTwister19937.Next();
			if (Supposed[i] != Result) {
				std::cout << "BigLib::Random::MersenneTwister19937_64 Failed At Index " << i << " Expected: " << Supposed[i] << " Got: " << Result << std::endl;
				break;
			}

		}
	}
}

size_t _TEST_CRC_I = 0;
size_t _TEST_CRC_PASS = 0;
template<
	typename Type,
	const Type Polynomial,

	const bool ReflectIn = false,
	const bool ReflectOut = false,
	const Type Initial = 0,
	const Type XOROut = 0,

	const size_t Width = SIZEOF_BITS(Type),
	const size_t TableLen = 256
>
void _TEST_CRC(BigLib::Crypts::CRC::CRC_Base<Type, Polynomial, ReflectIn, ReflectOut, Initial, XOROut, Width, TableLen>* CRCClass) {
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
		0xA1, // 8 MAXIM
		0x15, // 8 DARC
		0x7E, // 8 ICODE
		0x97, // 8 EBU
		0xD0, // 8 ROHC
		0xA1, // 8 ITU
		0xF4, // 8 SMBUS
		0x26, // 8 Bluetooth
		0xDF, // 8 Autosar
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
		"8 MAXIM",
		"8 DARC",
		"8 ICODE",
		"8 EBU",
		"8 ROHC",
		"8 ITU",
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
	};

	
	CRCClass->UpdateCRC(Data, DataLen);
	auto CRC = (uint64_t)CRCClass->GetCRC();
	if (CRC != Results[_TEST_CRC_I])
		std::cout << AlgoNames[_TEST_CRC_I] << "->[" << _TEST_CRC_I << "] Failed, Expected " << Results[_TEST_CRC_I] << " Got: " << CRC << std::endl;
	else {
		std::cout << AlgoNames[_TEST_CRC_I] << " Passed" << std::endl;
		_TEST_CRC_PASS++;
	}
		
	_TEST_CRC_I++;

	delete[] CRCClass;
}

void TEST_CRCs() {
	// Not Optimal But Will Do The Job.
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_3_GSM());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_3_ROHC());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_4_G_704());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_4_INTERLAKEN());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_5_EPC_C1G2());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_5_G_704());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_5_USB());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_6_CDMA2000_A());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_6_CDMA2000_B());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_6_DARC());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_6_G_704());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_6_GSM());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_7_MMC());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_7_ROHC());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_7_UMTS());
	
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_CDMA2000());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_WCDMA());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_DBV_S2());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_MAXIM());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_DARC());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_I_CODE());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_EBU());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_ROHC());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_ITU());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_SMBUS());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_BLUETOOTH());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_AUTOSAR());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_GSM_A());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_GSM_B());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_HITAG());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_MIFARE_MAD());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_LTE());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_NRSC_5());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_OPENSAFETY());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_8_SAE_J1850());

	_TEST_CRC(new BigLib::Crypts::CRC::CRC_10_ATM());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_10_CDMA2000());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_10_GSM());

	_TEST_CRC(new BigLib::Crypts::CRC::CRC_11_FLEXRAY());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_11_UMTS());

	_TEST_CRC(new BigLib::Crypts::CRC::CRC_12_CDMA2000());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_12_DECT());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_12_GSM());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_12_UMTS());

	_TEST_CRC(new BigLib::Crypts::CRC::CRC_13_BBC());

	_TEST_CRC(new BigLib::Crypts::CRC::CRC_14_DARC());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_14_GSM());
	
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_15_CAN());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_15_MPT1327());

	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_ARC());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_CDMA2000());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_CMS());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_DDS_110());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_DECT_R());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_DECT_X());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_DNP());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_EN_13757());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_GENIBUS());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_GSM());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_IBM_3740());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_IBM_SDLC());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_ISO_IEC_14443_3_A());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_KERMIT());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_LJ1200());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_M17());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_MAXIM_DOW());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_MCRF4XX());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_MODBUS());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_NRSC_5());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_OPENSAFETY_A());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_OPENSAFETY_B());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_PROFIBUS());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_RIELLO());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_SPI_FUJITSU());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_T10_DIF());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_TELEDISK());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_TMS37157());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_UMTS());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_USB());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_16_XMODEM());



	if (_TEST_CRC_PASS != _TEST_CRC_I)
		std::cout << _TEST_CRC_PASS << " Out Of " << _TEST_CRC_I << " CRCs Passed" << std::endl;
	else
		std::cout << "All " << _TEST_CRC_PASS << " CRCs Passed" << std::endl;
}


/*
0x00, 0x12, 0x24, 0x36, 0x48, 0x5A, 0x6C, 0x7E,
0x90, 0x82, 0xB4, 0xA6, 0xD8, 0xCA, 0xFC, 0xEE,
0x32, 0x20, 0x16, 0x04, 0x7A, 0x68, 0x5E, 0x4C,
0xA2, 0xB0, 0x86, 0x94, 0xEA, 0xF8, 0xCE, 0xDC,
0x64, 0x76, 0x40, 0x52, 0x2C, 0x3E, 0x08, 0x1A,
0xF4, 0xE6, 0xD0, 0xC2, 0xBC, 0xAE, 0x98, 0x8A,
0x56, 0x44, 0x72, 0x60, 0x1E, 0x0C, 0x3A, 0x28,
0xC6, 0xD4, 0xE2, 0xF0, 0x8E, 0x9C, 0xAA, 0xB8,
0xC8, 0xDA, 0xEC, 0xFE, 0x80, 0x92, 0xA4, 0xB6,
0x58, 0x4A, 0x7C, 0x6E, 0x10, 0x02, 0x34, 0x26,
0xFA, 0xE8, 0xDE, 0xCC, 0xB2, 0xA0, 0x96, 0x84,
0x6A, 0x78, 0x4E, 0x5C, 0x22, 0x30, 0x06, 0x14,
0xAC, 0xBE, 0x88, 0x9A, 0xE4, 0xF6, 0xC0, 0xD2,
0x3C, 0x2E, 0x18, 0x0A, 0x74, 0x66, 0x50, 0x42,
0x9E, 0x8C, 0xBA, 0xA8, 0xD6, 0xC4, 0xF2, 0xE0,
0x0E, 0x1C, 0x2A, 0x38, 0x46, 0x54, 0x62, 0x70,
0x82, 0x90, 0xA6, 0xB4, 0xCA, 0xD8, 0xEE, 0xFC,
0x12, 0x00, 0x36, 0x24, 0x5A, 0x48, 0x7E, 0x6C,
0xB0, 0xA2, 0x94, 0x86, 0xF8, 0xEA, 0xDC, 0xCE,
0x20, 0x32, 0x04, 0x16, 0x68, 0x7A, 0x4C, 0x5E,
0xE6, 0xF4, 0xC2, 0xD0, 0xAE, 0xBC, 0x8A, 0x98,
0x76, 0x64, 0x52, 0x40, 0x3E, 0x2C, 0x1A, 0x08,
0xD4, 0xC6, 0xF0, 0xE2, 0x9C, 0x8E, 0xB8, 0xAA,
0x44, 0x56, 0x60, 0x72, 0x0C, 0x1E, 0x28, 0x3A,
0x4A, 0x58, 0x6E, 0x7C, 0x02, 0x10, 0x26, 0x34,
0xDA, 0xC8, 0xFE, 0xEC, 0x92, 0x80, 0xB6, 0xA4,
0x78, 0x6A, 0x5C, 0x4E, 0x30, 0x22, 0x14, 0x06,
0xE8, 0xFA, 0xCC, 0xDE, 0xA0, 0xB2, 0x84, 0x96,
0x2E, 0x3C, 0x0A, 0x18, 0x66, 0x74, 0x42, 0x50,
0xBE, 0xAC, 0x9A, 0x88, 0xF6, 0xE4, 0xD2, 0xC0,
0x1C, 0x0E, 0x38, 0x2A, 0x54, 0x46, 0x70, 0x62,
0x8C, 0x9E, 0xA8, 0xBA, 0xC4, 0xD6, 0xE0, 0xF2,


*/

void make_crc_table(unsigned char crcTable[])
{
	unsigned char POLYNOMIAL = 0x30;
	unsigned char remainder;
	unsigned char b = 0;
	do
	{
		remainder = b;
		for (int bit = 8; bit > 0; --bit)
		{
			if (remainder & 0x80)
				remainder = (remainder << 1) ^ POLYNOMIAL;
			else
				remainder = (remainder << 1);
		}
		crcTable[(size_t)b] = remainder;
	} while (0 != ++b);
}

int main() {
	//TestThreadPool();
	//TestList();
	//TestPRNGs();
	//return 0;
	//std::cout << (size_t)BigLib::Bitwise::BinaryReflect<uint8_t>(0xF1) << '\n';
	TEST_CRCs();
	
	unsigned char Table[256];
	make_crc_table(Table);
	{
		for (size_t i = 0; i < 256; i++) {
			if (i % 8 == 0)
				std::cout << '\n';

			std::cout << std::hex << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << (size_t)(Table[i]) << ", ";
		}
		std::cout << '\n';
	}
	{
		BigLib::Crypts::CRC::CRC_Base<uint8_t, 0x03, false, false, 0xF, 0xF, 4> CRCTest = {};
		CRCTest.UpdateCRC("123456789", 9);
		auto CRC = (uint64_t)CRCTest.GetCRC();
		std::cout << std::hex << CRC << '\n';
		for (size_t i = 0; i < 256; i++) {
			if (i % 8 == 0)
				std::cout << '\n';
			
			std::cout << std::hex << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << (size_t)(CRCTest.GetLookupTable()[i]) << ", ";
		}
		std::cout << '\n';
	}
	
	

	//BigLib::Random::ACORN Gen = {};
	//Gen.Seed(1);
	//for (size_t i = 0; i < 100; i++) {
	//	std::cout << Gen.Next() << std::endl;
	//}
	
}
