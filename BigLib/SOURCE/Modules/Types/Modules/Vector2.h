#pragma once
#include "../../Data/Math.h"

namespace BigLib {
	namespace Types {
		template<typename Value>
		class Vector2 {
		public:
			Value X, Y;
			
			CONST_EXPRESSION INLINE Vector2<Value>()
				: X()
				, Y()
			{};
			CONST_EXPRESSION INLINE Vector2<Value>(Value X, Value Y)
				: X(X)
				, Y(Y)
			{};
			
			// Class Functions
			Vector2<Value>& Lerp(Vector2<Value>& Target, Value Interpolation) {
				this->X = Math::Lerp(this->X, Target.X, Interpolation);
				this->Y = Math::Lerp(this->Y, Target.Y, Interpolation);
				return *this;
			}
			Value Dot(Vector2<Value> Target) {
				Value First[] = { this->X, this->Y };
				Value Second[] = { Target.X, Target.Y };
				return Math::Dot(First, Second, 2);
			}
			template<typename RetValue=double>
			RetValue Magnitude() {
				return Math::SquareRoot((RetValue)Math::Power(this->X, 2) + (RetValue)Math::Power(this->Y, 2));
			}

			// Comparation Functions
			CONST_EXPRESSION INLINE bool operator==(const Vector2<Value>& Target) const {
				return (this->X == Target.X) && (this->Y == Target.Y);
			}
			CONST_EXPRESSION INLINE bool operator!=(const Vector2<Value>& Target) const {
				return (this->X != Target.X) && (this->Y != Target.Y);
			}

			CONST_EXPRESSION INLINE bool operator>=(const Vector2<Value>& Target) const {
				return (this->X >= Target.X) && (this->Y >= Target.Y);
			}
			CONST_EXPRESSION INLINE bool operator<=(const Vector2<Value>& Target) const {
				return (this->X <= Target.X) && (this->Y <= Target.Y);
			}

			CONST_EXPRESSION INLINE bool operator>(const Vector2<Value>& Target) const {
				return (this->X > Target.X) && (this->Y > Target.Y);
			}
			CONST_EXPRESSION INLINE bool operator<(const Vector2<Value>& Target) const {
				return (this->X < Target.X) && (this->Y < Target.Y);
			}

			// Math + & -
			CONST_EXPRESSION INLINE void operator+(const Vector2<Value>& Target) {
				this->X += Target.X; this->Y += Target.Y;
			}
			CONST_EXPRESSION INLINE void operator-(const Vector2<Value>& Target) {
				this->X -= Target.X; this->Y -= Target.Y;
			}

			CONST_EXPRESSION INLINE void operator+(const Value Target) {
				this->X += Target; this->Y += Target;
			}
			CONST_EXPRESSION INLINE void operator-(const Value Target) {
				this->X -= Target; this->Y -= Target;
			}

			// Math * & /
			CONST_EXPRESSION INLINE void operator*(const Vector2<Value>& Target) {
				this->X *= Target.X; this->Y *= Target.Y;
			}
			CONST_EXPRESSION INLINE void operator/(const Vector2<Value>& Target) {
				this->X /= Target.X; this->Y /= Target.Y;
			}

			CONST_EXPRESSION INLINE void operator*(const Value Target) {
				this->X *= Target; this->Y *= Target;
			}
			CONST_EXPRESSION INLINE void operator/(const Value Target) {
				this->X /= Target; this->Y /= Target;
			}

			// Math ^ & %
			CONST_EXPRESSION INLINE void operator^(const Vector2<Value>& Target) {
				this->X = Math::Power<Value>(this->X, Target.X); this->Y = Math::Power<Value>(this->Y, Target.Y);
			}
			CONST_EXPRESSION INLINE void operator%(const Vector2<Value>& Target) {
				this->X = Math::Modulo<Value>(this->X, Target.X); this->Y = Math::Modulo<Value>(this->Y, Target.Y);
			}

			CONST_EXPRESSION INLINE void operator^(const Value Target) {
				this->X = Math::Power<Value>(this->X, Target); this->Y = Math::Power<Value>(this->Y, Target);
			}
			CONST_EXPRESSION INLINE void operator%(const Value Target) {
				this->X = Math::Modulo<Value>(this->X, Target); this->Y = Math::Modulo<Value>(this->Y, Target);
				
			}
		};
	}
}
