#include <iostream>
#include <iomanip>
#include "../SOURCE/BigLib.h"
#include "Example.h"

template<typename Type>
void PrintList(Type* List, size_t ListSize, size_t Separate = 8, size_t Padding = (sizeof(Type) * 2)) {
	for (size_t i = 0; i < ListSize; i++) {
		if (i % Separate == 0)
			std::cout << '\n';

		std::cout << std::hex << "0x" << std::uppercase << std::setfill('0') << std::setw(Padding) << (size_t)(List[i]) << ", ";
	}
	std::cout << std::dec << '\n';
}


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
		std::cout << i << " : " << (size_t)List[i] << '\n';
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
void _TEST_CRC(BigLib::Crypts::CRC::CRCEngine<Type, Polynomial, ReflectIn, ReflectOut, Initial, XOROut, Width, TableLen>* CRCClass) {
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
	};


	CRCClass->UpdateCRC(Data, DataLen);
	auto CRC = (uint64_t)CRCClass->GetCRC();
	if (CRC != Results[_TEST_CRC_I]) {
		std::cout << AlgoNames[_TEST_CRC_I] << "->[" << _TEST_CRC_I << "] Failed, Expected " << Results[_TEST_CRC_I] << " Got: " << CRC;
		PrintList(CRCClass->GetLookupTable(), TableLen);
	}
	else {
		std::cout << AlgoNames[_TEST_CRC_I] << " Passed" << std::endl;
		_TEST_CRC_PASS++;
	}

	_TEST_CRC_I++;

	delete[] CRCClass;
}

float TEST_CRCs() {
	std::cout << "Begining CRC Test\n";
	_TEST_CRC_I = 0;
	_TEST_CRC_PASS = 0;
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

	_TEST_CRC(new BigLib::Crypts::CRC::CRC_17_CAN_FD());

	_TEST_CRC(new BigLib::Crypts::CRC::CRC_21_CAN_FD());

	_TEST_CRC(new BigLib::Crypts::CRC::CRC_24_BLE());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_24_FLEXRAY_A());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_24_FLEXRAY_B());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_24_INTERLAKEN());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_24_LTE_A());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_24_LTE_B());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_24_OPENPGP());
	_TEST_CRC(new BigLib::Crypts::CRC::CRC_24_OS_9());

	if (_TEST_CRC_PASS != _TEST_CRC_I)
		std::cout << _TEST_CRC_PASS << " Out Of " << _TEST_CRC_I << " CRCs Passed" << std::endl;
	else
		std::cout << "All " << _TEST_CRC_PASS << " CRCs Passed" << std::endl;

	std::cout << "CRC Test Finished\n";
	return ((float)_TEST_CRC_PASS / (float)_TEST_CRC_I) * 100.f;
}


void LIB_TEST() {
	float Stability[] = {
		TEST_CRCs()
	};
	size_t TestCount = sizeof(Stability) / sizeof(float);

	size_t Fails = 0;
	size_t Unsuccessful = 0;
	for (size_t i = 0; i < TestCount; i++) {
		if (Stability[i] == 0.f) Fails++;
		else if (Stability[i] != 100.f) Unsuccessful++;
	}

	std::cout << TestCount << " Tests Were Ran";
	if (Fails != 0)
		std::cout << " " << Fails << " Tests Failed";
	if (Unsuccessful != 0)
		std::cout << " " << Unsuccessful << " Tests Were Unsuccessful.";

	std::cout << "\nLibrary Stability: " << BigLib::Math::Average(Stability, TestCount) << "%\n";
}

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

static unsigned char const table_byte[256] = {
	0x00, 0x30, 0x60, 0x50, 0xC0, 0xF0, 0xA0, 0x90,
	0xB0, 0x80, 0xD0, 0xE0, 0x70, 0x40, 0x10, 0x20,
	0x50, 0x60, 0x30, 0x00, 0x90, 0xA0, 0xF0, 0xC0,
	0xE0, 0xD0, 0x80, 0xB0, 0x20, 0x10, 0x40, 0x70,
	0xA0, 0x90, 0xC0, 0xF0, 0x60, 0x50, 0x00, 0x30,
	0x10, 0x20, 0x70, 0x40, 0xD0, 0xE0, 0xB0, 0x80,
	0xF0, 0xC0, 0x90, 0xA0, 0x30, 0x00, 0x50, 0x60,
	0x40, 0x70, 0x20, 0x10, 0x80, 0xB0, 0xE0, 0xD0,
	0x70, 0x40, 0x10, 0x20, 0xB0, 0x80, 0xD0, 0xE0,
	0xC0, 0xF0, 0xA0, 0x90, 0x00, 0x30, 0x60, 0x50,
	0x20, 0x10, 0x40, 0x70, 0xE0, 0xD0, 0x80, 0xB0,
	0x90, 0xA0, 0xF0, 0xC0, 0x50, 0x60, 0x30, 0x00,
	0xD0, 0xE0, 0xB0, 0x80, 0x10, 0x20, 0x70, 0x40,
	0x60, 0x50, 0x00, 0x30, 0xA0, 0x90, 0xC0, 0xF0,
	0x80, 0xB0, 0xE0, 0xD0, 0x40, 0x70, 0x20, 0x10,
	0x30, 0x00, 0x50, 0x60, 0xF0, 0xC0, 0x90, 0xA0,
	0xE0, 0xD0, 0x80, 0xB0, 0x20, 0x10, 0x40, 0x70,
	0x50, 0x60, 0x30, 0x00, 0x90, 0xA0, 0xF0, 0xC0,
	0xB0, 0x80, 0xD0, 0xE0, 0x70, 0x40, 0x10, 0x20,
	0x00, 0x30, 0x60, 0x50, 0xC0, 0xF0, 0xA0, 0x90,
	0x40, 0x70, 0x20, 0x10, 0x80, 0xB0, 0xE0, 0xD0,
	0xF0, 0xC0, 0x90, 0xA0, 0x30, 0x00, 0x50, 0x60,
	0x10, 0x20, 0x70, 0x40, 0xD0, 0xE0, 0xB0, 0x80,
	0xA0, 0x90, 0xC0, 0xF0, 0x60, 0x50, 0x00, 0x30,
	0x90, 0xA0, 0xF0, 0xC0, 0x50, 0x60, 0x30, 0x00,
	0x20, 0x10, 0x40, 0x70, 0xE0, 0xD0, 0x80, 0xB0,
	0xC0, 0xF0, 0xA0, 0x90, 0x00, 0x30, 0x60, 0x50,
	0x70, 0x40, 0x10, 0x20, 0xB0, 0x80, 0xD0, 0xE0,
	0x30, 0x00, 0x50, 0x60, 0xF0, 0xC0, 0x90, 0xA0,
	0x80, 0xB0, 0xE0, 0xD0, 0x40, 0x70, 0x20, 0x10,
	0x60, 0x50, 0x00, 0x30, 0xA0, 0x90, 0xC0, 0xF0,
	0xD0, 0xE0, 0xB0, 0x80, 0x10, 0x20, 0x70, 0x40,
};

unsigned crc4interlaken_byte(unsigned crc, void const* mem, size_t len) {
	unsigned char const* data = (const uint8_t*)mem;
	if (data == NULL)
		return 0;
	crc &= 0xf;
	crc <<= 4;
	while (len--)
		crc = table_byte[crc ^ *data++];
	crc >>= 4;
	return crc;
}


#if EXAMPLE_SELECTOR == 0
int main() {
	std::cout << crc4interlaken_byte(0x0, "0123456789", 10) << '\n';

	LIB_TEST();
	//TestThreadPool();
	TestList();
	//TestPRNGs();
	//return 0;
	//std::cout << (size_t)BigLib::Bitwise::BinaryReflect<uint8_t>(0xF1) << '\n';

	/*
	{
		BigLib::Crypts::CRC::CRC_Base<uint8_t, 0x03, false, false, 0xF, 0xF, 4> CRCTest = {};
		CRCTest.UpdateCRC("123456789", 9);
		auto CRC = (uint64_t)CRCTest.GetCRC();
		std::cout << std::hex << CRC << '\n';
		std::cout << std::hex << (size_t)(CRCTest.GetLookupTable()[0x80]) << '\n';

	}
	*/


	//BigLib::Random::ACORN Gen = {};
	//Gen.Seed(1);
	//for (size_t i = 0; i < 100; i++) {
	//	std::cout << Gen.Next() << std::endl;
	//}
}
#endif