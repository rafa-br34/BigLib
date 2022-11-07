#pragma once
#include "../../../Includes.h"
#include "../../Math/Math.h"

namespace BigLib {
	namespace Utils {
		class Color24 {
			uint8_t R, G, B;

			CONST_EXPRESSION INLINE Color24()
				: R()
				, G()
				, B()
			{};
			CONST_EXPRESSION INLINE Color24(uint8_t R, uint8_t G, uint8_t B)
				: R(R)
				, G(G)
				, B(B)
			{};


			CONST_EXPRESSION INLINE Color24& Blend(Color24& Target, double Power) {
				this->R = (uint8_t)Math::Lerp<double>((double)this->R, (double)Target.R, Power);
				this->G = (uint8_t)Math::Lerp<double>((double)this->G, (double)Target.G, Power);
				this->B = (uint8_t)Math::Lerp<double>((double)this->B, (double)Target.B, Power);
				return *this;
			}
			CONST_EXPRESSION INLINE Color24& Blend(Color24& Target, float Power) {
				// Floats Will Be Faster Sometimes
				this->R = (uint8_t)Math::Lerp<float>((float)this->R, (float)Target.R, Power);
				this->G = (uint8_t)Math::Lerp<float>((float)this->G, (float)Target.G, Power);
				this->B = (uint8_t)Math::Lerp<float>((float)this->B, (float)Target.B, Power);
				return *this;
			}


			// RGB(Red, Green, Blue) Color (0-255)
			CONST_EXPRESSION INLINE Color24& FromRGB(uint8_t Red, uint8_t Green, uint8_t Blue) {
				this->B = Red;
				this->G = Green;
				this->B = Blue;
				return *this;
			}
			// RGB(Red, Green, Blue) Color (0.0-1.0)
			CONST_EXPRESSION INLINE Color24& FromRGB(double Red, double Green, double Blue) {
				this->R = (uint8_t)Math::Lerp<double>((double)this->R, 255.0, Red);
				this->G = (uint8_t)Math::Lerp<double>((double)this->G, 255.0, Green);
				this->B = (uint8_t)Math::Lerp<double>((double)this->B, 255.0, Blue);
				return *this;
			}
			// RGB(Red, Green, Blue) Color (0.0-1.0)
			CONST_EXPRESSION INLINE Color24& FromRGB(float Red, float Green, float Blue) {
				this->R = (uint8_t)Math::Lerp<float>((float)this->R, 255.f, Red);
				this->G = (uint8_t)Math::Lerp<float>((float)this->G, 255.f, Green);
				this->B = (uint8_t)Math::Lerp<float>((float)this->B, 255.f, Blue);
				return *this;
			}

			// HSL(Hue, Saturation, Lightness) Color (0.0-1.0)
			CONST_EXPRESSION INLINE Color24& FromHSL(double Hue, double Saturation, double Lightness) {
				Hue = Hue * 360.0;

				while (Hue > 360.0) {
					Hue -= 360.0;
				}

				auto C = Saturation * Lightness;
				auto X = C * (1.0 - Math::Absolute(Math::Modulo(Hue / 60.0, 2.0) - 1.0));
				auto SaturationKey = Lightness - C;
				double Red, Green, Blue;

				if (Hue >= 0.0 && Hue < 60.0) {
					Red = C;
					Green = X;
					Blue = 0.0;
				}
				else if (Hue >= 60.0 && Hue < 120.0) {
					Red = X;
					Green = C;
					Blue = 0.0;
				}
				else if (Hue >= 120.0 && Hue < 180.0) {
					Red = 0.0;
					Green = C;
					Blue = X;
				}
				else if (Hue >= 180.0 && Hue < 240.0) {
					Red = 0.0;
					Green = X;
					Blue = C;
				}
				else if (Hue >= 240.0 && Hue < 300.0) {
					Red = X;
					Green = 0.0;
					Blue = C;
				}
				else {
					Red = C;
					Green = 0.0;
					Blue = X;
				}

				this->FromRGB(
					(Red + SaturationKey),
					(Green + SaturationKey),
					(Blue + SaturationKey)
				);
				return *this;
			}

			// HSL(Hue, Saturation, Lightness) Color (0.0-1.0)
			CONST_EXPRESSION INLINE Color24& FromHSL(float Hue, float Saturation, float Lightness) {
				Hue = Hue * 360.f;

				while (Hue > 360.f) {
					Hue -= 360.f;
				}

				auto C = Saturation * Lightness;
				auto X = C * (1.0 - Math::Absolute(Math::Modulo(Hue / 60.f, 2.f) - 1.f));
				auto SaturationKey = Lightness - C;
				float Red, Green, Blue;

				if (Hue >= 0.f && Hue < 60.f) {
					Red = C;
					Green = X;
					Blue = 0.f;
				}
				else if (Hue >= 60.f && Hue < 120.f) {
					Red = X;
					Green = C;
					Blue = 0.f;
				}
				else if (Hue >= 120.f && Hue < 180.f) {
					Red = 0.f;
					Green = C;
					Blue = X;
				}
				else if (Hue >= 180.f && Hue < 240.f) {
					Red = 0.f;
					Green = X;
					Blue = C;
				}
				else if (Hue >= 240.f && Hue < 300.f) {
					Red = X;
					Green = 0.f;
					Blue = C;
				}
				else {
					Red = C;
					Green = 0.f;
					Blue = X;
				}

				this->FromRGB(
					(Red + SaturationKey),
					(Green + SaturationKey),
					(Blue + SaturationKey)
				);
				return *this;
			}

			// CMYK(Cyan, Magenta, Yellow, Key) Color (0.0-1.0)
			CONST_EXPRESSION INLINE Color24& FromCMYK(double Cyan, double Magenta, double Yellow, double Key) {
				this->R = (uint8_t)(255.0 * (1.0 - Cyan) * (1.0 - Key));
				this->G = (uint8_t)(255.0 * (1.0 - Magenta) * (1.0 - Key));
				this->B = (uint8_t)(255.0 * (1.0 - Yellow) * (1.0 - Key));
				return *this;
			}
			// CMYK(Cyan, Magenta, Yellow, Key) Color (0-255)
			CONST_EXPRESSION INLINE Color24& FromCMYK(uint8_t Cyan, uint8_t Magenta, uint8_t Yellow, uint8_t Key) {
				uint8_t
					R = 255 - Cyan,
					G = 255 - Magenta,
					B = 255 - Yellow;
				this->R = R - (Key * (R >= Key));
				this->G = G - (Key * (G >= Key));
				this->B = B - (Key * (B >= Key));
				return *this;
			}

		};
	}
}