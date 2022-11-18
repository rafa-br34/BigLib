#pragma once
#include "../../Data/Math.h"

namespace BigLib {
	namespace Utils {
		template<typename Value>
		class Vector3 {
		public:
			Value X, Y, Z;

			Vector3<Value>()
				: X()
				, Y()
				, Z()
			{};
			Vector3<Value>(Value X, Value Y, Value Z)
				: X(X)
				, Y(Y)
				, Z(Z)
			{};

			// Class Functions
			Vector3<Value>& Lerp(Vector3<Value>& Target, Value Interpolation) {
				this->X = Math::Lerp(this->X, Target.X, Interpolation);
				this->Y = Math::Lerp(this->Y, Target.Y, Interpolation);
				this->Z = Math::Lerp(this->Z, Target.Z, Interpolation);
				return *this;
			}
			Value Dot(Vector3<Value> Target) {
				Value First[] = { this->X, this->Y, this->Z };
				Value Second[] = { Target.X, Target.Y, Target.Z };
				return Math::Dot(First, Second, 3);
			}
			template<typename RetValue = double>
			RetValue Magnitude() {
				return Math::SquareRoot((RetValue)Math::Power(this->X, 2) + (RetValue)Math::Power(this->Y, 2) + (RetValue)Math::Power(this->Z, 2));
			}

			// Comparation Functions
			CONST_EXPRESSION INLINE bool operator==(const Vector3<Value>& Target) const {
				return (this->X == Target.X) && (this->Y == Target.Y) && (this->Z == Target.Z);
			}
			CONST_EXPRESSION INLINE bool operator!=(const Vector3<Value>& Target) const {
				return (this->X != Target.X) && (this->Y != Target.Y) && (this->Z != Target.Z);
			}

			CONST_EXPRESSION INLINE bool operator>=(const Vector3<Value>& Target) const {
				return (this->X >= Target.X) && (this->Y >= Target.Y) && (this->Z >= Target.Z);
			}
			CONST_EXPRESSION INLINE bool operator<=(const Vector3<Value>& Target) const {
				return (this->X <= Target.X) && (this->Y <= Target.Y) && (this->Z <= Target.Z);
			}

			CONST_EXPRESSION INLINE bool operator>(const Vector3<Value>& Target) const {
				return (this->X > Target.X) && (this->Y > Target.Y) && (this->Z > Target.Z);
			}
			CONST_EXPRESSION INLINE bool operator<(const Vector3<Value>& Target) const {
				return (this->X < Target.X) && (this->Y < Target.Y) && (this->Z < Target.Z);
			}

			// Math + & -
			CONST_EXPRESSION INLINE void operator+(const Vector3<Value>& Target) {
				this->X += Target.X;
				this->Y += Target.Y;
				this->Z += Target.Z;
			}
			CONST_EXPRESSION INLINE void operator-(const Vector3<Value>& Target) {
				this->X -= Target.X;
				this->Y -= Target.Y;
				this->Z -= Target.Z;
			}

			CONST_EXPRESSION INLINE void operator+(const Value Target) {
				this->X += Target;
				this->Y += Target;
				this->Z += Target;
			}
			CONST_EXPRESSION INLINE void operator-(const Value Target) {
				this->X -= Target;
				this->Y -= Target;
				this->Z -= Target;
			}

			// Math * & /
			CONST_EXPRESSION INLINE void operator*(const Vector3<Value>& Target) {
				this->X *= Target.X;
				this->Y *= Target.Y;
				this->Z *= Target.Z;
			}
			CONST_EXPRESSION INLINE void operator/(const Vector3<Value>& Target) {
				this->X /= Target.X;
				this->Y /= Target.Y;
				this->Z /= Target.Z;
			}

			CONST_EXPRESSION INLINE void operator*(const Value Target) {
				this->X *= Target;
				this->Y *= Target;
				this->Z *= Target;
			}
			CONST_EXPRESSION INLINE void operator/(const Value Target) {
				this->X /= Target;
				this->Y /= Target;
				this->Z /= Target;
			}

			// Math ^ & %
			CONST_EXPRESSION INLINE void operator^(const Vector3<Value>& Target) {
				this->X = Math::Power<Value>(this->X, Target.X);
				this->Y = Math::Power<Value>(this->Y, Target.Y);
				this->Z = Math::Power<Value>(this->Z, Target.Z);
			}
			CONST_EXPRESSION INLINE void operator%(const Vector3<Value>& Target) {
				this->X = Math::Modulo<Value>(this->X, Target.X);
				this->Y = Math::Modulo<Value>(this->Y, Target.Y);
				this->Z = Math::Modulo<Value>(this->Z, Target.Z);
			}

			CONST_EXPRESSION INLINE void operator^(const Value Target) {
				this->X = Math::Power<Value>(this->X, Target);
				this->Y = Math::Power<Value>(this->Y, Target);
				this->Z = Math::Power<Value>(this->Z, Target);
			}
			CONST_EXPRESSION INLINE void operator%(const Value Target) {
				this->X = Math::Modulo<Value>(this->X, Target);
				this->Y = Math::Modulo<Value>(this->Y, Target);
				this->Z = Math::Modulo<Value>(this->Z, Target);

			}
		};
	}
}
