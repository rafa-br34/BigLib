#include <iostream>
#include <fstream>

#include "../SOURCE/BigLib.h"

#include "Example.h"


/*
* Idea: 
* Descriptors:
* Medium: 1 DATA(3: DATA_SIZE, 3: ALGORITHM, 2: RESERVED), 2 INDEX, 3 SEED
* Small: 1 DATA(3: DATA_SIZE, 3: ALGORITHM, 2: RESERVED), 1 INDEX, 2 SEED
* 
* 1 DATA(2: DATA_SIZE + DescriptorSize, 3: D)
*/


//enum FileDescriptorFlags : uint8_t {
//	FLAG_
//};

struct FileDescriptor {
	uint16_t Compressions = 0;

};


FORCE_INLINE void MakeDescriptor(size_t Seed, size_t Index, uint8_t Algo, uint8_t Size, uint8_t* DataPointer) {
	
	// ** ** ** 00 ?? ?? 
	*((uint32_t*)DataPointer) = (uint16_t)Seed;
	// ?? ?? ?? ** ** ??
	*((uint16_t*)(DataPointer + 1)) = (uint16_t)Seed;
	// ?? ?? ?? ?? ?? **
	DataPointer[5] = (Algo << 5) | (Algo << 2);
}

template<typename Generator, typename GeneratorType, const size_t MaxSeed, const size_t MaxIndex>
FORCE_INLINE bool TryGenerator(uint8_t* Input, size_t* ResultingStartIndex, uint8_t* ResultingSize, size_t* ResultingSeed) {
	Generator GeneratorInstance;
	bool Successful = false;

	for (size_t Seed = 1; Seed < MaxSeed + 1; Seed++) {

		uint8_t DataIndex = 0;
		size_t StartIndex = 0;
		GeneratorInstance.Seed((GeneratorType)Seed);

		uint8_t Data[MaxIndex];
		for (size_t i = 0; i < MaxIndex / sizeof(GeneratorType); i++) {
			((GeneratorType*)Data)[i] = GeneratorInstance.Next();
		}
		for (size_t i = 0; i < MaxIndex; i++) {
			if (Data[i] == Input[DataIndex]) {
				DataIndex++;
			}
			else {
				if (DataIndex >= 2 && DataIndex > *ResultingSize) {
					*ResultingStartIndex = i;
					*ResultingSize = DataIndex;
					*ResultingSeed = Seed;
					Successful = true;
				}

				i -= (DataIndex - 1);
				DataIndex = 0;
			}
		}
	}
	return Successful;
}


FORCE_INLINE bool FindForData(uint8_t* Input, uint8_t* DescriptorOut, uint8_t* E) {
	const size_t MaxIndex = 0xFF;
	const size_t MaxSeed = 0xFF;

	size_t ResultingStartIndex = 0;
	size_t ResultingSeed = 0;
	uint8_t ResultingSize = 0;
	bool Successful = false;

	const char* Names = "WX*MmSPps@xº";

	for (uint8_t A = 0; A < 1; A++) {
		switch (A) {
			case 0:
				Successful = TryGenerator<BigLib::Random::XorWow, uint64_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;
			case 1:
				Successful = TryGenerator<BigLib::Random::XoShiro256P, uint64_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;
			case 2:
				Successful = TryGenerator<BigLib::Random::XoShiro256SS, uint64_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;

			case 3:
				Successful = TryGenerator<BigLib::Random::MersenneTwister19937_64, uint64_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;

			case 4:
				Successful = TryGenerator<BigLib::Random::MersenneTwister19937, uint32_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;

			case 5:
				Successful = TryGenerator<BigLib::Random::SplitMix64, uint64_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;

			case 6:
				Successful = TryGenerator<BigLib::Random::T_PCG32Fast, uint32_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;

			case 7:
				Successful = TryGenerator<BigLib::Random::T_XorShift128P, uint64_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;

			case 8:
				Successful = TryGenerator<BigLib::Random::XorShift64S, uint64_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;

			case 9:
				Successful = TryGenerator<BigLib::Random::XorShift64, uint64_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;

			case 10:
				Successful = TryGenerator<BigLib::Random::XorShift32, uint32_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;

			case 11:
				Successful = TryGenerator<BigLib::Random::XorShift128, uint64_t, MaxSeed, MaxIndex>(Input, &ResultingStartIndex, &ResultingSize, &ResultingSeed);
				break;

			default:
				break;
		}
		
		if (Successful) {
			std::cout << (char)('0' + ResultingSize);
			*E = Names[A];
			//MakeDescriptor(ResultingSeed, ResultingStartIndex, A, ResultingSize, DescriptorOut);
			break;
		}
	}

	if (!Successful) std::cout << '\xDB';
	return Successful;
}


#if EXAMPLE_SELECTOR == 1
int main(int argc, char* argv[]) {
	std::cout << "Opening File \"" << argv[1] << "\"\n";

	std::ifstream File(argv[1], std::ios_base::binary | std::ios_base::ate);

	size_t DescriptorSize = 3;
	size_t FileSize = File.tellg();
	File.seekg(0, std::ios_base::beg);

	if (FileSize < DescriptorSize) {
		std::cout << "File Is Too Tiny" << std::endl;
		return 1;
	}
	else {
		std::cout << "File Has " << FileSize << " Bytes" << std::endl;
	}

	size_t Delta = FileSize % DescriptorSize;

	if (Delta > 0)
		std::cout << Delta << " Bytes Will Not Be Compressed" << std::endl;

	std::cout << "File Fits " << ((FileSize - Delta) / DescriptorSize) << " Descriptors" << std::endl;

	std::ofstream Entropy("Entropy.bin", std::ios_base::binary);
	BigLib::Random::XorShift64S GeneratorInstance;
	for (size_t Seed = 1; Seed < 0xFFFF + 1; Seed++) {
		GeneratorInstance.Seed(Seed);
	
		for (size_t i = 0; i < 0xFF / sizeof(uint64_t); i++) {
			auto A = GeneratorInstance.Next();
			Entropy.write((char*)&A, sizeof(A));
			//std::cout << (void*)A << std::endl;
		}
	}
	Entropy.close();

	// bruh...
	size_t Chars[256]{};

	uint8_t* FileBuffer = new uint8_t[FileSize];
	File.read((char*)FileBuffer, FileSize);

	uint8_t* Buffer = new uint8_t[DescriptorSize];
	for (size_t i = 0; i < ((FileSize - Delta) / DescriptorSize); i++) {

		//std::cout << *(void**)Buffer << std::endl;
		uint8_t C = 0;
		FindForData(FileBuffer + (i * DescriptorSize), Buffer, &C);
		if (C > 0) Chars[C]++;
	}
	delete[] Buffer;
	std::cout << '\n';

	size_t Total = 0;
	for (size_t i = 0; i < 256; i++) {
		if (Chars[i] > 0) { Total += Chars[i]; std::cout << char(i) << ' ' << Chars[i] << std::endl; }
	}
	std::cout << "Total: " << Total << std::endl;
	

}
#endif


/*
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <string>
#include <intrin.h>

#define FORCE_INLINE __forceinline

class XORShift64 {
public:
	uint64_t State = 0;
	FORCE_INLINE uint64_t NextState() {
		uint64_t X = this->State;
		X ^= X << 13;
		X ^= X >> 7;
		X ^= X << 17;
		return this->State = X;
	}
};


FORCE_INLINE bool Compare(uint8_t* Data, size_t DataSize, size_t IterMax, XORShift64* Generator, size_t* ResultSize, size_t* ResultIndex) {
	uint64_t CurrentChunk = 0;
	uint8_t CurrentOffset = 0;
	size_t DataIndex = 0;
	size_t DataBest = 0;
	size_t DataBestIndex = 0;

	for (size_t i = 0; i < IterMax; i++) {
		// If The Chunk Has Been Used Load Another One
		if (i % 8 == 0) {
			CurrentChunk = Generator->NextState();
			CurrentOffset = 0;
		}
		uint8_t Byte = (CurrentChunk & (0xFF << CurrentOffset)) >> CurrentOffset;


		if (Byte == Data[DataIndex]) {
			DataIndex++;
			if (DataIndex >= DataSize) {
				*ResultIndex = i;
				*ResultSize = DataIndex;
				return true;
			}
		}
		else {
			// We still want to get the best result even if there is no possible full match
			if (DataIndex > DataBest) {
				DataBest = DataIndex;
				DataBestIndex = i;
			}
			//i -= (DataIndex - 1);
			DataIndex = 0;
		}

		CurrentOffset += 8;
	}

	*ResultIndex = DataBestIndex;
	*ResultSize = DataBest;
	return false;
}

bool FindResultForData(uint8_t* Data, uint8_t DataSize, uint8_t SeedSize, uint8_t IndexSize, uint64_t* ResultSeed, size_t* ResultIndex, size_t* MaxSize) {
	XORShift64 XorShift;

	size_t SeedMax = (256 ^ SeedSize) - 1;
	size_t IndexMax = (256 ^ IndexSize) - 1;

	uint64_t InitialSeed = 1;

	uint64_t BestSeed = 0;
	size_t Index = 0;
	size_t BestSize = 0;

	size_t TempResultSize = 0;
	size_t TempResultIndex = 0;
	while (InitialSeed < SeedMax) {
		XorShift.State = InitialSeed;

		if (Compare(Data, DataSize, IndexMax, &XorShift, &TempResultSize, &TempResultIndex)) {
			std::cout << "\tFound Result With Seed: " << InitialSeed << " Index: " << TempResultIndex << std::endl;
			*ResultSeed = InitialSeed;
			*ResultIndex = TempResultIndex;
			return true;
			break;
		}
		else if (TempResultSize > BestSize) {
			BestSeed = InitialSeed;
			Index = TempResultIndex;
			BestSize = TempResultSize;
			//std::cout << "\tExpanded To Size: " << TempResultSize << " With Seed: " << InitialSeed << " Index: " << TempResultIndex << std::endl;
		}

		InitialSeed++;
	}
	*ResultSeed = InitialSeed;
	*ResultIndex = TempResultIndex;
	*MaxSize = BestSize;
	return false;
}


struct DataDescriptor {

};

int main(int argc, char* argv[]) {
	uint8_t SeedSize = 2;
	uint8_t IterSize = 1;

	std::ifstream Input(argv[1], std::ios_base::in | std::ios_base::binary);


	uint8_t Buffer[5] = {};

	size_t Seed = 0;
	size_t Index = 0;
	size_t BestSize = 0;

	size_t SavedBytes = 0;

	while (!Input.eof()) {
		Input.read((char*)Buffer, sizeof(Buffer));

		bool Result = FindResultForData(Buffer, sizeof(Buffer), SeedSize, IterSize, &Seed, &Index, &BestSize);
		if (Result)
			SavedBytes += sizeof(Buffer) - (SeedSize + IterSize);
		else if (BestSize > SeedSize + IterSize)
			SavedBytes += (sizeof(Buffer) - BestSize) - (SeedSize + IterSize);
	}

	std::cout << " Bytes: " << SavedBytes << std::endl;
}

/
size_t Seed = 0;

#ifdef GCC
#define FORCE_INLINE always_inline
#else
#define FORCE_INLINE __forceinline
#endif

size_t Random() {
	%NEW_PRNG_CODE%


	return Seed;
}



int main() {
	srand((unsigned int)(std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1)));

	size_t FileSize = 0x100000; // 1MB


	for (size_t i = 0; i < 200; i++) {

		Seed = (size_t)rand();
		std::string FileName = "Results/";
		FileName.append(std::to_string(Seed));
		FileName.append(".bin");
		std::ofstream Output(FileName, std::ios_base::binary);


		for (size_t Size = 0; Size < FileSize / sizeof(size_t); Size++) {
			static size_t RandomRes = Random();
			Output.write((char*)&RandomRes, sizeof(size_t));
		}

		Output.close();
	}

}

*/