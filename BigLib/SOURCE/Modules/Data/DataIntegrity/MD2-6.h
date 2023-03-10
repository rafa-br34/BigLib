#pragma once
#include "../../../Includes.h"
#include "../Bitwise.h"
#include "../BaseMemory.h"
#include "../Math.h"

namespace BigLib {
	namespace DataIntegrity {
		namespace MD2_6 {

			class MD2 {
			private:
				uint8_t p_AuxBlock[48]{};
				uint8_t p_Block[16]{};

				size_t p_Size;

				void p_ProcessBlock(uint8_t* ChecksumBlock, uint8_t* AuxBlock, const uint8_t* Block) CONST {
					STATIC_CONST uint8_t STable[] = {
						0x29, 0x2E, 0x43, 0xC9, 0xA2, 0xD8, 0x7C, 0x01, 0x3D, 0x36, 0x54, 0xA1, 0xEC, 0xF0, 0x06, 0x13,
						0x62, 0xA7, 0x05, 0xF3, 0xC0, 0xC7, 0x73, 0x8C, 0x98, 0x93, 0x2B, 0xD9, 0xBC, 0x4C, 0x82, 0xCA,
						0x1E, 0x9B, 0x57, 0x3C, 0xFD, 0xD4, 0xE0, 0x16, 0x67, 0x42, 0x6F, 0x18, 0x8A, 0x17, 0xE5, 0x12,
						0xBE, 0x4E, 0xC4, 0xD6, 0xDA, 0x9E, 0xDE, 0x49, 0xA0, 0xFB, 0xF5, 0x8E, 0xBB, 0x2F, 0xEE, 0x7A,
						0xA9, 0x68, 0x79, 0x91, 0x15, 0xB2, 0x07, 0x3F, 0x94, 0xC2, 0x10, 0x89, 0x0B, 0x22, 0x5F, 0x21,
						0x80, 0x7F, 0x5D, 0x9A, 0x5A, 0x90, 0x32, 0x27, 0x35, 0x3E, 0xCC, 0xE7, 0xBF, 0xF7, 0x97, 0x03,
						0xFF, 0x19, 0x30, 0xB3, 0x48, 0xA5, 0xB5, 0xD1, 0xD7, 0x5E, 0x92, 0x2A, 0xAC, 0x56, 0xAA, 0xC6,
						0x4F, 0xB8, 0x38, 0xD2, 0x96, 0xA4, 0x7D, 0xB6, 0x76, 0xFC, 0x6B, 0xE2, 0x9C, 0x74, 0x04, 0xF1,
						0x45, 0x9D, 0x70, 0x59, 0x64, 0x71, 0x87, 0x20, 0x86, 0x5B, 0xCF, 0x65, 0xE6, 0x2D, 0xA8, 0x02,
						0x1B, 0x60, 0x25, 0xAD, 0xAE, 0xB0, 0xB9, 0xF6, 0x1C, 0x46, 0x61, 0x69, 0x34, 0x40, 0x7E, 0x0F,
						0x55, 0x47, 0xA3, 0x23, 0xDD, 0x51, 0xAF, 0x3A, 0xC3, 0x5C, 0xF9, 0xCE, 0xBA, 0xC5, 0xEA, 0x26,
						0x2C, 0x53, 0x0D, 0x6E, 0x85, 0x28, 0x84, 0x09, 0xD3, 0xDF, 0xCD, 0xF4, 0x41, 0x81, 0x4D, 0x52,
						0x6A, 0xDC, 0x37, 0xC8, 0x6C, 0xC1, 0xAB, 0xFA, 0x24, 0xE1, 0x7B, 0x08, 0x0C, 0xBD, 0xB1, 0x4A,
						0x78, 0x88, 0x95, 0x8B, 0xE3, 0x63, 0xE8, 0x6D, 0xE9, 0xCB, 0xD5, 0xFE, 0x3B, 0x00, 0x1D, 0x39,
						0xF2, 0xEF, 0xB7, 0x0E, 0x66, 0x58, 0xD0, 0xE4, 0xA6, 0x77, 0x72, 0xF8, 0xEB, 0x75, 0x4B, 0x0A,
						0x31, 0x44, 0x50, 0xB4, 0x8F, 0xED, 0x1F, 0x1A, 0xDB, 0x99, 0x8D, 0x33, 0x9F, 0x11, 0x83, 0x14
					};

					uint8_t J;
					uint8_t T;

					for (T = ChecksumBlock[15], J = 0; J < 16; J++) {
						ChecksumBlock[J] ^= STable[Block[J] ^ T];
						T = ChecksumBlock[J];
					}

					for (J = 0; J < 16; J++) {
						AuxBlock[16 + J] = Block[J];
						AuxBlock[32 + J] = AuxBlock[16 + J] ^ AuxBlock[J];
					}

					for (T = 0, J = 0; J < 18; J++) {

						for (uint8_t K = 0; K < 48; K++) {
							AuxBlock[K] ^= STable[T];
							T = AuxBlock[K];
						}

						T = (T + J) & 0xFF;
					}
				}

			public:
				uint8_t Checksum[16]{};

				MD2() {
					this->Reset();
				}

				MD2& Reset() {
					Memory::MemoryFill(this->Checksum, 0x00, sizeof(this->Checksum));
					Memory::MemoryFill(this->p_AuxBlock, 0x00, sizeof(this->p_AuxBlock));
					this->p_Size = 0;
					return *this;
				}

				MD2& Update(const uint8_t* Data, size_t Size) {
					size_t N;

					while (Size > 0) {
						N = Math::Min(Size, 16 - this->p_Size);

						Memory::MemorySet(this->p_Block + this->p_Size, Data, N);
						this->p_Size += N;
						Data += N;
						Size -= N;

						if (this->p_Size == 16) {
							this->p_ProcessBlock(this->Checksum, this->p_AuxBlock, this->p_Block);
							this->p_Size = 0;
						}
					}
					return *this;
				}

				const uint8_t* Finalize() {
					const uint8_t N = 16 - (uint8_t)this->p_Size;
					Memory::MemoryFill(this->p_Block + this->p_Size, N, N);
					this->p_ProcessBlock(this->Checksum, this->p_AuxBlock, this->p_Block);
					Memory::MemorySet(this->p_Block, this->Checksum, 16);
					this->p_ProcessBlock(this->Checksum, this->p_AuxBlock, this->p_Block);

					return this->Checksum;
				}
				
			};

		}
	}
}