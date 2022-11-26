#pragma once
#include "../../Includes.h"

namespace BigLib {
	namespace Random {
		namespace XorShift {
			// -- START OF THE XORSHIFT FAMILY -- //

			// XorShift32
			struct XorShift32 {
				// Must Not Be Zero
				uint32_t State = 1;

				CONST_EXPRESSION FORCE_INLINE uint32_t Next() {
					uint32_t X = this->State;
					X ^= X << 13;
					X ^= X >> 17;
					X ^= X << 5;
					return X;
				}
			};

			// XorShift64
			struct XorShift64 {
				// Must Not Be Zero
				uint64_t State = 1;

				CONST_EXPRESSION FORCE_INLINE uint64_t Next() {
					uint64_t X = this->State;
					X ^= X << 13;
					X ^= X >> 7;
					X ^= X << 17;
					return X;
				}
			};

			// XorShift128
			struct XorShift128 {
				// Must Not Be All-Zero
				uint32_t State[4];

				CONST_EXPRESSION FORCE_INLINE uint64_t Next() {
					uint32_t* S = this->State;
					uint32_t T = S[3];

					uint32_t A = S[0];  /* Perform a contrived 32-bit shift. */
					S[3] = S[2];
					S[2] = S[1];
					S[1] = A;

					T ^= T << 11;
					T ^= T >> 8;
					return S[0] = T ^ A ^ (A >> 19);
				}

			};


			/*
			* Non-Linear Variations
			*/


			// XorWow
			// Recommended To Use As Floating Point Generator(Low Bits Are Weak)
			struct XorWow {
				// First 4 Words Must Not Be All-Zero
				uint32_t State[5] = {1, 1, 1, 1, 0};
				uint32_t Counter;

				CONST_EXPRESSION FORCE_INLINE uint32_t Next() {
					auto X = this->State;
	
					uint32_t T = X[4];

					uint32_t S = X[0];
					X[4] = X[3];
					X[3] = X[2];
					X[2] = X[1];
					X[1] = S;

					T ^= T >> 2;
					T ^= T << 1;
					T ^= S ^ (S << 4);
					X[0] = T;
					this->Counter += 362437;
					return T + this->Counter;
				}

			};

			// XorShift64*
			// Recommended To Use As Floating Point Generator(Low Bits Are Weak), However, The High 32-40 Bits Are Still Safe To Use
			struct XorShift64S {
				// Must Not Be Zero
				uint64_t State;

				CONST_EXPRESSION FORCE_INLINE uint64_t Next() {
					uint64_t X = this->State;
					X ^= X >> 12;
					X ^= X << 25;
					X ^= X >> 27;
					return X * UI64(0x2545F4914F6CDD1D);
				}

				// XorShift64* Modified To Return The High 32 Bits, Safest Version
				CONST_EXPRESSION FORCE_INLINE uint32_t NextHigh32() {
					return (Next() & UI64(0xFFFFFFFF00000000)) >> (64 - 32);
				}

				// XorShift64* Modified To Return The High 40 Bits, Safe Version
				CONST_EXPRESSION FORCE_INLINE uint64_t NextHigh40() {
					return (Next() & UI64(0xFFFFFFFFF0000000)) >> (64 - 40);
				}
			};

			// XorShift128+
			// Better Than Mersanne Twister Or WELL When Properly Seeded
			// Do Not Reverse The Output
			// Parameters Can Be Tuned
			template<const uint8_t A=23, const uint8_t B=18, const uint8_t C=5>
			struct XorShift128P {
				// Must Not Be All-Zero
				uint64_t State[2]{1, 1};

				CONST_EXPRESSION FORCE_INLINE uint64_t Next() {
					uint64_t T = this->State[0];
					uint64_t S = this->State[1];

					this->State[0] = S;

					T ^= T << A;
					T ^= T >> B;
					T ^= S ^ (S >> C);

					this->State[1] = T;
					return T + S;
				}
			};


			/*
			* XoShiro Generators
			*/


			// XoShiro256**
			struct XoShiro256SS {
				// Must Not Be All-Zero
				uint64_t State[4];

				CONST_EXPRESSION FORCE_INLINE uint64_t Next() {
					uint64_t* S = this->State;
					const uint64_t Result = Bitwise::RotateLeft(S[1] * 5, 7) * 9;
					const uint64_t T = S[1] << 17;

					S[2] ^= S[0];
					S[3] ^= S[1];
					S[1] ^= S[2];
					S[0] ^= S[3];

					S[2] ^= T;
					S[3] = Bitwise::RotateLeft(S[3], 45);

					return Result;
				}
			};

			// XoShiro256+
			// About 15% Faster Than XoShiro256** But The Low 3 Bits Have Low Linear Complexity
			struct XoShiro256P {
				// Must Not Be All-Zero
				uint64_t State[4];

				CONST_EXPRESSION FORCE_INLINE uint64_t Next() {
					uint64_t* S = this->State;
					const uint64_t Result = S[0] + S[3];
					const uint64_t T = S[1] << 17;

					S[2] ^= S[0];
					S[3] ^= S[1];
					S[1] ^= S[2];
					S[0] ^= S[3];

					S[2] ^= T;
					S[3] = Bitwise::RotateLeft(S[3], 45);

					return Result;
				}
			};

			
			// -- END OF THE XORSHIFT FAMILY -- //
		}

		// PRNG Recommended For Use In Seeding Random Generators That Need A Initial Non-Zero State, Such as XorShift And XoShiro Generators
		// You May Want To Use This When Seeding A Generator For Cryptographic Uses
		struct SplitMix64 {
			uint64_t State;

			CONST_EXPRESSION FORCE_INLINE uint64_t Next() {
				uint64_t Result = (this->State += 0x9E3779B97f4A7C15);

				Result = (Result ^ (Result >> 30)) * 0xBF58476D1CE4E5B9;
				Result = (Result ^ (Result >> 27)) * 0x94D049BB133111EB;

				return Result ^ (Result >> 31);
			}
		};


		template<
			typename Type,
			const size_t W,
			const size_t N,
			const size_t M,
			const size_t R,
			const size_t A,
			const size_t U,
			const size_t D,
			const size_t S,
			const size_t B,
			const size_t T,
			const size_t C,
			const size_t L,
			const size_t F
		>
		class MersenneTwister {
			Type MT[N - 1];
			Type Index = N + 1;
			const Type LowerMask = (Type)((UI64(1) << R) - 1);
			const Type UpperMask = LOWEST_BITS(~LowerMask, W);

			CONST_EXPRESSION FORCE_INLINE void Twist() {
				for (size_t i = 0; i < (N - 1); i++) {
					Type X = (this->MT[i] & this->UpperMask) | (MT[(i + 1) % N] & LowerMask);
					Type XA = X >> 1;
					if (X % 2 != 0) {
						XA ^= A;
					}
					this->MT[i] = this->MT[(i + M) % N] ^ XA;
				}
				this->Index = 0;
			}

		public:
			CONST_EXPRESSION INLINE void Seed(Type Seed=5489) {
				this->Index = N;
				this->MT[0] = Seed;
				for (size_t i = 1; i < (N - 1); i++)
					this->MT[i] = LOWEST_BITS((F * (this->MT[i-1] ^ (this->MT[i-1] >> (W - 2))) + i), W);
			}

			CONST_EXPRESSION FORCE_INLINE Type Next() {
				if (this->Index >= N) {
					IASSERT_EX(this->Index > N, "Generator Wasn't Seeded Properly.")
					this->Twist();
				}

				Type Y = this->MT[this->Index];
				Y ^= ((Y >> U) & D);
				Y ^= ((Y << S) & B);
				Y ^= ((Y << T) & C);
				Y ^= (Y >> L);

				this->Index++;

				return LOWEST_BITS(Y, W);
			}
		};


		// Normal Version Of MersenneTwister.
		using MersenneTwister19937 = MersenneTwister<
			unsigned int,
			32, 624, 397, 31,
			UI32(0x9908B0DF),
			11, UI32(0xFFFFFFFF),
			7, UI32(0x9D2C5680),
			15, UI32(0xEFC60000),
			18, UI32(1812433253)
		>;

		// 64-Bit Version Of MersenneTwister.
		using MersenneTwister19937_64 = MersenneTwister<
			unsigned long long,
			64, 312, 156, 31,
			UI64(0xB5026f5AA96619E9),
			29, UI64(0x5555555555555555),
			17, UI64(0x71D67FFFEDA60000),
			37, UI64(0xFFF7EEE000000000),
			43, UI64(6364136223846793005)
		>;
	}
}

