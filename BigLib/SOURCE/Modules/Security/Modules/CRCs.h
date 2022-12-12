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

						// We Want Only To Mirror The First 8 Bits Here, Otherwise The Left Shift Will Remove Our Bits.
						Type Remainder = ((ReflectIn ? Bitwise::BinaryReflect<Type, 8>((Type)Dividend) : (Type)Dividend) << (Width - 8));

						for (size_t i = 0; i < 8; i++) {
							if (Remainder & this->MSBMask)
								// Hopefully The Compiler Will Optimize.
								Remainder = (Remainder << 1) ^ (ReflectPolynomial ? Bitwise::BinaryReflect<Type>(Polynomial) : Polynomial);
							else
								Remainder = (Remainder << 1);
						}
						// Reflecting The Table Item Will Have The Same Effect Of ReflectIn.
						this->LookupTable[Dividend] = (ReflectIn ? Bitwise::BinaryReflect<Type>(Remainder) : Remainder);
					}
				}

				CONST_EXPRESSION FORCE_INLINE void NormalUpdateCRC(Type Data) {
					this->CRC = this->LookupTable[Data ^ (this->CRC >> (Width - 8))] ^ (this->CRC << 8);
				}

				CONST_EXPRESSION FORCE_INLINE void InversedUpdateCRC(Type Data) {
					if CONST_EXPRESSION (Width > 8)
						this->CRC = this->LookupTable[((this->CRC ^ Data) & 0xFF)] ^ (this->CRC >> 8);
					else
						// ^ (this->CRC >> 8) Ins't Used If Width <= 8 
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
					return this->CRC ^ XOROut;
				}

				CONST_EXPRESSION FORCE_INLINE Type& UpdateCRC(Type Data) {
					// InversedUpdateCRC Can Be Used To Emulate ReflectIn
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
			typedef CRC_Base<uint8_t,	0x07,		false,		false,	false,	0x00,		0x00> CRC_8;			// AKA CRC-8/SMBUS
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
			


			typedef CRC_Base<uint32_t,	0x04C11DB7, false,		true,	true,	0xFFFFFFFF, 0xFFFFFFFF> CRC_32;
		}
	}
}