#pragma once
#include "../../Includes.h"



namespace BigLib {
	namespace Math {
		CONST_EXPRESSION auto CONSTANTS_PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706;
		CONST_EXPRESSION auto CONSTANTS_E  = 2.71828182845904523536028747135266249775724709369995957496696762772407663035354759457138217852516642;

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


		template<typename Value, typename ModuloRoundCastType=int>
		CONST_EXPRESSION INLINE Value Modulo(Value A, Value B) {
			return A - ModuloRoundCastType(A / B) * B;
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

		template<typename Value>
		CONST_EXPRESSION INLINE Value Absolute(Value X) {
#if APPROACH__MATH_ABSOLUTE == 1
			return X > Value(0) ? X : -X;
#elif APPROACH__MATH_ABSOLUTE == 2
			return (X * (X > Value(0))) + (-X * (X < Value(0)));
#elif APPROACH__MATH_ABSOLUTE == 3
			Value Temp = X >> ((sizeof(Value) * 8) - 1);
			X ^= Temp;
			X += Temp & 1;
			return X;
#else
#error APPROACH__MATH_ABSOLUTE Invalid Value
#endif
		}

		// TODO: Make actual SQRT implementation
		INLINE double SquareRoot(double X) {
			return std::sqrt(X);
		}

		INLINE float SquareRoot(float X) {
			return std::sqrt(X);
		}

		INLINE double SquareRoot(int X) {
			return std::sqrt(X);
		}

		template<typename Value>
		Value Exponent(Value A) {
			return Power(CONSTANTS_E, A);
		}

		template<typename Value>
		bool IsPrimeInteger(Value A) {
			if (A == Value(0) || A == Value(1))
				return false;

			for (Value B = Value(2); B < 2 + (A / 2); B++) {
				if (A % B == Value(0))
					return false;
			}
			return true;
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

		template <typename Value>
		CONST_EXPRESSION INLINE Value StepFunction(Value Input) {
			return (Input > Value(0)) ? Value(1) : Value(0);
		}

		template <typename Value>
		CONST_EXPRESSION INLINE Value Sigmoid(Value Input) {
			return Value(1) / (Value(1) + Exponent(-Input));
		}

		template <typename Value>
		CONST_EXPRESSION INLINE Value HyperbolicTangent(Value Input) {
			Value Val = Exponent(Value(2) * Input);
			return (Val - Value(1)) / (Val + Value(1));
		}

		template <typename Value>
		CONST_EXPRESSION INLINE Value SiLU(Value Input) {
			return Input / (Value(1) + Exponent(-Input));
		}

		template <typename Value>
		CONST_EXPRESSION INLINE Value ReLU(Value Input) {
			return Max(Value(0), Input);
		}
	}
}
