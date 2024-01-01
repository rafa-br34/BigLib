#pragma once
#include "../../../Includes.h"
#include "../../Data/DataIntegrity/SimpleChecksums.h"
#include "../../Data/BaseMemory.h"
#include "../../Data/BaseString.h"

namespace BigLib::Protocols::Network {

	namespace IPv4 {
		enum class IPv4Errors : uint8 {
			NONE,
			INVALID_NOTATION,
			INVALID_VALUE,
		};

		union IPv4Address {
			uint8 Parts_8[4];
			uint16 Parts_16[2];
			uint32 Parts_32[1];
		};

		// Converts a IPv4 address written as X.X.X.X into a binary value with network endianness
		// Requires a buffer with at least 4 bytes
		IPv4Errors AddressToBinary(CONST char* Input, uint8* Buffer) {
			uint8 StringSize = Strings::StringLength<char, uint8>(Input);
			CONST auto* StringEnd = Input + StringSize;

			uint32 Address = 0;
			for (uint8 i = 0; i < 4; i++) {
				uint8 Offset = Strings::FindCharacter(Input, (uint8)4, '.');
				
				if (Offset == uint8(-1)) {
					if (i < 3) // Invalid notation
						return IPv4Errors::INVALID_NOTATION;
					else // Reached the end of the string
						Offset = uint8(StringEnd - Input);
				}

				Address |= Strings::ToNumber<uint8>(Input, Offset) << (32 - ((i + 1) * 8));
				Input += Offset + 1;
			}
			
			*(uint32*)Buffer = Bitwise::InterpretAsBigEndian(Address);

			return IPv4Errors::NONE;
		}

		// Requires a buffer with at least 4 bytes
		IPv4Errors AddressToBinary(uint8 A, uint8 B, uint8 C, uint8 D, uint8* Buffer) {
			*(uint32*)Buffer = Bitwise::InterpretAsBigEndian((A << 24) | (B << 16) | (C << 8) | (D << 0));

			return IPv4Errors::NONE;
		}
	}

	namespace IPv6 {
		enum class IPv6Errors : uint8 {
			NONE,
			INVALID_NOTATION,
			INVALID_VALUE,
		};

		union IPv6Address {
			uint8 Parts_8[16];
			uint16 Parts_16[8];
			uint32 Parts_32[4];
		};


		// Requires a buffer with at least 16 bytes
		IPv6Errors AddressToBinary(uint16 A, uint16 B, uint16 C, uint16 D, uint16 E, uint16 F, uint16 G, uint16 H, uint8* Buffer) {
			((uint16*)Buffer)[0] = Bitwise::InterpretAsBigEndian<uint16>(A);
			((uint16*)Buffer)[1] = Bitwise::InterpretAsBigEndian<uint16>(B);
			((uint16*)Buffer)[2] = Bitwise::InterpretAsBigEndian<uint16>(C);
			((uint16*)Buffer)[3] = Bitwise::InterpretAsBigEndian<uint16>(D);
			((uint16*)Buffer)[4] = Bitwise::InterpretAsBigEndian<uint16>(E);
			((uint16*)Buffer)[5] = Bitwise::InterpretAsBigEndian<uint16>(F);
			((uint16*)Buffer)[6] = Bitwise::InterpretAsBigEndian<uint16>(G);
			((uint16*)Buffer)[7] = Bitwise::InterpretAsBigEndian<uint16>(H);


			return IPv6Errors::NONE;
		}
	}
}