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

typedef uint32_t crc32_t;

static const crc32_t crc_table_24_ble[256] = {
	0x000000, 0x01b4c0, 0x036980, 0x02dd40, 0x06d300, 0x0767c0, 0x05ba80, 0x040e40,
	0x0da600, 0x0c12c0, 0x0ecf80, 0x0f7b40, 0x0b7500, 0x0ac1c0, 0x081c80, 0x09a840,
	0x1b4c00, 0x1af8c0, 0x182580, 0x199140, 0x1d9f00, 0x1c2bc0, 0x1ef680, 0x1f4240,
	0x16ea00, 0x175ec0, 0x158380, 0x143740, 0x103900, 0x118dc0, 0x135080, 0x12e440,
	0x369800, 0x372cc0, 0x35f180, 0x344540, 0x304b00, 0x31ffc0, 0x332280, 0x329640,
	0x3b3e00, 0x3a8ac0, 0x385780, 0x39e340, 0x3ded00, 0x3c59c0, 0x3e8480, 0x3f3040,
	0x2dd400, 0x2c60c0, 0x2ebd80, 0x2f0940, 0x2b0700, 0x2ab3c0, 0x286e80, 0x29da40,
	0x207200, 0x21c6c0, 0x231b80, 0x22af40, 0x26a100, 0x2715c0, 0x25c880, 0x247c40,
	0x6d3000, 0x6c84c0, 0x6e5980, 0x6fed40, 0x6be300, 0x6a57c0, 0x688a80, 0x693e40,
	0x609600, 0x6122c0, 0x63ff80, 0x624b40, 0x664500, 0x67f1c0, 0x652c80, 0x649840,
	0x767c00, 0x77c8c0, 0x751580, 0x74a140, 0x70af00, 0x711bc0, 0x73c680, 0x727240,
	0x7bda00, 0x7a6ec0, 0x78b380, 0x790740, 0x7d0900, 0x7cbdc0, 0x7e6080, 0x7fd440,
	0x5ba800, 0x5a1cc0, 0x58c180, 0x597540, 0x5d7b00, 0x5ccfc0, 0x5e1280, 0x5fa640,
	0x560e00, 0x57bac0, 0x556780, 0x54d340, 0x50dd00, 0x5169c0, 0x53b480, 0x520040,
	0x40e400, 0x4150c0, 0x438d80, 0x423940, 0x463700, 0x4783c0, 0x455e80, 0x44ea40,
	0x4d4200, 0x4cf6c0, 0x4e2b80, 0x4f9f40, 0x4b9100, 0x4a25c0, 0x48f880, 0x494c40,
	0xda6000, 0xdbd4c0, 0xd90980, 0xd8bd40, 0xdcb300, 0xdd07c0, 0xdfda80, 0xde6e40,
	0xd7c600, 0xd672c0, 0xd4af80, 0xd51b40, 0xd11500, 0xd0a1c0, 0xd27c80, 0xd3c840,
	0xc12c00, 0xc098c0, 0xc24580, 0xc3f140, 0xc7ff00, 0xc64bc0, 0xc49680, 0xc52240,
	0xcc8a00, 0xcd3ec0, 0xcfe380, 0xce5740, 0xca5900, 0xcbedc0, 0xc93080, 0xc88440,
	0xecf800, 0xed4cc0, 0xef9180, 0xee2540, 0xea2b00, 0xeb9fc0, 0xe94280, 0xe8f640,
	0xe15e00, 0xe0eac0, 0xe23780, 0xe38340, 0xe78d00, 0xe639c0, 0xe4e480, 0xe55040,
	0xf7b400, 0xf600c0, 0xf4dd80, 0xf56940, 0xf16700, 0xf0d3c0, 0xf20e80, 0xf3ba40,
	0xfa1200, 0xfba6c0, 0xf97b80, 0xf8cf40, 0xfcc100, 0xfd75c0, 0xffa880, 0xfe1c40,
	0xb75000, 0xb6e4c0, 0xb43980, 0xb58d40, 0xb18300, 0xb037c0, 0xb2ea80, 0xb35e40,
	0xbaf600, 0xbb42c0, 0xb99f80, 0xb82b40, 0xbc2500, 0xbd91c0, 0xbf4c80, 0xbef840,
	0xac1c00, 0xada8c0, 0xaf7580, 0xaec140, 0xaacf00, 0xab7bc0, 0xa9a680, 0xa81240,
	0xa1ba00, 0xa00ec0, 0xa2d380, 0xa36740, 0xa76900, 0xa6ddc0, 0xa40080, 0xa5b440,
	0x81c800, 0x807cc0, 0x82a180, 0x831540, 0x871b00, 0x86afc0, 0x847280, 0x85c640,
	0x8c6e00, 0x8ddac0, 0x8f0780, 0x8eb340, 0x8abd00, 0x8b09c0, 0x89d480, 0x886040,
	0x9a8400, 0x9b30c0, 0x99ed80, 0x985940, 0x9c5700, 0x9de3c0, 0x9f3e80, 0x9e8a40,
	0x972200, 0x9696c0, 0x944b80, 0x95ff40, 0x91f100, 0x9045c0, 0x929880, 0x932c40
};

static crc32_t crc_update_ble(crc32_t crc, const void* data, size_t data_len)
{
	const unsigned char* d = (const unsigned char*)data;
	unsigned int tbl_idx;

	while (data_len--) {
		tbl_idx = (crc ^ *d) & 0xff;
		crc = (crc_table_24_ble[tbl_idx] ^ (crc >> 8)) & 0xffffff;

		d++;
	}
	return crc & 0xffffff;
}

#if EXAMPLE_SELECTOR == 0
int main() {
	uint8_t Table[256];
	std::cout << crc_update_ble(0x555555, "0123456789", 10)  << '\n';

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