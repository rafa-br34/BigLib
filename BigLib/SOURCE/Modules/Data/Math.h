#pragma once
#include "../../Includes.h"

#define INDEX_2D_FROM_XY(X, Y, Width) ((Width) * (Y) + (X))
#define INDEX_3D_FROM_XYZ(X, Y, Z, Width, Height) ((X) + (Height) * ((Y) + (Width) * (Z)))


namespace BigLib {
	namespace Math {
		CONST_EXPRESSION INLINE auto CONSTANTS_PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706;
		CONST_EXPRESSION INLINE auto CONSTANTS_E  = 2.71828182845904523536028747135266249775724709369995957496696762772407663035354759457138217852516642;

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


		template<typename Value, typename ModuloFloorCastType=int>
		CONST_EXPRESSION INLINE Value Modulo(CONST Value X, CONST Value Y) {
			Value R = X / Y;
			Value D = R - Value(ModuloFloorCastType(R));
			if (D > Value(0))
				return X - (Y * Value(ModuloFloorCastType(R)));
			else if (D < Value(0))
				if (X > Value(0))
					return ((Y * Value(ModuloFloorCastType(R))) + X);
				else
					return Y - ((Y * Value(ModuloFloorCastType(R))) - X);

			return Value(0);
		}

		template<typename Value>
		CONST_EXPRESSION INLINE Value Min(CONST Value A, CONST Value B) {
			return A < B ? A : B;
		}

		template<typename Value>
		CONST_EXPRESSION INLINE Value Max(CONST Value A, CONST Value B) {
			return A > B ? A : B;
		}

		template<typename Value, typename IterType=size_t>
		CONST_EXPRESSION INLINE Value IntegerPower(CONST Value X, CONST IterType Y) {
			Value Result = X;
			for (IterType i = 0; i < Y - IterType(1); i++)
				Result *= X;
			return Result;
		}

		template<typename Value>
		CONST_EXPRESSION INLINE Value Power(CONST Value X, CONST Value Y) {
			Value T;
			if (Y == Value(0))
				return Value(1);
			T = Power<Value>(X, Y / Value(2));
			if (Y % Value(2) == Value(0))
				return T * T;
			else {
				if (Y > Value(0))
					return X * T * T;
				else
					return (T * T) / X;
			}
		}

		template<typename Value>
		CONST_EXPRESSION INLINE Value Absolute(CONST Value X) {
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
#error APPROACH__MATH_ABSOLUTE Invalid Approach
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
		bool IsIntegerPrime(const Value A) {
			if (A == Value(0) || A == Value(1))
				return false;

			for (Value B = Value(2); B < 2 + (A / 2); B++) {
				if (A % B == Value(0))
					return false;
			}
			return true;
		}


		template<typename Value, typename SizeType=size_t>
		CONST_EXPRESSION INLINE Value Average(const Value List[], SizeType Items) {
			Value Sum{};
			for (SizeType i = 0; i < Items; i++) {
				Sum += List[i];
			}
			return Sum / (Value)Items;
		}

		template<typename DataType, typename SizeType = size_t, typename FloatType = double>
		CONST_EXPRESSION INLINE FloatType ShannonEntropy(DataType Data[], SizeType DataCount) {

		}

		template <typename Value>
		CONST_EXPRESSION INLINE Value DotProduct(const Value First[], const Value Second[], size_t ArraySize) {
			Value Result{};
			for (size_t i = 0; i < ArraySize; i++) {
				Result += First[i] * Second[i];
			}
			return Result;
		}
		

		template<typename Value>
		Value Exponent(CONST Value A) {
			return FloatPower<Value>(CONSTANTS_E, A);
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
