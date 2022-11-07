#pragma once
#include "../../Includes.h"



namespace BigLib {
	namespace Math {
		constexpr auto CONSTANTS_PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;


		template<typename Value>
		CONST_EXPRESSION INLINE Value ImpreciseLerp(Value A, Value B, Value T) {
			return A + T * (B - A);
		}

		template<typename Value>
		CONST_EXPRESSION INLINE Value PreciseLerp(Value A, Value B, Value T) {
			return (Value(1) - T) * A + T * B;
		}

		template<typename Value>
		CONST_EXPRESSION INLINE Value Lerp(Value A, Value B, Value T) {
			return Math::ImpreciseLerp<Value>(A, B, T);
		}

		template<typename Value, typename ModuloFloored=int>
		CONST_EXPRESSION INLINE Value Modulo(Value A, Value B) {
			return A - ModuloFloored(A / B) * B;
		}

		template<typename Value>
		CONST_EXPRESSION INLINE Value Absolute(Value X) {
#if MATH_ABSOLUTE_APPROACH == 1
			return X > 0 ? X : -X;
#elif MATH_ABSOLUTE_APPROACH == 2
			return (X * (X > 0)) + (-X * (X < 0));
#elif MATH_ABSOLUTE_APPROACH == 3
			Value Temp = X >> ((sizeof(Value) * 8) - 1);
			X ^= Temp;
			X += Temp & 1;
			return X;
#else
#error MATH_ABSOLUTE Invalid Approach
#endif
		}

		template<typename Value>
		CONST_EXPRESSION INLINE Value Min(Value A, Value B) {
			return A < B ? A : B;
		}

		template<typename Value>
		CONST_EXPRESSION INLINE Value Max(Value A, Value B) {
			return A > B ? A : B;
		}

		template<typename Value>
		CONST_EXPRESSION INLINE Value Power(Value A, Value V) {
			Value Result = A;
			for (Value i = 0; i < V - Value(1); i++)
				Result *= A;
			return Result;
		}

		INLINE double SquareRoot(double X) {
			return std::sqrt(X);
		}

		INLINE float SquareRoot(float X) {
			return std::sqrt(X);
		}

		INLINE int SquareRoot(int X) {
			return std::sqrt(X);
		}

		template<typename Value>
		CONST_EXPRESSION INLINE Value Average(Value List[], size_t Items) {
			Value Sum{};
			for (size_t i = 0; i < Items; i++) {
				Sum += List[i];
			}
			return Sum / (Value)Items;
		}

		template <typename Value>
		CONST_EXPRESSION INLINE Value Dot(Value First[], Value Second[], size_t ArraySize) {
			Value Result{};
			for (size_t i = 0; i < ArraySize; i++) {
				Result += First[i] * Second[i];
			}
			return Result;
		}
	}
}
