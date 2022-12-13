#pragma once
#include "../../../Includes.h"
#include "../../Data/Bitwise.h"

namespace BigLib {
	namespace Crypts {
		namespace CRC {
			template<
				typename Type,
				const Type Polynomial,
				const bool ReflectPolynomial,
				
				const bool ReflectIn=false,
				const bool ReflectOut=false,
				const Type Initial=0,
				const Type XOROut=0,
				
				const size_t Width=SIZEOF_BITS(Type),
				const size_t TableLen=256
			>
			class CRC_Base {
			private:
				Type LookupTable[TableLen];
				const Type LimiterMask = Bitwise::MakeBinaryMask<Type>(Width);
				const Type MSBMask = 1 << (Width - 1);

				CONST_EXPRESSION void GenerateLookupTable() {
					for (size_t Dividend = 0; Dividend < TableLen; Dividend++) {
						// Only Mirror The First 8 Bits Here, Otherwise The Left Shift Will Remove Our Bits.
						Type Remainder = (ReflectIn ? Bitwise::BinaryReflect<Type, 8>((Type)Dividend) : (Type)Dividend);
						
						if CONST_EXPRESSION(Width > 8)
							Remainder = Remainder << (Width - 8);

						for (size_t i = 0; i < 8; i++) {							
							
							if (Remainder & this->MSBMask)
								// Hopefully The Compiler Will Optimize.
								Remainder = (Remainder << 1) ^ (ReflectPolynomial ? Bitwise::BinaryReflect<Type, Width>(Polynomial) : Polynomial);
							else
								Remainder = (Remainder << 1);
						}
						// Reflecting The Table Item Will Have The Same Effect Of ReflectIn.
						this->LookupTable[Dividend] = (ReflectIn ? Bitwise::BinaryReflect<Type, Width>(Remainder) : Remainder) & this->LimiterMask;
					}
				}

				CONST_EXPRESSION FORCE_INLINE void NormalUpdateCRC(Type Data) {
					if CONST_EXPRESSION(Width > 8)
						this->CRC = this->LookupTable[(Data ^ (this->CRC >> (Width - 8)) & 0xFF)] ^ (this->CRC << 8);
					else
						// (this->CRC << 8) Becomes Zero If Width <= 8 
						// If Width Is Less Than 8 There Will Be Problems With (Width - 8).
						if CONST_EXPRESSION(Width < 8)
							this->CRC = this->LookupTable[(Data ^ this->CRC) & 0xFF];
						else
							this->CRC = this->LookupTable[(Data ^ (this->CRC >> (Width - 8))) & 0xFF];
					this->CRC &= this->LimiterMask;
				}

				CONST_EXPRESSION FORCE_INLINE void InversedUpdateCRC(Type Data) {
					if CONST_EXPRESSION (Width > 8)
						this->CRC = this->LookupTable[((this->CRC ^ Data) & 0xFF)] ^ (this->CRC >> 8);
					else
						// (this->CRC >> 8) Becomes Zero When Width Is Lower Or Equal To 8 
						this->CRC = this->LookupTable[((this->CRC ^ Data) & 0xFF)];
				}

			public:
				CRC_Base() {
					GenerateLookupTable();
				}

				Type CRC = Initial;

				CONST_EXPRESSION FORCE_INLINE void ResetCRC() {
					this->CRC = Initial;
				}

				CONST_EXPRESSION FORCE_INLINE Type GetCRC() {
					if CONST_EXPRESSION(ReflectOut && !ReflectIn)
						return Bitwise::BinaryReflect<Type, Width>(this->CRC ^ XOROut);
					else
						return this->CRC ^ XOROut;
				}

				CONST_EXPRESSION FORCE_INLINE Type& UpdateCRC(Type Data) {
					// InversedUpdateCRC Needs To Be Used In Case The Table Is Reflected To Fully Emulate ReflectIn
					if CONST_EXPRESSION(ReflectIn)
						InversedUpdateCRC(Data); 
					else
						NormalUpdateCRC(Data);

					return this->CRC;
				}

				CONST_EXPRESSION FORCE_INLINE const Type* GetLookupTable() {
					return (const Type*)&this->LookupTable;
				}

				template<typename BufferType>
				CONST_EXPRESSION INLINE void UpdateCRC(const BufferType* Buffer, size_t Size) {
					for (size_t i = 0; i < Size; i++)
						this->UpdateCRC((Type)Buffer[i]);
				}

				
			};


			// Predefined CRCs
			//				 Type		Polynomial	RefPoly		RefIn	RefOut	Initial		XOROut Width TableLen

			typedef CRC_Base<uint8_t,	0x9B,		false,		false,	false,	0xFF,		0x00> CRC_8_CDMA2000;
			typedef CRC_Base<uint8_t,	0x9B,		false,		true,	true,	0x00,		0x00> CRC_8_WCDMA;		// Mobile Networks
			typedef CRC_Base<uint8_t,	0xD5,		false,		false,	false,	0x00,		0x00> CRC_8_DBV_S2;		// Digital Video Broadcasting - Satellite - Second Generation (DVB-S2)
			typedef CRC_Base<uint8_t,	0x31,		false,		true,	true,	0x00,		0x00> CRC_8_MAXIM;		// 1-Wire Bus
			typedef CRC_Base<uint8_t,	0x39,		false,		true,	true,	0x00,		0x00> CRC_8_DARC;		// Data Channel Radio
			typedef CRC_Base<uint8_t,	0x1D,		false,		false,	false,	0xFD,		0x00> CRC_8_I_CODE;		// Philips Semiconductors(SL2 ICS11)
			typedef CRC_Base<uint8_t,	0x1D,		false,		true,	true,	0xFF,		0x00> CRC_8_EBU;		// AKA CRC-8/AES Or CRC-8/TECH-3250
			typedef CRC_Base<uint8_t,	0x07,		false,		true,	true,	0xFF,		0x00> CRC_8_ROHC;
			typedef CRC_Base<uint8_t,	0x07,		false,		false,	false,	0x00,		0x55> CRC_8_ITU;		// AKA CRC-8/I-432-1, Used As The Asynchronous Transfer Mode Header Error Control Sequence
			typedef CRC_Base<uint8_t,	0x07,		false,		false,	false,	0x00,		0x00> CRC_8_SMBUS;		
			typedef CRC_Base<uint8_t,	0xA7,		false,		true,	true,	0x00,		0x00> CRC_8_BLUETOOTH;	// Bluetooth Header Error Correction
			typedef CRC_Base<uint8_t,	0x2F,		false,		false,	false,	0xFF,		0xFF> CRC_8_AUTOSAR;
			typedef CRC_Base<uint8_t,	0x1D,		false,		false,	false,	0x00,		0x00> CRC_8_GSM_A;
			typedef CRC_Base<uint8_t,	0x49,		false,		false,	false,	0x00,		0xFF> CRC_8_GSM_B;
			typedef CRC_Base<uint8_t,	0x1D,		false,		false,	false,	0xFF,		0x00> CRC_8_HITAG;		// Used In RFID Applications
			typedef CRC_Base<uint8_t,	0x1D,		false,		false,	false,	0xC7,		0x00> CRC_8_MIFARE_MAD; // NPX Semi-Conductors
			typedef CRC_Base<uint8_t,	0x9B,		false,		false,	false,	0x00,		0x00> CRC_8_LTE;
			typedef CRC_Base<uint8_t,	0x31,		false,		false,	false,	0xFF,		0x00> CRC_8_NRSC_5;
			typedef CRC_Base<uint8_t,	0x2F,		false,		false,	false,	0x00,		0x00> CRC_8_OPENSAFETY;
			typedef CRC_Base<uint8_t,	0x1D,		false,		false,	false,	0xFF,		0xFF> CRC_8_SAE_J1850;

			typedef CRC_Base<uint16_t,	0x233,		false,		false,	false,	0x000,		0x000,	10> CRC_10_ATM;
			typedef CRC_Base<uint16_t,	0x3D9,		false,		false,	false,	0x3FF,		0x000,	10> CRC_10_CDMA2000;
			typedef CRC_Base<uint16_t,	0x175,		false,		false,	false,	0x000,		0x3FF,	10> CRC_10_GSM;

			typedef CRC_Base<uint16_t,	0x385,		false,		false,	false,	0x01A,		0x000,	11> CRC_11_FLEXRAY;
			typedef CRC_Base<uint16_t,	0x307,		false,		false,	false,	0x000,		0x000,	11> CRC_11_UMTS;
			
			typedef CRC_Base<uint16_t,	0xF13,		false,		false,	false,	0xFFF,		0x000,	12> CRC_12_CDMA2000;
			typedef CRC_Base<uint16_t,	0x80F,		false,		false,	false,	0x000,		0x000,	12> CRC_12_DECT;
			typedef CRC_Base<uint16_t,	0xD31,		false,		false,	false,	0x000,		0xFFF,	12> CRC_12_GSM;
			typedef CRC_Base<uint16_t,	0x80F,		false,		false,	true,	0x000,		0x000,	12> CRC_12_UMTS;

			typedef CRC_Base<uint16_t,	0x1CF5,		false,		false,	false,	0x0000,		0x0000,	13> CRC_13_BBC;
			
			typedef CRC_Base<uint16_t,	0x0805,		false,		true,	true,	0x0000,		0x0000,	14> CRC_14_DARC;
			typedef CRC_Base<uint16_t,	0x202D,		false,		false,	false,	0x0000,		0x3FFF,	14> CRC_14_GSM;

			typedef CRC_Base<uint16_t,	0x4599,		false,		false,	false,	0x0000,		0x0000,	15> CRC_15_CAN;
			typedef CRC_Base<uint16_t,	0x6815,		false,		false,	false,	0x0000,		0x0001,	15> CRC_15_MPT1327;

			typedef CRC_Base<uint16_t,	0x8005,		false,		true,	true,	0x0000,		0x0000> CRC_16_ARC;
			typedef CRC_Base<uint16_t,	0xC867,		false,		false,	false,	0xFFFF,		0x0000> CRC_16_CDMA2000;
			typedef CRC_Base<uint16_t,	0x8005,		false,		false,	false,	0xFFFF,		0x0000> CRC_16_CMS;
			typedef CRC_Base<uint16_t,	0x8005,		false,		false,	false,	0x800D,		0x0000> CRC_16_DDS_110;
			typedef CRC_Base<uint16_t,	0x0589,		false,		false,	false,	0x0000,		0x0001> CRC_16_DECT_R;
			typedef CRC_Base<uint16_t,	0x0589,		false,		false,	false,	0x0000,		0x0000> CRC_16_DECT_X;
			typedef CRC_Base<uint16_t,	0x3D65,		false,		true,	true,	0x0000,		0xFFFF> CRC_16_DNP;
			typedef CRC_Base<uint16_t,	0x3D65,		false,		false,	false,	0x0000,		0xFFFF> CRC_16_EN_13757;
			typedef CRC_Base<uint16_t,	0x1021,		false,		false,	false,	0xFFFF,		0xFFFF> CRC_16_GENIBUS;
			typedef CRC_Base<uint16_t,	0x1021,		false,		false,	false,	0x0000,		0xFFFF> CRC_16_GSM;
			typedef CRC_Base<uint16_t,	0x1021,		false,		false,	false,	0xFFFF,		0x0000> CRC_16_IBM_3740;

			typedef CRC_Base<uint32_t,	0x04C11DB7, false,		true,	true,	0xFFFFFFFF, 0xFFFFFFFF> CRC_32;
		}
	}
}