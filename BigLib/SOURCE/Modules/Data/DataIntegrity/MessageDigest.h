#pragma once
#include "../../../Includes.h"
#include "../Bitwise.h"
#include "../BaseMemory.h"
#include "../Math.h"

namespace BigLib {
	namespace DataIntegrity {
		namespace MessageDigest {
			class MD2 {
			private:
				uint8 p_Block48[48]{};
				uint8 p_Block16[16]{};
				umax p_Size;

				void p_TransformBlock(uint8* CurrentChecksum, uint8* Block48, CONST uint8* State16) CONST {
					STATIC_CONST uint8 S[] = {
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


					uint8 J;
					uint8 T = CurrentChecksum[15];
					for (J = 0; J < 16; J++) {
						Block48[32 + J] = (Block48[16 + J] = State16[J]) ^ Block48[J];
						T = (CurrentChecksum[J] ^= S[State16[J] ^ T]);
					}

					// Encrypt 18 Rounds
					uint8 R;
					for (T = 0, R = 0; R < 18; R++) {

						for (J = 0; J < 48; J++)
							T = (Block48[J] ^= S[T]);

						// T = (T + R) % 256
						STATIC_ASSERT(sizeof(T) == 1, FILELINE_SUFFIX("Type Of \"T\" Is Bigger Than 0xFF"));
						T += R; // Overflow will act as modulo 0xFF
					}
				}

			public:
				uint8 Checksum[16]{};

				MD2() {
					this->Reset();
				}

				MD2& Reset() {
					Memory::MemoryFill(this->Checksum, 0x00, sizeof(this->Checksum));
					Memory::MemoryFill(this->p_Block16, 0x00, sizeof(this->p_Block16));
					Memory::MemoryFill(this->p_Block48, 0x00, sizeof(this->p_Block48));
					
					this->p_Size = 0;
					return *this;
				}

				MD2& Update(CONST uint8* Data, umax Size) {
					umax N;

					while (Size > 0) {
						N = Math::Min(Size, 16 - this->p_Size);

						Memory::MemoryCopy(this->p_Block16 + this->p_Size, Data, N);
						this->p_Size += N;
						Data += N;
						Size -= N;

						if (this->p_Size == 16) {
							this->p_TransformBlock(this->Checksum, this->p_Block48, this->p_Block16);
							this->p_Size = 0;
						}
					}
					return *this;
				}

				CONST uint8* Finalize() {
					CONST uint8 N = 16 - (uint8)this->p_Size;

					uint8 Padding[16];
					Memory::MemoryFill(Padding, N, N);

					this->Update(Padding, N);
					this->Update(this->Checksum, 16);

					Memory::MemoryCopy(this->Checksum, this->p_Block48, 16);

					return this->Checksum;
				}
			};

			// Implementation based on RFC 1320
#define F(X, Y, Z) (((X) & (Y)) | ((~(X)) & (Z)))
#define G(X, Y, Z) (((X) & (Y)) | ((X) & (Z)) | ((Y) & (Z)))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))

#define ROUND1(A, B, C, D, X, K, S) ((A) = Bitwise::RotateLeft((A) + F(B, C, D) + (X[K]), (S)))
#define ROUND2(A, B, C, D, X, K, S) ((A) = Bitwise::RotateLeft((A) + G(B, C, D) + (X[K]) + uint32(0x5A827999), (S)))
#define ROUND3(A, B, C, D, X, K, S) ((A) = Bitwise::RotateLeft((A) + H(B, C, D) + (X[K]) + uint32(0x6ED9EBA1), (S)))
			class MD4 {
			private:
				uint8 p_Buffer[64]{};
				umax p_Size[2];

				void p_TransformBlock(uint32* State, uint32 CONST* X) CONST {
					uint32 A = State[0], B = State[1], C = State[2], D = State[3];
					uint32 AA = A, BB = B, CC = C, DD = D;

					ROUND1(A,B,C,D, X,0, 3);	ROUND1(D,A,B,C, X,1, 7);	ROUND1(C,D,A,B, X,2, 11);	ROUND1(B,C,D,A, X,3, 19);
					ROUND1(A,B,C,D, X,4, 3);	ROUND1(D,A,B,C, X,5, 7);	ROUND1(C,D,A,B, X,6, 11);	ROUND1(B,C,D,A, X,7, 19);
					ROUND1(A,B,C,D, X,8, 3);	ROUND1(D,A,B,C, X,9, 7);	ROUND1(C,D,A,B, X,10, 11);	ROUND1(B,C,D,A, X,11, 19);
					ROUND1(A,B,C,D, X,12, 3);	ROUND1(D,A,B,C, X,13, 7);	ROUND1(C,D,A,B, X,14, 11);	ROUND1(B,C,D,A, X,15, 19);

					ROUND2(A,B,C,D, X,0, 3);	ROUND2(D,A,B,C, X,4, 5);	ROUND2(C,D,A,B, X,8, 9);	ROUND2(B,C,D,A, X,12, 13);
					ROUND2(A,B,C,D, X,1, 3);	ROUND2(D,A,B,C, X,5, 5);	ROUND2(C,D,A,B, X,9, 9);	ROUND2(B,C,D,A, X,13, 13);
					ROUND2(A,B,C,D, X,2, 3);	ROUND2(D,A,B,C, X,6, 5);	ROUND2(C,D,A,B, X,10, 9);	ROUND2(B,C,D,A, X,14, 13);
					ROUND2(A,B,C,D, X,3, 3);	ROUND2(D,A,B,C, X,7, 5);	ROUND2(C,D,A,B, X,11, 9);	ROUND2(B,C,D,A, X,15, 13);

					ROUND3(A,B,C,D, X,0, 3);	ROUND3(D,A,B,C, X,8, 9);	ROUND3(C,D,A,B, X,4, 11);	ROUND3(B,C,D,A, X,12, 15);
					ROUND3(A,B,C,D, X,2, 3);	ROUND3(D,A,B,C, X,10, 9);	ROUND3(C,D,A,B, X,6, 11);	ROUND3(B,C,D,A, X,14, 15);
					ROUND3(A,B,C,D, X,1, 3);	ROUND3(D,A,B,C, X,9, 9);	ROUND3(C,D,A,B, X,5, 11);	ROUND3(B,C,D,A, X,13, 15);
					ROUND3(A,B,C,D, X,3, 3);	ROUND3(D,A,B,C, X,11, 9);	ROUND3(C,D,A,B, X,7, 11);	ROUND3(B,C,D,A, X,15, 15);

					State[0] = A + AA; State[1] = B + BB; State[2] = C + CC; State[3] = D + DD;
				}

			public:
				uint8 Checksum[16]{};

				MD4() {
					this->Reset();
				}

				MD4& Reset() {
					Memory::MemoryCopy(this->Checksum, (const void*)"\x01\x23\x45\x67\x89\xAB\xCD\xEF\xFE\xDC\xBA\x98\x76\x54\x32\x10", sizeof(this->Checksum));

					this->p_Size[0] = 0;
					this->p_Size[1] = 0;
					return *this;
				}

				// TODO: Make sure it can handle more than 4GB of input data
				MD4& Update(CONST uint8* Data, umax Size) {
					umax Index = (this->p_Size[0] >> 3) & 0b00111111;
					
					if ((this->p_Size[0] += (Size << 3)) < (Size << 3))
						this->p_Size[1]++;
					this->p_Size[1] += (Size >> 29);

					umax PartLen = 64 - Index;
					umax i = 0;
					
					if (Size >= PartLen) {
						Memory::MemoryCopy(&this->p_Buffer[Index], Data, PartLen);
						this->p_TransformBlock((uint32*)this->Checksum, (uint32*)this->p_Buffer);

						for (i = PartLen; i + 63 < Size; i += 64)
							this->p_TransformBlock((uint32*)this->Checksum, (CONST uint32*)&Data[i]);

						Index = 0;
					}
					else
						i = 0;
					
					Memory::MemoryCopy(&this->p_Buffer[Index], &Data[i], Size - i);

					return *this;
				}

				CONST uint8* Finalize() {
					STATIC_CONST uint8 Padding[] = {
						0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
					};

					uint8 Bits[8];

					((uint32*)Bits)[0] = (uint32)this->p_Size[0];
					((uint32*)Bits)[1] = (uint32)this->p_Size[1];

					uint32 Index = ((this->p_Size[0] >> 3) & 0b00111111);
					uint32 PadLen = (Index < 56) ? (56 - Index) : (120 - Index);
					this->Update(Padding, PadLen);

					this->Update(Bits, 8);
					return this->Checksum;
					
				}
			};

#undef ROUND1
#undef ROUND2
#undef ROUND3

#undef F
#undef G
#undef H
		}
	}
}