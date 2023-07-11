#pragma once
#include "../../../Includes.h"
#include "../Bitwise.h"

namespace BigLib {
	namespace DataIntegrity {
		namespace CRC {
			template<
				typename Type,
				const Type Polynomial,
				
				const bool ReflectIn=false,
				const bool ReflectOut=false,
				const Type XORIn=0,
				const Type XOROut=0,
				
				const umax Width=SIZEOF_BITS(Type),
			
				const umax TableLen=256
			>
			class CRCEngineStatic {
			private:
				Type LookupTable[TableLen];
				const Type LimiterMask = Bitwise::MakeBinaryMask<Type>(Width);

				CONST_EXPRESSION void p_GenerateLookupTable() {
					const Type MaxBitMask = (Width < 8 ? 0x80 : UI64(1) << (Width - 1));
					
					Type Poly;
					if CONST_EXPRESSION(Width < 8)
						Poly = (Polynomial << (8 - Width));
					else
						Poly = Polynomial;

					for (umax I = 0; I < TableLen; I++) {
						// Only mirror the first 8 bits, otherwise the left shift will remove the bits.
						Type Remainder = ReflectIn ? Bitwise::BinaryReflect<Type, 8>((Type)I) : (Type)I;

						if CONST_EXPRESSION(Width > 8)
							Remainder <<= (Width - 8);

						for (umax B = 0; B < 8; B++)
							Remainder = (Remainder & MaxBitMask) ? ((Remainder << 1) ^ Poly) : (Remainder << 1);
							
						// Reflecting the table value will have the same effect of ReflectIn on the output.
						this->LookupTable[I] = ReflectIn ? (Bitwise::BinaryReflect<Type, (Width < 8) ? 8 : Width>(Remainder)) : (Remainder);
					}
				}

				FORCE_INLINE void p_NormalUpdateCRC(Type Data) {
					if CONST_EXPRESSION(Width > 8)
						this->CRC = (this->LookupTable[(Data ^ (this->CRC >> (Width - 8))) & 0xFF] ^ (this->CRC << 8)) & this->LimiterMask;
					else
						// (this->CRC << 8) becomes zero if Width <= 8 
						this->CRC = this->LookupTable[(Data ^ this->CRC) & 0xFF];
				}

				FORCE_INLINE void p_InversedUpdateCRC(Type Data) {
					if CONST_EXPRESSION(Width > 8)
						this->CRC = (this->LookupTable[(this->CRC ^ Data) & 0xFF] ^ (this->CRC >> 8)) & this->LimiterMask;
					else
						// (this->CRC >> 8) becomes zero if Width <= 8
						this->CRC = this->LookupTable[(this->CRC ^ Data) & 0xFF];
				}

			public:
				CRCEngineStatic() {
					this->p_GenerateLookupTable();
					this->Reset();
				}

				Type CRC;

				FORCE_INLINE CRCEngineStatic& Reset(Type Initial=Type(0)) {
					if CONST_EXPRESSION(ReflectIn)
						this->CRC = Bitwise::BinaryReflect<Type, Width>(Initial ^ XORIn);
					else
						this->CRC = Initial ^ XORIn;

					if CONST_EXPRESSION(Width < 8 && !ReflectOut)
						this->CRC <<= (8 - Width);

					return *this;
				}

				FORCE_INLINE Type Get() {
					Type CRCOut = this->CRC;

					if CONST_EXPRESSION(Width < 8 && !ReflectIn)
						CRCOut >>= (8 - Width);
					
					CRCOut ^= XOROut;

					if CONST_EXPRESSION(ReflectOut && !ReflectIn)
						return Bitwise::BinaryReflect<Type, Width>(CRCOut) & this->LimiterMask;
					else
						return CRCOut & this->LimiterMask;
				}
				
				FORCE_INLINE CONST Type* GetLookupTable() {
					return (CONST Type*)&this->LookupTable;
				}

				FORCE_INLINE CRCEngineStatic& Update(Type Data) {
					// p_InversedUpdateCRC Needs To Be Used In Case The Table Is Reflected To Fully Emulate ReflectIn
					if CONST_EXPRESSION(ReflectIn)
						p_InversedUpdateCRC(Data);
					else
						p_NormalUpdateCRC(Data);

					return *this;
				}

				template<typename BufferType>
				INLINE CRCEngineStatic& Update(CONST BufferType* Buffer, umax Size) {
					for (umax i = 0; i < Size; i++)
						this->Update((Type)Buffer[i]);
					return *this;
				}
			};


			// Predefined CRCs
			//					Type			Polynomial			RefIn	RefOut	XORIn				XOROut	Width

			typedef CRCEngineStatic<uint8,		0x3,					false,	false,	0x0,					0x7,		3> CRC_3_GSM;
			typedef CRCEngineStatic<uint8,		0x3,					true,	true,	0x7,					0x0,		3> CRC_3_ROHC;

			typedef CRCEngineStatic<uint8,		0x3,					true,	true,	0x0,					0x0,		4> CRC_4_G_704; // AKA CRC-4/ITU
			typedef CRCEngineStatic<uint8,		0x3,					false,	false,	0xF,					0xF,		4> CRC_4_INTERLAKEN;

			typedef CRCEngineStatic<uint8,		0x09,				false,	false,	0x09,				0x00,	5> CRC_5_EPC_C1G2; // AKA CRC-5/EPC
			typedef CRCEngineStatic<uint8,		0x15,				true,	true,	0x00,				0x00,	5> CRC_5_G_704; // AKA CRC-5/ITU
			typedef CRCEngineStatic<uint8,		0x05,				true,	true,	0x1F,				0x1F,	5> CRC_5_USB;

			typedef CRCEngineStatic<uint8,		0x27,				false,	false,	0x3F,				0x00,	6> CRC_6_CDMA2000_A;
			typedef CRCEngineStatic<uint8,		0x07,				false,	false,	0x3F,				0x00,	6> CRC_6_CDMA2000_B;
			typedef CRCEngineStatic<uint8,		0x19,				true,	true,	0x00,				0x00,	6> CRC_6_DARC;
			typedef CRCEngineStatic<uint8,		0x03,				true,	true,	0x00,				0x00,	6> CRC_6_G_704; // AKA CRC-6/ITU
			typedef CRCEngineStatic<uint8,		0x2F,				false,	false,	0x00,				0x3F,	6> CRC_6_GSM;

			typedef CRCEngineStatic<uint8,		0x09,				false,	false,	0x00,				0x00,	7> CRC_7_MMC; // AKA CRC-7
			typedef CRCEngineStatic<uint8,		0x4F,				true,	true,	0x7F,				0x00,	7> CRC_7_ROHC;
			typedef CRCEngineStatic<uint8,		0x45,				false,	false,	0x00,				0x00,	7> CRC_7_UMTS;

			typedef CRCEngineStatic<uint8,		0x2F,				false,	false,	0xFF,				0xFF> CRC_8_AUTOSAR;
			typedef CRCEngineStatic<uint8,		0xA7,				true,	true,	0x00,				0x00> CRC_8_BLUETOOTH;
			typedef CRCEngineStatic<uint8,		0x9B,				false,	false,	0xFF,				0x00> CRC_8_CDMA2000;
			typedef CRCEngineStatic<uint8,		0x39,				true,	true,	0x00,				0x00> CRC_8_DARC;
			typedef CRCEngineStatic<uint8,		0xD5,				false,	false,	0x00,				0x00> CRC_8_DVB_S2;
			typedef CRCEngineStatic<uint8,		0x1D,				false,	false,	0x00,				0x00> CRC_8_GSM_A;
			typedef CRCEngineStatic<uint8,		0x49,				false,	false,	0x00,				0xFF> CRC_8_GSM_B;
			typedef CRCEngineStatic<uint8,		0x1D,				false,	false,	0xFF,				0x00> CRC_8_HITAG;
			typedef CRCEngineStatic<uint8,		0x07,				false,	false,	0x00,				0x55> CRC_8_I_432_1; // AKA CRC-8/ITU
			typedef CRCEngineStatic<uint8,		0x1D,				false,	false,	0xFD,				0x00> CRC_8_I_CODE;
			typedef CRCEngineStatic<uint8,		0x9B,				false,	false,	0x00,				0x00> CRC_8_LTE;
			typedef CRCEngineStatic<uint8,		0x31,				true,	true,	0x00,				0x00> CRC_8_MAXIM_DOW; // AKA CRC-8/MAXIM, DOW-CRC
			typedef CRCEngineStatic<uint8,		0x1D,				false,	false,	0xC7,				0x00> CRC_8_MIFARE_MAD;
			typedef CRCEngineStatic<uint8,		0x31,				false,	false,	0xFF,				0x00> CRC_8_NRSC_5;
			typedef CRCEngineStatic<uint8,		0x2F,				false,	false,	0x00,				0x00> CRC_8_OPENSAFETY;
			typedef CRCEngineStatic<uint8,		0x07,				true,	true,	0xFF,				0x00> CRC_8_ROHC;
			typedef CRCEngineStatic<uint8,		0x1D,				false,	false,	0xFF,				0xFF> CRC_8_SAE_J1850;
			typedef CRCEngineStatic<uint8,		0x07,				false,	false,	0x00,				0x00> CRC_8_SMBUS; // AKA CRC-8
			typedef CRCEngineStatic<uint8,		0x1D,				true,	true,	0xFF,				0x00> CRC_8_TECH_3250; // AKA CRC-8/AES, CRC-8/EBU
			typedef CRCEngineStatic<uint8,		0x9B,				true,	true,	0x00,				0x00> CRC_8_WCDMA;
			
			typedef CRCEngineStatic<uint16,		0x233,				false,	false,	0x000,				0x000,	10> CRC_10_ATM; // AKA CRC-10, CRC-10/I-610
			typedef CRCEngineStatic<uint16,		0x3D9,				false,	false,	0x3FF,				0x000,	10> CRC_10_CDMA2000;
			typedef CRCEngineStatic<uint16,		0x175,				false,	false,	0x000,				0x3FF,	10> CRC_10_GSM;

			typedef CRCEngineStatic<uint16,		0x385,				false,	false,	0x01A,				0x000,	11> CRC_11_FLEXRAY; // AKA CRC-11
			typedef CRCEngineStatic<uint16,		0x307,				false,	false,	0x000,				0x000,	11> CRC_11_UMTS;
			
			typedef CRCEngineStatic<uint16,		0xF13,				false,	false,	0xFFF,				0x000,	12> CRC_12_CDMA2000;
			typedef CRCEngineStatic<uint16,		0x80F,				false,	false,	0x000,				0x000,	12> CRC_12_DECT; // AKA X-CRC-12
			typedef CRCEngineStatic<uint16,		0xD31,				false,	false,	0x000,				0xFFF,	12> CRC_12_GSM;
			typedef CRCEngineStatic<uint16,		0x80F,				false,	true,	0x000,				0x000,	12> CRC_12_UMTS; // AKA CRC-12/3GPP

			typedef CRCEngineStatic<uint16,		0x1CF5,				false,	false,	0x0000,				0x0000,	13> CRC_13_BBC;
			
			typedef CRCEngineStatic<uint16,		0x0805,				true,	true,	0x0000,				0x0000,	14> CRC_14_DARC;
			typedef CRCEngineStatic<uint16,		0x202D,				false,	false,	0x0000,				0x3FFF,	14> CRC_14_GSM;
			
			typedef CRCEngineStatic<uint16,		0x4599,				false,	false,	0x0000,				0x0000,	15> CRC_15_CAN;
			typedef CRCEngineStatic<uint16,		0x6815,				false,	false,	0x0000,				0x0001,	15> CRC_15_MPT1327;

			typedef CRCEngineStatic<uint16,		0x8005,				true,	true,	0x0000,				0x0000> CRC_16_ARC; // AKA CRC-16, CRC-16/LHA, CRC-IBM
			typedef CRCEngineStatic<uint16,		0xC867,				false,	false,	0xFFFF,				0x0000> CRC_16_CDMA2000;
			typedef CRCEngineStatic<uint16,		0x8005,				false,	false,	0xFFFF,				0x0000> CRC_16_CMS;
			typedef CRCEngineStatic<uint16,		0x8005,				false,	false,	0x800D,				0x0000> CRC_16_DDS_110;
			typedef CRCEngineStatic<uint16,		0x0589,				false,	false,	0x0000,				0x0001> CRC_16_DECT_R;
			typedef CRCEngineStatic<uint16,		0x0589,				false,	false,	0x0000,				0x0000> CRC_16_DECT_X;
			typedef CRCEngineStatic<uint16,		0x3D65,				true,	true,	0x0000,				0xFFFF> CRC_16_DNP;
			typedef CRCEngineStatic<uint16,		0x3D65,				false,	false,	0x0000,				0xFFFF> CRC_16_EN_13757;
			typedef CRCEngineStatic<uint16,		0x1021,				false,	false,	0xFFFF,				0xFFFF> CRC_16_GENIBUS; // AKA CRC-16/DARC, CRC-16/EPC, CRC-16/EPC-C1G2, CRC-16/I-CODE
			typedef CRCEngineStatic<uint16,		0x1021,				false,	false,	0x0000,				0xFFFF> CRC_16_GSM;
			typedef CRCEngineStatic<uint16,		0x1021,				false,	false,	0xFFFF,				0x0000> CRC_16_IBM_3740; // AKA CRC-16/AUTOSAR, CRC-16/CCITT-FALSE
			typedef CRCEngineStatic<uint16,		0x1021,				true,	true,	0xFFFF,				0xFFFF> CRC_16_IBM_SDLC; // AKA CRC-16/ISO-HDLC, CRC-16/ISO-IEC-14443-3-B, CRC-16/X-25, CRC-B, X-25
			typedef CRCEngineStatic<uint16,		0x1021,				true,	true,	0xC6C6,				0x0000> CRC_16_ISO_IEC_14443_3_A; // AKA CRC-A
			typedef CRCEngineStatic<uint16,		0x1021,				true,	true,	0x0000,				0x0000> CRC_16_KERMIT; // AKA CRC-16/BLUETOOTH, CRC-16/CCITT, CRC-16/CCITT-TRUE, CRC-16/V-41-LSB, CRC-CCITT
			typedef CRCEngineStatic<uint16,		0x6F63,				false,	false,	0x0000,				0x0000> CRC_16_LJ1200;
			typedef CRCEngineStatic<uint16,		0x5935,				false,	false,	0xFFFF,				0x0000> CRC_16_M17;
			typedef CRCEngineStatic<uint16,		0x8005,				true,	true,	0x0000,				0xFFFF> CRC_16_MAXIM_DOW; // AKA CRC-16/MAXIM
			typedef CRCEngineStatic<uint16,		0x1021,				true,	true,	0xFFFF,				0x0000> CRC_16_MCRF4XX;
			typedef CRCEngineStatic<uint16,		0x8005,				true,	true,	0xFFFF,				0x0000> CRC_16_MODBUS;
			typedef CRCEngineStatic<uint16,		0x080B,				true,	true,	0xFFFF,				0x0000> CRC_16_NRSC_5;
			typedef CRCEngineStatic<uint16,		0x5935,				false,	false,	0x0000,				0x0000> CRC_16_OPENSAFETY_A;
			typedef CRCEngineStatic<uint16,		0x755B,				false,	false,	0x0000,				0x0000> CRC_16_OPENSAFETY_B;
			typedef CRCEngineStatic<uint16,		0x1DCF,				false,	false,	0xFFFF,				0xFFFF> CRC_16_PROFIBUS; // AKA CRC-16/IEC-61158-2
			typedef CRCEngineStatic<uint16,		0x1021,				true,	true,	0xB2AA,				0x0000> CRC_16_RIELLO;
			typedef CRCEngineStatic<uint16,		0x1021,				false,	false,	0x1D0F,				0x0000> CRC_16_SPI_FUJITSU; // AKA CRC-16/AUG-CCITT
			typedef CRCEngineStatic<uint16,		0x8BB7,				false,	false,	0x0000,				0x0000> CRC_16_T10_DIF;
			typedef CRCEngineStatic<uint16,		0xA097,				false,	false,	0x0000,				0x0000> CRC_16_TELEDISK;
			typedef CRCEngineStatic<uint16,		0x1021,				true,	true,	0x89EC,				0x0000> CRC_16_TMS37157;
			typedef CRCEngineStatic<uint16,		0x8005,				false,	false,	0x0000,				0x0000> CRC_16_UMTS; // AKA CRC-16/BUYPASS, CRC-16/VERIFONE
			typedef CRCEngineStatic<uint16,		0x8005,				true,	true,	0xFFFF,				0xFFFF> CRC_16_USB;
			typedef CRCEngineStatic<uint16,		0x1021,				false,	false,	0x0000,				0x0000> CRC_16_XMODEM; // AKA CRC-16/ACORN, CRC-16/LTE, CRC-16/V-41-MSB, XMODEM, ZMODEM

			typedef CRCEngineStatic<uint32,		0x1685B,				false,	false,	0x00000,				0x00000,		17> CRC_17_CAN_FD;

			typedef CRCEngineStatic<uint32,		0x102899,			false,	false,	0x000000,			0x000000,	21> CRC_21_CAN_FD;
			
			typedef CRCEngineStatic<uint32,		0x00065B,			true,	true,	0x555555,			0x000000,	24> CRC_24_BLE;
			typedef CRCEngineStatic<uint32,		0x5D6DCB,			false,	false,	0xFEDCBA,			0x000000,	24> CRC_24_FLEXRAY_A;
			typedef CRCEngineStatic<uint32,		0x5D6DCB,			false,	false,	0xABCDEF,			0x000000,	24> CRC_24_FLEXRAY_B;
			typedef CRCEngineStatic<uint32,		0x328B63,			false,	false,	0xFFFFFF,			0xFFFFFF,	24> CRC_24_INTERLAKEN;
			typedef CRCEngineStatic<uint32,		0x864CFB,			false,	false,	0x000000,			0x000000,	24> CRC_24_LTE_A;
			typedef CRCEngineStatic<uint32,		0x800063,			false,	false,	0x000000,			0x000000,	24> CRC_24_LTE_B;
			typedef CRCEngineStatic<uint32,		0x864CFB,			false,	false,	0xB704CE,			0x000000,	24> CRC_24_OPENPGP; // AKA CRC-24
			typedef CRCEngineStatic<uint32,		0x800063,			false,	false,	0xFFFFFF,			0xFFFFFF,	24> CRC_24_OS_9;

			typedef CRCEngineStatic<uint32,		0x2030B9C7,			false,	false,	0x3FFFFFFF,			0x3FFFFFFF,	30> CRC_30_CDMA;
			
			typedef CRCEngineStatic<uint32,		0x04C11DB7,			false,	false,	0x7FFFFFFF,			0x7FFFFFFF,	31> CRC_31_PHILIPS; 

			typedef CRCEngineStatic<uint32,		0x814141AB,			false,	false,	0x00000000,			0x00000000> CRC_32_AIXM; // AKA CRC-32Q
			typedef CRCEngineStatic<uint32,		0xF4ACFB13,			true,	true,	0xFFFFFFFF,			0xFFFFFFFF> CRC_32_AUTOSAR;
			typedef CRCEngineStatic<uint32,		0xA833982B,			true,	true,	0xFFFFFFFF,			0xFFFFFFFF> CRC_32_BASE91_D; // AKA CRC-32D
			typedef CRCEngineStatic<uint32,		0x04C11DB7,			false,	false,	0xFFFFFFFF,			0xFFFFFFFF> CRC_32_BZIP2; // AKA CRC-32/AAL5, CRC-32/DECT-B, B-CRC-32
			typedef CRCEngineStatic<uint32,		0x8001801B,			true,	true,	0x00000000,			0x00000000> CRC_32_CD_ROM_EDC;
			typedef CRCEngineStatic<uint32,		0x04C11DB7,			false,	false,	0x00000000,			0xFFFFFFFF> CRC_32_CKSUM; // AKA CRC-32/POSIX
			typedef CRCEngineStatic<uint32,		0x1EDC6F41,			true,	true,	0xFFFFFFFF,			0xFFFFFFFF> CRC_32_ISCSI; // AKA CRC-32/BASE91-C, CRC-32/CASTAGNOLI, CRC-32/INTERLAKEN, CRC-32C
			typedef CRCEngineStatic<uint32,		0x04C11DB7,			true,	true,	0xFFFFFFFF,			0xFFFFFFFF> CRC_32_ISO_HDLC; // AKA CRC-32, CRC-32/ADCCP, CRC-32/V-42, CRC-32/XZ, PKZIP
			typedef CRCEngineStatic<uint32,		0x04C11DB7,			true,	true,	0xFFFFFFFF,			0x00000000> CRC_32_JAMCRC;
			typedef CRCEngineStatic<uint32,		0x741B8CD7,			true,	true,	0xFFFFFFFF,			0x00000000> CRC_32_MEF;
			typedef CRCEngineStatic<uint32,		0x04C11DB7,			false,	false,	0xFFFFFFFF,			0x00000000> CRC_32_MPEG_2;
			typedef CRCEngineStatic<uint32,		0x000000AF,			false,	false,	0x00000000,			0x00000000> CRC_32_XFER;
			
			typedef CRCEngineStatic<uint64,		0x0004820009,			false,	false,	0x0000000000,			0xFFFFFFFFFF,	40> CRC_40_GSM;

			typedef CRCEngineStatic<uint64,		0x42F0E1EBA9EA3693,	false,	false,	0x0000000000000000,	0x0000000000000000> CRC_64_ECMA_182; // CRC-64
			typedef CRCEngineStatic<uint64,		0x000000000000001B,	true,	true,	0xFFFFFFFFFFFFFFFF,	0xFFFFFFFFFFFFFFFF> CRC_64_GO_ISO;
			typedef CRCEngineStatic<uint64,		0x259C84CBA6426349,	true,	true,	0xFFFFFFFFFFFFFFFF,	0x0000000000000000> CRC_64_MS;
			typedef CRCEngineStatic<uint64,		0xAD93D23594C935A9,	true,	true,	0x0000000000000000,	0x0000000000000000> CRC_64_REDIS;
			typedef CRCEngineStatic<uint64,		0x42F0E1EBA9EA3693,	false,	false,	0xFFFFFFFFFFFFFFFF,	0xFFFFFFFFFFFFFFFF> CRC_64_WE;
			typedef CRCEngineStatic<uint64,		0x42F0E1EBA9EA3693,	true,	true,	0xFFFFFFFFFFFFFFFF,	0xFFFFFFFFFFFFFFFF> CRC_64_XZ;
		}
	}
}