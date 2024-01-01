#pragma once
#include "../../../../Includes.h"

namespace BigLib {
	namespace VMs {
		// RootLogic is a very straightforward and easy-to-optimize bytecode-like format that is suitable for many tasks, such as a middle-man for architecture conversion, compilation/decompilation, and virtualization when properly used.
		namespace RootLogic {


			enum Registers : uint8_t {
				D8_0,  D8_1,  D8_2,  D8_3,  D8_4,  D8_5,  D8_6,  D8_7,  D8_8,  D8_9,  D8_10,  D8_11,  D8_12,  D8_13,  D8_14,  D8_15,  D8_16,  D8_17,  D8_18,  D8_19,  D8_20,  D8_21,  D8_22,  D8_23,  D8_24,  D8_25,  D8_26,  D8_27,  D8_28,  D8_29,  D8_30,  D8_31,
				D16_0, D16_1, D16_2, D16_3, D16_4, D16_5, D16_6, D16_7, D16_8, D16_9, D16_10, D16_11, D16_12, D16_13, D16_14, D16_15, D16_16, D16_17, D16_18, D16_19, D16_20, D16_21, D16_22, D16_23, D16_24, D16_25, D16_26, D16_27, D16_28, D16_29, D16_30, D16_31,
				D32_0, D32_1, D32_2, D32_3, D32_4, D32_5, D32_6, D32_7, D32_8, D32_9, D32_10, D32_11, D32_12, D32_13, D32_14, D32_15, D32_16, D32_17, D32_18, D32_19, D32_20, D32_21, D32_22, D32_23, D32_24, D32_25, D32_26, D32_27, D32_28, D32_29, D32_30, D32_31,
				D64_0, D64_1, D64_2, D64_3, D64_4, D64_5, D64_6, D64_7, D64_8, D64_9, D64_10, D64_11, D64_12, D64_13, D64_14, D64_15, D64_16, D64_17, D64_18, D64_19, D64_20, D64_21, D64_22, D64_23, D64_24, D64_25, D64_26, D64_27, D64_28, D64_29, D64_30, D64_31,
				PC64,
				STCKB64, STCKP64,
				FLAGS64
			};

			// Values
			// U8  : UINT8
			// U16 : UINT16
			// U32 : UINT32
			// U64 : UINT64
			// I8  : INT8
			// I16 : INT16
			// I32 : INT32
			// I64 : INT64
			enum Bytecode : uint8_t {
				RL_LEVEL_0, // First Level, Contains The Basic Instructions
				SETDREG, // Set Data Register SETDREG <U8>Index <U8-U64>Value
				JUMPR8, // Jump To Relative Address JUMPR8 <I8>Relative Address
				JUMPR16, // Jump To Relative Address JUMPR16 <I16>Relative Address
				JUMPR32, // Jump To Relative Address JUMPR32 <I32>Relative Address
				JUMPR64, // Jump To Relative Address JUMPR64 <I64>Relative Address
			};


			struct VMState {
				void* VMMemory;

				uint64_t PC64;
				uint64_t STCKB64, STCKP64;
				uint64_t FLAGS64;

				// 3.840 Bits In Total
				uint8_t D8[32];		// 256  Bits
				uint16_t D16[32];	// 512  Bits
				uint32_t D32[32];	// 1024 Bits
				uint64_t D64[32];	// 2048 Bits


			};
		}
	}
}
