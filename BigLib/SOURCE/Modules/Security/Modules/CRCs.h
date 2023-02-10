#pragma once
#include "../../../Includes.h"
#include "../../Data/Bitwise.h"

namespace BigLib {
	namespace Crypts {
		namespace CRC {

			// TODO: Fix CRCs With Width Lower Than 8
			template<
				typename Type,
				const Type Polynomial,
				
				const bool ReflectIn=false,
				const bool ReflectOut=false,
				const Type Initial=0,
				const Type XOROut=0,
				
				const size_t Width=SIZEOF_BITS(Type),
			
				const size_t TableLen=256
			>
			class CRCEngine {
			private:
				Type LookupTable[TableLen];
				const Type LimiterMask = Bitwise::MakeBinaryMask<Type>(Width);

				CONST_EXPRESSION void GenerateLookupTable() {
					const Type MaxBitMask = (Width < 8 ? 0x80 : UI64(1) << (Width - 1));
					
					Type Poly;
					if CONST_EXPRESSION(Width < 8)
						Poly = (Polynomial << (8 - Width));
					else
						Poly = Polynomial;

					for (size_t I = 0; I < TableLen; I++) {
						// Only Mirror The First 8 Bits Here, Otherwise The Left Shift Will Remove Our Bits.
						Type Remainder = ReflectIn ? Bitwise::BinaryReflect<Type, 8>((Type)I) : (Type)I;

						if CONST_EXPRESSION(Width > 8)
							Remainder <<= (Width - 8);

						for (size_t B = 0; B < 8; B++)
							//Remainder = (Remainder & 1) ? ((Remainder >> 1) ^ Poly) : (Remainder >> 1);
							Remainder = (Remainder & MaxBitMask) ? ((Remainder << 1) ^ Poly) : (Remainder << 1);
							
						// Reflecting The Table Item Will Have The Same Effect Of ReflectIn.
						this->LookupTable[I] = ReflectIn ? (Bitwise::BinaryReflect<Type, (Width < 8) ? 8 : Width>(Remainder)) : (Remainder);
					}
				}

				CONST_EXPRESSION FORCE_INLINE void NormalUpdateCRC(Type Data) {
					if CONST_EXPRESSION(Width > 8)
						this->CRC = (this->LookupTable[(Data ^ (this->CRC >> (Width - 8))) & 0xFF] ^ (this->CRC << 8)) & this->LimiterMask;
					else
						// (this->CRC << 8) Becomes Zero If Width <= 8 
						// If Width Is Less Than Or Equal To 8 It Should Be Removed.
						this->CRC = this->LookupTable[(Data ^ this->CRC) & 0xFF];
				}

				CONST_EXPRESSION FORCE_INLINE void InversedUpdateCRC(Type Data) {
					if CONST_EXPRESSION(Width > 8)
						this->CRC = (this->LookupTable[(this->CRC ^ Data) & 0xFF] ^ (this->CRC >> 8)) & this->LimiterMask;
					else
						// (this->CRC >> 8) Becomes Zero When Width Is Lower Or Equal To 8 
						this->CRC = this->LookupTable[(this->CRC ^ Data) & 0xFF];
				}

			public:
				CRCEngine() {
					this->GenerateLookupTable();
					this->ResetCRC();
				}

				Type CRC = Initial;

				CONST_EXPRESSION FORCE_INLINE void ResetCRC() {
					this->CRC = Initial;

					if CONST_EXPRESSION(Width < 8 && !ReflectOut)
						this->CRC <<= (8 - Width);
				}

				CONST_EXPRESSION FORCE_INLINE Type GetCRC() {
					Type CRCOut = this->CRC;

					if CONST_EXPRESSION(Width < 8 && !ReflectIn)
						CRCOut >>= (8 - Width);
					
					CRCOut ^= XOROut;

					if CONST_EXPRESSION(ReflectOut && !ReflectIn)
						return Bitwise::BinaryReflect<Type, Width>(CRCOut) & this->LimiterMask;
					else
						return CRCOut & this->LimiterMask;
				}

				CONST_EXPRESSION FORCE_INLINE Type& UpdateCRC(Type Data) {
					// InversedUpdateCRC Needs To Be Used In Case The Table Is Reflected To Fully Emulate ReflectIn
					if CONST_EXPRESSION(ReflectIn)
						InversedUpdateCRC(Data); 
					else
						NormalUpdateCRC(Data);

					if CONST_EXPRESSION(Width >= 8)
						this->CRC &= this->LimiterMask;

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
			//				Type			Polynomial		RefIn	RefOut	Initial		XOROut	Width

			typedef CRCEngine<uint8_t,		0x3,				false,	false,	0x0,			0x7,		3> CRC_3_GSM;
			typedef CRCEngine<uint8_t,		0x3,				true,	true,	0x7,			0x0,		3> CRC_3_ROHC;

			typedef CRCEngine<uint8_t,		0x3,				true,	true,	0x0,			0x0,		4> CRC_4_G_704;
			typedef CRCEngine<uint8_t,		0x3,				false,	false,	0xF,			0xF,		4> CRC_4_INTERLAKEN;

			typedef CRCEngine<uint8_t,		0x09,			false,	false,	0x09,		0x00,	5> CRC_5_EPC_C1G2;
			typedef CRCEngine<uint8_t,		0x15,			true,	true,	0x00,		0x00,	5> CRC_5_G_704;
			typedef CRCEngine<uint8_t,		0x05,			true,	true,	0x1F,		0x1F,	5> CRC_5_USB;

			typedef CRCEngine<uint8_t,		0x27,			false,	false,	0x3F,		0x00,	6> CRC_6_CDMA2000_A;
			typedef CRCEngine<uint8_t,		0x07,			false,	false,	0x3F,		0x00,	6> CRC_6_CDMA2000_B;
			typedef CRCEngine<uint8_t,		0x19,			true,	true,	0x00,		0x00,	6> CRC_6_DARC;
			typedef CRCEngine<uint8_t,		0x03,			true,	true,	0x00,		0x00,	6> CRC_6_G_704;
			typedef CRCEngine<uint8_t,		0x2F,			false,	false,	0x00,		0x3F,	6> CRC_6_GSM;

			typedef CRCEngine<uint8_t,		0x09,			false,	false,	0x00,		0x00,	7> CRC_7_MMC;
			typedef CRCEngine<uint8_t,		0x4F,			true,	true,	0x7F,		0x00,	7> CRC_7_ROHC;
			typedef CRCEngine<uint8_t,		0x45,			false,	false,	0x00,		0x00,	7> CRC_7_UMTS;

			typedef CRCEngine<uint8_t,		0x9B,			false,	false,	0xFF,		0x00> CRC_8_CDMA2000;
			typedef CRCEngine<uint8_t,		0x9B,			true,	true,	0x00,		0x00> CRC_8_WCDMA;		// Mobile Networks
			typedef CRCEngine<uint8_t,		0xD5,			false,	false,	0x00,		0x00> CRC_8_DBV_S2;		// Digital Video Broadcasting - Satellite - Second Generation (DVB-S2)
			typedef CRCEngine<uint8_t,		0x31,			true,	true,	0x00,		0x00> CRC_8_MAXIM;		// 1-Wire Bus
			typedef CRCEngine<uint8_t,		0x39,			true,	true,	0x00,		0x00> CRC_8_DARC;		// Data Channel Radio
			typedef CRCEngine<uint8_t,		0x1D,			false,	false,	0xFD,		0x00> CRC_8_I_CODE;		// Philips Semiconductors(SL2 ICS11)
			typedef CRCEngine<uint8_t,		0x1D,			true,	true,	0xFF,		0x00> CRC_8_EBU;		// AKA CRC-8/AES Or CRC-8/TECH-3250
			typedef CRCEngine<uint8_t,		0x07,			true,	true,	0xFF,		0x00> CRC_8_ROHC;
			typedef CRCEngine<uint8_t,		0x07,			false,	false,	0x00,		0x55> CRC_8_ITU;		// AKA CRC-8/I-432-1, Used As The Asynchronous Transfer Mode Header Error Control Sequence
			typedef CRCEngine<uint8_t,		0x07,			false,	false,	0x00,		0x00> CRC_8_SMBUS;		
			typedef CRCEngine<uint8_t,		0xA7,			true,	true,	0x00,		0x00> CRC_8_BLUETOOTH;	// Bluetooth Header Error Correction
			typedef CRCEngine<uint8_t,		0x2F,			false,	false,	0xFF,		0xFF> CRC_8_AUTOSAR;
			typedef CRCEngine<uint8_t,		0x1D,			false,	false,	0x00,		0x00> CRC_8_GSM_A;
			typedef CRCEngine<uint8_t,		0x49,			false,	false,	0x00,		0xFF> CRC_8_GSM_B;
			typedef CRCEngine<uint8_t,		0x1D,			false,	false,	0xFF,		0x00> CRC_8_HITAG;		// Used In RFID Applications
			typedef CRCEngine<uint8_t,		0x1D,			false,	false,	0xC7,		0x00> CRC_8_MIFARE_MAD; // NPX Semi-Conductors
			typedef CRCEngine<uint8_t,		0x9B,			false,	false,	0x00,		0x00> CRC_8_LTE;
			typedef CRCEngine<uint8_t,		0x31,			false,	false,	0xFF,		0x00> CRC_8_NRSC_5;
			typedef CRCEngine<uint8_t,		0x2F,			false,	false,	0x00,		0x00> CRC_8_OPENSAFETY;
			typedef CRCEngine<uint8_t,		0x1D,			false,	false,	0xFF,		0xFF> CRC_8_SAE_J1850;

			typedef CRCEngine<uint16_t,	0x233,			false,	false,	0x000,		0x000,	10> CRC_10_ATM;
			typedef CRCEngine<uint16_t,	0x3D9,			false,	false,	0x3FF,		0x000,	10> CRC_10_CDMA2000;
			typedef CRCEngine<uint16_t,	0x175,			false,	false,	0x000,		0x3FF,	10> CRC_10_GSM;

			typedef CRCEngine<uint16_t,	0x385,			false,	false,	0x01A,		0x000,	11> CRC_11_FLEXRAY;
			typedef CRCEngine<uint16_t,	0x307,			false,	false,	0x000,		0x000,	11> CRC_11_UMTS;
			
			typedef CRCEngine<uint16_t,	0xF13,			false,	false,	0xFFF,		0x000,	12> CRC_12_CDMA2000;
			typedef CRCEngine<uint16_t,	0x80F,			false,	false,	0x000,		0x000,	12> CRC_12_DECT;
			typedef CRCEngine<uint16_t,	0xD31,			false,	false,	0x000,		0xFFF,	12> CRC_12_GSM;
			typedef CRCEngine<uint16_t,	0x80F,			false,	true,	0x000,		0x000,	12> CRC_12_UMTS;

			typedef CRCEngine<uint16_t,	0x1CF5,			false,	false,	0x0000,		0x0000,	13> CRC_13_BBC;
			
			typedef CRCEngine<uint16_t,	0x0805,			true,	true,	0x0000,		0x0000,	14> CRC_14_DARC;
			typedef CRCEngine<uint16_t,	0x202D,			false,	false,	0x0000,		0x3FFF,	14> CRC_14_GSM;
			
			typedef CRCEngine<uint16_t,	0x4599,			false,	false,	0x0000,		0x0000,	15> CRC_15_CAN;
			typedef CRCEngine<uint16_t,	0x6815,			false,	false,	0x0000,		0x0001,	15> CRC_15_MPT1327;

			typedef CRCEngine<uint16_t,	0x8005,			true,	true,	0x0000,		0x0000> CRC_16_ARC;
			typedef CRCEngine<uint16_t,	0xC867,			false,	false,	0xFFFF,		0x0000> CRC_16_CDMA2000;
			typedef CRCEngine<uint16_t,	0x8005,			false,	false,	0xFFFF,		0x0000> CRC_16_CMS;
			typedef CRCEngine<uint16_t,	0x8005,			false,	false,	0x800D,		0x0000> CRC_16_DDS_110;
			typedef CRCEngine<uint16_t,	0x0589,			false,	false,	0x0000,		0x0001> CRC_16_DECT_R;
			typedef CRCEngine<uint16_t,	0x0589,			false,	false,	0x0000,		0x0000> CRC_16_DECT_X;
			typedef CRCEngine<uint16_t,	0x3D65,			true,	true,	0x0000,		0xFFFF> CRC_16_DNP;
			typedef CRCEngine<uint16_t,	0x3D65,			false,	false,	0x0000,		0xFFFF> CRC_16_EN_13757;
			typedef CRCEngine<uint16_t,	0x1021,			false,	false,	0xFFFF,		0xFFFF> CRC_16_GENIBUS;
			typedef CRCEngine<uint16_t,	0x1021,			false,	false,	0x0000,		0xFFFF> CRC_16_GSM;
			typedef CRCEngine<uint16_t,	0x1021,			false,	false,	0xFFFF,		0x0000> CRC_16_IBM_3740;
			typedef CRCEngine<uint16_t,	0x1021,			true,	true,	0xFFFF,		0xFFFF> CRC_16_IBM_SDLC;
			typedef CRCEngine<uint16_t,	0x1021,			true,	true,	0x6363,		0x0000> CRC_16_ISO_IEC_14443_3_A;
			typedef CRCEngine<uint16_t,	0x1021,			true,	true,	0x0000,		0x0000> CRC_16_KERMIT;
			typedef CRCEngine<uint16_t,	0x6F63,			false,	false,	0x0000,		0x0000> CRC_16_LJ1200;
			typedef CRCEngine<uint16_t,	0x5935,			false,	false,	0xFFFF,		0x0000> CRC_16_M17;
			typedef CRCEngine<uint16_t,	0x8005,			true,	true,	0x0000,		0xFFFF> CRC_16_MAXIM_DOW;
			typedef CRCEngine<uint16_t,	0x1021,			true,	true,	0xFFFF,		0x0000> CRC_16_MCRF4XX;
			typedef CRCEngine<uint16_t,	0x8005,			true,	true,	0xFFFF,		0x0000> CRC_16_MODBUS;
			typedef CRCEngine<uint16_t,	0x080B,			true,	true,	0xFFFF,		0x0000> CRC_16_NRSC_5;
			typedef CRCEngine<uint16_t,	0x5935,			false,	false,	0x0000,		0x0000> CRC_16_OPENSAFETY_A;
			typedef CRCEngine<uint16_t,	0x755B,			false,	false,	0x0000,		0x0000> CRC_16_OPENSAFETY_B;
			typedef CRCEngine<uint16_t,	0x1DCF,			false,	false,	0xFFFF,		0xFFFF> CRC_16_PROFIBUS;
			typedef CRCEngine<uint16_t,	0x1021,			true,	true,	0x554D,		0x0000> CRC_16_RIELLO;
			typedef CRCEngine<uint16_t,	0x1021,			false,	false,	0x1D0F,		0x0000> CRC_16_SPI_FUJITSU;
			typedef CRCEngine<uint16_t,	0x8BB7,			false,	false,	0x0000,		0x0000> CRC_16_T10_DIF;
			typedef CRCEngine<uint16_t,	0xA097,			false,	false,	0x0000,		0x0000> CRC_16_TELEDISK;
			typedef CRCEngine<uint16_t,	0x1021,			true,	true,	0x3791,		0x0000> CRC_16_TMS37157;
			typedef CRCEngine<uint16_t,	0x8005,			false,	false,	0x0000,		0x0000> CRC_16_UMTS;
			typedef CRCEngine<uint16_t,	0x8005,			true,	true,	0xFFFF,		0xFFFF> CRC_16_USB;
			typedef CRCEngine<uint16_t,	0x1021,			false,	false,	0x0000,		0x0000> CRC_16_XMODEM;

			typedef CRCEngine<uint32_t,	0x1685B,			false,	false,	0x00000,		0x00000,		17> CRC_17_CAN_FD;

			typedef CRCEngine<uint32_t,	0x102899,		false,	false,	0x000000,	0x000000,	21> CRC_21_CAN_FD;
			
			typedef CRCEngine<uint32_t,	0x00065B,		true,	true,	0x555555,	0x000000,	24> CRC_24_BLE;
			typedef CRCEngine<uint32_t,	0x5D6DCB,		false,	false,	0xFEDCBA,	0x000000,	24> CRC_24_FLEXRAY_A;
			typedef CRCEngine<uint32_t,	0x5D6DCB,		false,	false,	0xABCDEF,	0x000000,	24> CRC_24_FLEXRAY_B;
			typedef CRCEngine<uint32_t,	0x328B63,		false,	false,	0xFFFFFF,	0xFFFFFF,	24> CRC_24_INTERLAKEN;
			typedef CRCEngine<uint32_t,	0x864CFB,		false,	false,	0x000000,	0x000000,	24> CRC_24_LTE_A;
			typedef CRCEngine<uint32_t,	0x800063,		false,	false,	0x000000,	0x000000,	24> CRC_24_LTE_B;
			typedef CRCEngine<uint32_t,	0x864CFB,		false,	false,	0xB704CE,	0x000000,	24> CRC_24_OPENPGP;
			typedef CRCEngine<uint32_t,	0x800063,		false,	false,	0xFFFFFF,	0xFFFFFF,	24> CRC_24_OS_9;

			typedef CRCEngine<uint32_t,	0x2030B9C7,		false,	false,	0x3FFFFFFF,	0x3FFFFFFF,	30> CRC_30_CDMA;
			
			typedef CRCEngine<uint32_t,	0x04C11DB7,		false,	false,	0x7FFFFFFF,	0x7FFFFFFF,	31> CRC_31_PHILIPS; 

			typedef CRCEngine<uint32_t,	0x814141AB,		false,	false,	0x00000000,	0x00000000> CRC_32_AIXM;
			typedef CRCEngine<uint32_t,	0xF4ACFB13,		true,	true,	0xFFFFFFFF,	0xFFFFFFFF> CRC_32_AUTOSAR;
			typedef CRCEngine<uint32_t,	0xA833982B,		true,	true,	0xFFFFFFFF,	0xFFFFFFFF> CRC_32_BASE91_D;
			typedef CRCEngine<uint32_t,	0x04C11DB7,		false,	false,	0xFFFFFFFF,	0xFFFFFFFF> CRC_32_BZIP2;
			typedef CRCEngine<uint32_t,	0x8001801B,		true,	true,	0x00000000,	0x00000000> CRC_32_CD_ROM_EDC;
			typedef CRCEngine<uint32_t,	0x04C11DB7,		false,	false,	0x00000000,	0xFFFFFFFF> CRC_32_CKSUM;
			typedef CRCEngine<uint32_t,	0x1EDC6F41,		true,	true,	0xFFFFFFFF,	0xFFFFFFFF> CRC_32_ISCSI;
			typedef CRCEngine<uint32_t,	0x04C11DB7,		true,	true,	0xFFFFFFFF,	0xFFFFFFFF> CRC_32_ISO_HDLC; 
			typedef CRCEngine<uint32_t,	0x04C11DB7,		true,	true,	0xFFFFFFFF,	0x00000000> CRC_32_JAMCRC;
			typedef CRCEngine<uint32_t,	0x741B8CD7,		true,	true,	0xFFFFFFFF,	0x00000000> CRC_32_MEF;
			typedef CRCEngine<uint32_t,	0x04C11DB7,		false,	false,	0xFFFFFFFF,	0x00000000> CRC_32_MPEG_2;
			typedef CRCEngine<uint32_t,	0x000000AF,		false,	false,	0x00000000,	0x00000000> CRC_32_XFER;
			
			typedef CRCEngine<uint64_t,	0x0004820009,		false,	false,	0x0000000000,	0xFFFFFFFFFF,	40> CRC_40_GSM;
		}
	}
}