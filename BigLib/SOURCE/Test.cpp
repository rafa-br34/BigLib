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
	const bool ReflectPolynomial,

	const bool ReflectIn = false,
	const bool ReflectOut = false,
	const Type Initial = 0,
	const Type XOROut = 0,

	const size_t Width = SIZEOF_BITS(Type),
	const size_t TableLen = 256
>
void _TEST_CRC(BigLib::Crypts::CRC::CRC_Base<Type, Polynomial, ReflectPolynomial, ReflectIn, ReflectOut, Initial, XOROut, Width, TableLen>* CRCClass) {
	const char* Data = "123456789";
	size_t DataLen = 9;
	

	uint64_t Results[] = {
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
	};
	const char* AlgoNames[] = {
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
	};

	
	CRCClass->UpdateCRC(Data, DataLen);
	auto CRC = (uint64_t)CRCClass->GetCRC();
	if (CRC != Results[_TEST_CRC_I])
		std::cout << AlgoNames[_TEST_CRC_I] << " Failed, Expected " << Results[_TEST_CRC_I] << " Got: " << CRC << std::endl;
	else {
		std::cout << AlgoNames[_TEST_CRC_I] << " Passed" << std::endl;
		_TEST_CRC_PASS++;
	}
		
	_TEST_CRC_I++;

	delete[] CRCClass;
}

void TEST_CRCs() {
	// Not Optimal But Will Do The Job.
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


	if (_TEST_CRC_PASS != _TEST_CRC_I)
		std::cout << _TEST_CRC_PASS << " Out Of " << _TEST_CRC_I << " CRCs Passed" << std::endl;
	else
		std::cout << "All " << _TEST_CRC_PASS << " CRCs Passed" << std::endl;
}

int main() {
	//TestThreadPool();
	//TestList();
	//TestPRNGs();
	//return 0;
	//std::cout << (size_t)BigLib::Bitwise::BinaryReflect<uint8_t>(0xF1) << '\n';
	TEST_CRCs();
	/*
	{
		
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
	*/
	

	//BigLib::Random::ACORN Gen = {};
	//Gen.Seed(1);
	//for (size_t i = 0; i < 100; i++) {
	//	std::cout << Gen.Next() << std::endl;
	//}
	
}
