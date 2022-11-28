#include <iostream>
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
			std::cout << "D";
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
	BigLib::Utils::List<size_t> List = {};
	List.PreAllocate(10);

	for (size_t i = 0; i < 5; i++)
		List.PushFront(i * 10);
	for (size_t i = 0; i < 5; i++)
		List.PushBack(i * 10);

	for (size_t i = 0; i < List.Size(); i++) {
		std::cout << i << " : " << List[i] << '\n';
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



/*
struct IACO2
{
	KORDEJ, MAXJNT, IXV1[MAXOP1 + 1], IXV2[MAXOP1 + 1];
};


struct IACO2 iaco2;

// --------------------------------------------
double ACORNJ(XDUMMY);
{
	//C
	//C          Fortran implementation of ACORN random number generator
	//C          of order less than or equal to 120 (higher orders can be
	//C          obtained by increasing the parameter value MAXORD) and
	//C          modulus less than or equal to 2^60.
	//C
	//C          After appropriate initialization of the common block /IACO2/
	//C          each call to ACORNJ generates a single variate drawn from
	//C          a uniform distribution over the unit interval.
	//C
	// implicit double(A - H, O - Z)
	const MAXORD = 120;
	const MAXOP1 = MAXORD + 1;

	//COMMON/IACO2/KORDEJ, MAXJNT, IXV1(MAXOP1), IXV2(MAXOP1)
	for (I = 1, iaco2.KORDEJ) {
		iaco2.IXV1[I + 1] = (iaco2.IXV1[I + 1] + iaco2.IXV1[I]);
		iaco2.IXV2[I + 1] = (iaco2.IXV2[I + 1] + iaco2.IXV2[I]);
		if (iaco2.IXV2[I + 1] >= iaco2.MAXJNT)
		{
			iaco2.IXV2[I + 1] = iaco2.IXV2[I + 1] - iaco2.MAXJNT;
			iaco2.IXV1[I + 1]++;
		}
		if (iaco2.IXV1[I + 1] >= iaco2.MAXJNT)
			iaco2.IXV1[I + 1] = iaco2.IXV1[I + 1] - iaco2.MAXJNT;

	}
	return (DBLE(iaco2.IXV1[iaco2.KORDEJ + 1]) + DBLE(iaco2.IXV2[iaco2.KORDEJ + 1]) / iaco2.MAXJNT) / iaco2.MAXJNT;
};
*/


template<typename Type=long double, const size_t K=150, const long double S=2.0L>
struct ACORN {
	Type SV1[K + 1];
	Type SV2[K + 1];
	Type Y = 1;
	Type M = BigLib::Math::Power<Type>(Type(2), Type(60.0) * S);
	bool Swap = false;

	void Seed(Type Seed) {
		for (size_t i = 1; i < K + 1; i++) {
			this->SV1[i - 1] = BigLib::Math::Power<Type>(Seed, (Type)i);
		}
	}

	Type Next() {
		Swap = !Swap;

		Type* S1;
		Type* S2;
		if (Swap) {
			S1 = this->SV2;
			S2 = this->SV1;
		}
		else {
			S1 = this->SV1;
			S2 = this->SV2;
		}

		for (size_t i = 1; i < K; i++) {
			S1[i] = BigLib::Math::Modulo((S1[i - 1] + S2[i]), this->M);
		}

		return S1[K - 1] / M;
	}
};



int main() {
	//TestThreadPool();
	//TestList();
	//TestPRNGs();
	for (size_t i = 0; i < 0xFFFFFFFFFF; i++) {
		if (BigLib::Math::IsPrimeInteger<int>(i))
			std::cout << i << std::endl;
	}
	
	ACORN<double> Gen = {};
	Gen.Seed(1);
	for (size_t i = 0; i < 100; i++) {
		std::cout << Gen.Next() << std::endl;
	}
	
}
