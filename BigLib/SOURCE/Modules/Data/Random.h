#pragma once
#include "../../Includes.h"
#include "Math.h"

// TODO: Add Option
#define _INLINE_RANDOM_H_ FORCE_INLINE


namespace BigLib {
	namespace Random {

		// -- START OF THE XORSHIFT FAMILY -- //

		// XorShift32
		struct XorShift32 {
			// Must Not Be Zero
			uint32 State = 1;

			CONST_EXPRESSION FORCE_INLINE void Seed(uint32 Seed) {
				this->State = Seed;
			};

			CONST_EXPRESSION _INLINE_RANDOM_H_ uint32 Next() {
				uint32 X = this->State;
				X ^= X << 13;
				X ^= X >> 17;
				X ^= X << 5;
				return X;
			}
		};

		// XorShift64
		struct XorShift64 {
			// Must Not Be Zero
			uint64 State = 1;

			CONST_EXPRESSION FORCE_INLINE void Seed(uint64 Seed) {
				this->State = Seed;
			};

			CONST_EXPRESSION _INLINE_RANDOM_H_ uint64 Next() {
				uint64 X = this->State;
				X ^= X << 13;
				X ^= X >> 7;
				X ^= X << 17;
				return X;
			}
		};

		// XorShift128
		struct XorShift128 {
			// Must Not Be All-Zero
			uint32 State[4] = { 1, 1, 1, 1 };

			CONST_EXPRESSION FORCE_INLINE void Seed(uint32 Seed) {
				this->State[0] = Seed;
			};

			CONST_EXPRESSION FORCE_INLINE void Seed(uint64 Seed) {
				this->State[0] = (uint32)Seed;
				this->State[1] = (uint32)(Seed >> 32);
			};

			CONST_EXPRESSION _INLINE_RANDOM_H_ uint64 Next() {
				uint32* S = this->State;
				uint32 T = S[3];

				uint32 A = S[0];  /* Perform a contrived 32-bit shift. */
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
			uint32 State[5] = { 1, 1, 1, 1, 0 };
			uint32 Counter;

			CONST_EXPRESSION FORCE_INLINE void Seed(uint32 Seed) {
				this->State[0] = Seed;
			};

			CONST_EXPRESSION FORCE_INLINE void Seed(uint64 Seed) {
				this->State[0] = (uint32)Seed;
				this->State[1] = (uint32)(Seed >> 32);
			};

			CONST_EXPRESSION _INLINE_RANDOM_H_ uint32 Next() {
				auto X = this->State;

				uint32 T = X[4];

				uint32 S = X[0];
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
			uint64 State;

			CONST_EXPRESSION FORCE_INLINE void Seed(uint64 Seed) {
				this->State = Seed;
			};

			CONST_EXPRESSION _INLINE_RANDOM_H_ uint64 Next() {
				uint64 X = this->State;
				X ^= X >> 12;
				X ^= X << 25;
				X ^= X >> 27;
				this->State = X;
				return X * UI64(0x2545F4914F6CDD1D);
			}

			// XorShift64* Modified To Return The High 32 Bits, Safest Version
			CONST_EXPRESSION _INLINE_RANDOM_H_ uint32 NextHigh32() {
				return (Next() & UI64(0xFFFFFFFF00000000)) >> (64 - 32);
			}

			// XorShift64* Modified To Return The High 40 Bits, Safe Version
			CONST_EXPRESSION _INLINE_RANDOM_H_ uint64 NextHigh40() {
				return (Next() & UI64(0xFFFFFFFFF0000000)) >> (64 - 40);
			}
		};

		// XorShift128+
		// Better Than Mersanne Twister Or WELL When Properly Seeded
		// Do Not Reverse The Output
		// Parameters Can Be Tuned
		template<const uint8_t A = 23, const uint8_t B = 18, const uint8_t C = 5>
		struct XorShift128P {
			// Must Not Be All-Zero
			uint64 State[2]{ 1, 1 };

			CONST_EXPRESSION FORCE_INLINE void Seed(uint64 Seed) {
				this->State[0] = Seed;
			};

			CONST_EXPRESSION FORCE_INLINE void Seed(uint64 Seed0, uint64 Seed1) {
				this->State[0] = Seed0;
				this->State[1] = Seed1;
			};

			CONST_EXPRESSION _INLINE_RANDOM_H_ uint64 Next() {
				uint64 T = this->State[0];
				uint64 S = this->State[1];

				this->State[0] = S;

				T ^= T << A;
				T ^= T >> B;
				T ^= S ^ (S >> C);

				this->State[1] = T;
				return T + S;
			}
		};
		typedef XorShift128P<23, 18, 5> T_XorShift128P;

		/*
		* XoShiro Generators
		*/


		// XoShiro256**
		struct XoShiro256SS {
			// Must Not Be All-Zero
			uint64 State[4] = { 1, 1, 1, 1 };

			CONST_EXPRESSION FORCE_INLINE void Seed(uint64 Seed) {
				this->State[0] = Seed;
			};

			CONST_EXPRESSION _INLINE_RANDOM_H_ uint64 Next() {
				uint64* S = this->State;
				const uint64 Result = Bitwise::RotateLeft(S[1] * 5, 7) * 9;
				const uint64 T = S[1] << 17;

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
			uint64 State[4] = { 1, 1, 1, 1 };

			CONST_EXPRESSION FORCE_INLINE void Seed(uint64 Seed) {
				this->State[0] = Seed;
			};

			CONST_EXPRESSION _INLINE_RANDOM_H_ uint64 Next() {
				uint64* S = this->State;
				const uint64 Result = S[0] + S[3];
				const uint64 T = S[1] << 17;

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
		

		// PRNG Recommended For Use In Seeding Random Generators That Need A Initial Non-Zero State
		// You May Want To Use This When Seeding A Generator For Cryptographic Uses
		struct SplitMix64 {
			uint64 State;

			CONST_EXPRESSION FORCE_INLINE void Seed(uint64 Seed) {
				this->State = Seed;
			};

			CONST_EXPRESSION _INLINE_RANDOM_H_ uint64 Next() {
				uint64 Result = (this->State += 0x9E3779B97f4A7C15);

				Result = (Result ^ (Result >> 30)) * 0xBF58476D1CE4E5B9;
				Result = (Result ^ (Result >> 27)) * 0x94D049BB133111EB;

				return Result ^ (Result >> 31);
			}
		};


		template<const uint64 Multiplier=6364136223846793005u, const uint64 Increment=1442695040888963407u>
		struct PCG32 {
			uint64 State = 0x4D595DF4D0F33173;
			
			CONST_EXPRESSION _INLINE_RANDOM_H_ void Seed(uint64 Seed) {
				this->State = Seed + Increment;
				(void)this->Next();
			}

			CONST_EXPRESSION _INLINE_RANDOM_H_ void Seed(uint32 Seed) {
				this->Seed((uint64)Seed);
			}

			CONST_EXPRESSION _INLINE_RANDOM_H_ uint32 Next() {
				uint64 X = this->State;
				unsigned Count = (unsigned)(X >> 59);		// 59 = 64 - 5

				this->State = X * Multiplier + Increment;
				X ^= X >> 18;								// 18 = (64 - 27)/2
				return Bitwise::RotateRight((uint32)(X >> 27), Count);	// 27 = 32 - 5
			}

			
		};
		// Template For PCG32
		typedef PCG32<6364136223846793005u, 1442695040888963407u> T_PCG32;


		template<const uint64 Multiplier=6364136223846793005u, const uint64 Increment=1442695040888963407u>
		struct PCG32Fast {
			// Must Be Odd
			uint64 State = 0xCAFEF00DD15EA5E5u;

			CONST_EXPRESSION _INLINE_RANDOM_H_ void Seed(uint64 Seed) {
				this->State = 2 * Seed + 1;
				(void)this->Next();
			}

			CONST_EXPRESSION _INLINE_RANDOM_H_ void Seed(uint32 Seed) {
				this->Seed((uint64)Seed);
			}

			CONST_EXPRESSION _INLINE_RANDOM_H_ uint32 Next() {
				uint64 X = this->State;
				unsigned Count = (unsigned)(X >> 61);	// 61 = 64 - 3

				State = X * Multiplier;
				X ^= X >> 22;
				return (uint32)(X >> (22 + Count));	// 22 = 32 - 3 - 7
			}

			
		};
		// Template For PCG32Fast
		typedef PCG32Fast<6364136223846793005u, 1442695040888963407u> T_PCG32Fast;


		template<typename Type=long double, const umax K=150, const long double S=2.0L>
		struct ACORN {
			Type SV1[K + 1];
			Type SV2[K + 1];
			Type M = Math::Power<Type>(Type(2), Type(60.0) * S);
			bool Swap = false;

			// Seed Needs To Be A Prime Number
			CONST_EXPRESSION _INLINE_RANDOM_H_ void Seed(Type Seed) {
				for (umax i = 1; i < K + 1; i++) {
					this->SV1[i - 1] = Math::Power<Type>(Seed, (Type)i);
				}
			}

			CONST_EXPRESSION _INLINE_RANDOM_H_ Type Next() {
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

				for (umax i = 1; i < K; i++) {
					S1[i] = Math::Modulo((S1[i - 1] + S2[i]), this->M);
				}

				return S1[K - 1] / M;
			}
		};



		template<
			typename Type,
			const umax W,
			const umax N,
			const umax M,
			const umax R,
			const umax A,
			const umax U,
			const umax D,
			const umax S,
			const umax B,
			const umax T,
			const umax C,
			const umax L,
			const umax F
		>
		struct MersenneTwister {
		private:
			Type MT[N - 1];
			Type Index = N + 1;
			const Type LowerMask = (Type)((UI64(1) << R) - 1);
			const Type UpperMask = GET_LOW_BITS(~LowerMask, W);

			// NOTE: Keep The FORCE_INLINE On This Function Since It's Only Used Once
			FORCE_INLINE void p_Twist() {
				for (umax i = 0; i < (N - 1); i++) {
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
			_INLINE_RANDOM_H_ void Seed(Type Seed=5489) {
				this->Index = N;
				this->MT[0] = Seed;
				for (umax i = 1; i < (N - 1); i++)
					this->MT[i] = GET_LOW_BITS((F * (this->MT[i-1] ^ (this->MT[i-1] >> (W - 2))) + i), W);
			}

			_INLINE_RANDOM_H_ Type Next() {
				if (this->Index >= N) {
					IASSERT_EX(this->Index > N, "Generator Wasn't Seeded Properly.")
					this->p_Twist();
				}

				Type Y = this->MT[this->Index];
				Y ^= ((Y >> U) & D);
				Y ^= ((Y << S) & B);
				Y ^= ((Y << T) & C);
				Y ^= (Y >> L);

				this->Index++;

				return GET_LOW_BITS(Y, W);
			}
		};


		// MersenneTwister 19937 32-Bit.
		using MersenneTwister19937 = MersenneTwister<
			unsigned int,
			32, 624, 397, 31,
			UI32(0x9908B0DF),
			11, UI32(0xFFFFFFFF),
			7, UI32(0x9D2C5680),
			15, UI32(0xEFC60000),
			18, UI32(1812433253)
		>;

		// MersenneTwister 19937 64-Bit.
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

#undef _INLINE_RANDOM_H_