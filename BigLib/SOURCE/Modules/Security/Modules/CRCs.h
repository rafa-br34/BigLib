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
			
				const size_t TableLen=256,
				typename TableType=uint16_t
			>
			class CRC_Base {
			private:
				Type LookupTable[TableLen];
				const Type LimiterMask = Bitwise::MakeBinaryMask<Type>(Width);

				CONST_EXPRESSION void GenerateLookupTable() {
					// 0x80 Would Be (1 << (Width - 1)) << (8 - Width)
					const Type MSBMask	= (Width < 8 ? 0x80 : 1 << (Width - 1));
					Type Poly;
					if CONST_EXPRESSION(Width < 8) Poly = (Polynomial << (8 - Width)); else Poly = Polynomial;


					for (TableType B = 0; B < TableLen; B++) {
						// Only Mirror The First 8 Bits Here, Otherwise The Left Shift Will Remove Our Bits.
						Type Remainder = (ReflectIn ? Bitwise::BinaryReflect<Type, 8>((Type)B) : (Type)B);
						
						if CONST_EXPRESSION(Width > 8) Remainder <<= (Width - 8);

						for (size_t i = 0; i < 8; i++)
							Remainder = Remainder & MSBMask ? ((Remainder << 1) ^ Poly) : (Remainder << 1);
							
						// Reflecting The Table Item Will Have The Same Effect Of ReflectIn.
						this->LookupTable[B] = (ReflectIn ? Bitwise::BinaryReflect<Type, Width < 8 ? 8 : Width>(Remainder) : Remainder);
					}
				}

				CONST_EXPRESSION FORCE_INLINE void NormalUpdateCRC(Type Data) {
					if CONST_EXPRESSION(Width > 8)
						this->CRC = this->LookupTable[(Data ^ (this->CRC >> (Width - 8)) & 0xFF)] ^ (this->CRC << 8);
					else
						// (this->CRC << 8) Becomes Zero If Width <= 8 
						// If Width Is Less Than Or Equal To 8 It Should Be Removed.
						this->CRC = this->LookupTable[(Data ^ this->CRC) & 0xFF];
						
					if CONST_EXPRESSION(Width >= 8)
						this->CRC &= this->LimiterMask;
				}

				CONST_EXPRESSION FORCE_INLINE void InversedUpdateCRC(Type Data) {
					if CONST_EXPRESSION(Width > 8)
						this->CRC = this->LookupTable[((this->CRC ^ Data) & 0xFF)] ^ (this->CRC >> 8);
					else
						// (this->CRC >> 8) Becomes Zero When Width Is Lower Or Equal To 8 
						this->CRC = this->LookupTable[(this->CRC ^ Data)];
				}

			public:
				CRC_Base() {
					GenerateLookupTable();
				}

				Type CRC = Initial;

				CONST_EXPRESSION FORCE_INLINE void ResetCRC() {
					if CONST_EXPRESSION(Width < 8)
						this->CRC = (Initial << (8 - Width));
					else
						this->CRC = Initial;
				}

				CONST_EXPRESSION FORCE_INLINE Type GetCRC() {
					Type CRCOut;
					if CONST_EXPRESSION(Width < 8 && !ReflectIn)
						CRCOut = (this->CRC >> (8 - Width));
					else
						CRCOut = this->CRC;
					
					CRCOut ^= XOROut;

					if CONST_EXPRESSION(ReflectOut && !ReflectIn)
						return Bitwise::BinaryReflect<Type, Width>(CRCOut);
					else
						return CRCOut;
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
			//				 Type		Polynomial	RefIn	RefOut	Initial		XOROut	Width TableLen

			typedef CRC_Base<uint8_t,	0x3,		false,	false,	0x0,		0x7,	3> CRC_3_GSM;
			typedef CRC_Base<uint8_t,	0x3,		true,	true,	0x7,		0x0,	3> CRC_3_ROHC;

			typedef CRC_Base<uint8_t,	0x3,		true,	true,	0x0,		0x0,	4> CRC_4_G_704;
			typedef CRC_Base<uint8_t,	0x3,		false,	false,	0xF,		0xF,	4> CRC_4_INTERLAKEN;

			typedef CRC_Base<uint8_t,	0x09,		false,	false,	0x09,		0x00,	5> CRC_5_EPC_C1G2;
			typedef CRC_Base<uint8_t,	0x15,		true,	true,	0x00,		0x00,	5> CRC_5_G_704;
			typedef CRC_Base<uint8_t,	0x05,		true,	true,	0x1F,		0x1F,	5> CRC_5_USB;

			typedef CRC_Base<uint8_t,	0x27,		false,	false,	0x3F,		0x00,	6> CRC_6_CDMA2000_A;
			typedef CRC_Base<uint8_t,	0x07,		false,	false,	0x3F,		0x00,	6> CRC_6_CDMA2000_B;
			typedef CRC_Base<uint8_t,	0x19,		true,	true,	0x00,		0x00,	6> CRC_6_DARC;
			typedef CRC_Base<uint8_t,	0x03,		true,	true,	0x00,		0x00,	6> CRC_6_G_704;
			typedef CRC_Base<uint8_t,	0x2F,		false,	false,	0x00,		0x3F,	6> CRC_6_GSM;

			typedef CRC_Base<uint8_t,	0x09,		false,	false,	0x00,		0x00,	7> CRC_7_MMC;
			typedef CRC_Base<uint8_t,	0x4F,		true,	true,	0x7F,		0x00,	7> CRC_7_ROHC;
			typedef CRC_Base<uint8_t,	0x45,		false,	false,	0x00,		0x00,	7> CRC_7_UMTS;

			typedef CRC_Base<uint8_t,	0x9B,		false,	false,	0xFF,		0x00> CRC_8_CDMA2000;
			typedef CRC_Base<uint8_t,	0x9B,		true,	true,	0x00,		0x00> CRC_8_WCDMA;		// Mobile Networks
			typedef CRC_Base<uint8_t,	0xD5,		false,	false,	0x00,		0x00> CRC_8_DBV_S2;		// Digital Video Broadcasting - Satellite - Second Generation (DVB-S2)
			typedef CRC_Base<uint8_t,	0x31,		true,	true,	0x00,		0x00> CRC_8_MAXIM;		// 1-Wire Bus
			typedef CRC_Base<uint8_t,	0x39,		true,	true,	0x00,		0x00> CRC_8_DARC;		// Data Channel Radio
			typedef CRC_Base<uint8_t,	0x1D,		false,	false,	0xFD,		0x00> CRC_8_I_CODE;		// Philips Semiconductors(SL2 ICS11)
			typedef CRC_Base<uint8_t,	0x1D,		true,	true,	0xFF,		0x00> CRC_8_EBU;		// AKA CRC-8/AES Or CRC-8/TECH-3250
			typedef CRC_Base<uint8_t,	0x07,		true,	true,	0xFF,		0x00> CRC_8_ROHC;
			typedef CRC_Base<uint8_t,	0x07,		false,	false,	0x00,		0x55> CRC_8_ITU;		// AKA CRC-8/I-432-1, Used As The Asynchronous Transfer Mode Header Error Control Sequence
			typedef CRC_Base<uint8_t,	0x07,		false,	false,	0x00,		0x00> CRC_8_SMBUS;		
			typedef CRC_Base<uint8_t,	0xA7,		true,	true,	0x00,		0x00> CRC_8_BLUETOOTH;	// Bluetooth Header Error Correction
			typedef CRC_Base<uint8_t,	0x2F,		false,	false,	0xFF,		0xFF> CRC_8_AUTOSAR;
			typedef CRC_Base<uint8_t,	0x1D,		false,	false,	0x00,		0x00> CRC_8_GSM_A;
			typedef CRC_Base<uint8_t,	0x49,		false,	false,	0x00,		0xFF> CRC_8_GSM_B;
			typedef CRC_Base<uint8_t,	0x1D,		false,	false,	0xFF,		0x00> CRC_8_HITAG;		// Used In RFID Applications
			typedef CRC_Base<uint8_t,	0x1D,		false,	false,	0xC7,		0x00> CRC_8_MIFARE_MAD; // NPX Semi-Conductors
			typedef CRC_Base<uint8_t,	0x9B,		false,	false,	0x00,		0x00> CRC_8_LTE;
			typedef CRC_Base<uint8_t,	0x31,		false,	false,	0xFF,		0x00> CRC_8_NRSC_5;
			typedef CRC_Base<uint8_t,	0x2F,		false,	false,	0x00,		0x00> CRC_8_OPENSAFETY;
			typedef CRC_Base<uint8_t,	0x1D,		false,	false,	0xFF,		0xFF> CRC_8_SAE_J1850;

			typedef CRC_Base<uint16_t,	0x233,		false,	false,	0x000,		0x000,	10> CRC_10_ATM;
			typedef CRC_Base<uint16_t,	0x3D9,		false,	false,	0x3FF,		0x000,	10> CRC_10_CDMA2000;
			typedef CRC_Base<uint16_t,	0x175,		false,	false,	0x000,		0x3FF,	10> CRC_10_GSM;

			typedef CRC_Base<uint16_t,	0x385,		false,	false,	0x01A,		0x000,	11> CRC_11_FLEXRAY;
			typedef CRC_Base<uint16_t,	0x307,		false,	false,	0x000,		0x000,	11> CRC_11_UMTS;
			
			typedef CRC_Base<uint16_t,	0xF13,		false,	false,	0xFFF,		0x000,	12> CRC_12_CDMA2000;
			typedef CRC_Base<uint16_t,	0x80F,		false,	false,	0x000,		0x000,	12> CRC_12_DECT;
			typedef CRC_Base<uint16_t,	0xD31,		false,	false,	0x000,		0xFFF,	12> CRC_12_GSM;
			typedef CRC_Base<uint16_t,	0x80F,		false,	true,	0x000,		0x000,	12> CRC_12_UMTS;

			typedef CRC_Base<uint16_t,	0x1CF5,		false,	false,	0x0000,		0x0000,	13> CRC_13_BBC;
			
			typedef CRC_Base<uint16_t,	0x0805,		true,	true,	0x0000,		0x0000,	14> CRC_14_DARC;
			typedef CRC_Base<uint16_t,	0x202D,		false,	false,	0x0000,		0x3FFF,	14> CRC_14_GSM;

			typedef CRC_Base<uint16_t,	0x4599,		false,	false,	0x0000,		0x0000,	15> CRC_15_CAN;
			typedef CRC_Base<uint16_t,	0x6815,		false,	false,	0x0000,		0x0001,	15> CRC_15_MPT1327;

			typedef CRC_Base<uint16_t,	0x8005,		true,	true,	0x0000,		0x0000> CRC_16_ARC;
			typedef CRC_Base<uint16_t,	0xC867,		false,	false,	0xFFFF,		0x0000> CRC_16_CDMA2000;
			typedef CRC_Base<uint16_t,	0x8005,		false,	false,	0xFFFF,		0x0000> CRC_16_CMS;
			typedef CRC_Base<uint16_t,	0x8005,		false,	false,	0x800D,		0x0000> CRC_16_DDS_110;
			typedef CRC_Base<uint16_t,	0x0589,		false,	false,	0x0000,		0x0001> CRC_16_DECT_R;
			typedef CRC_Base<uint16_t,	0x0589,		false,	false,	0x0000,		0x0000> CRC_16_DECT_X;
			typedef CRC_Base<uint16_t,	0x3D65,		true,	true,	0x0000,		0xFFFF> CRC_16_DNP;
			typedef CRC_Base<uint16_t,	0x3D65,		false,	false,	0x0000,		0xFFFF> CRC_16_EN_13757;
			typedef CRC_Base<uint16_t,	0x1021,		false,	false,	0xFFFF,		0xFFFF> CRC_16_GENIBUS;
			typedef CRC_Base<uint16_t,	0x1021,		false,	false,	0x0000,		0xFFFF> CRC_16_GSM;
			typedef CRC_Base<uint16_t,	0x1021,		false,	false,	0xFFFF,		0x0000> CRC_16_IBM_3740;
			typedef CRC_Base<uint16_t,	0x1021,		true,	true,	0xFFFF,		0xFFFF> CRC_16_IBM_SDLC;
			typedef CRC_Base<uint16_t,	0x1021,		true,	true,	0x6363,		0x0000> CRC_16_ISO_IEC_14443_3_A;
			typedef CRC_Base<uint16_t,	0x1021,		true,	true,	0x0000,		0x0000> CRC_16_KERMIT;
			typedef CRC_Base<uint16_t,	0x6F63,		false,	false,	0x0000,		0x0000> CRC_16_LJ1200;
			typedef CRC_Base<uint16_t,	0x5935,		false,	false,	0xFFFF,		0x0000> CRC_16_M17;
			typedef CRC_Base<uint16_t,	0x8005,		true,	true,	0x0000,		0xFFFF> CRC_16_MAXIM_DOW;
			typedef CRC_Base<uint16_t,	0x1021,		true,	true,	0xFFFF,		0x0000> CRC_16_MCRF4XX;
			typedef CRC_Base<uint16_t,	0x8005,		true,	true,	0xFFFF,		0x0000> CRC_16_MODBUS;
			typedef CRC_Base<uint16_t,	0x080B,		true,	true,	0xFFFF,		0x0000> CRC_16_NRSC_5;
			typedef CRC_Base<uint16_t,	0x5935,		false,	false,	0x0000,		0x0000> CRC_16_OPENSAFETY_A;
			typedef CRC_Base<uint16_t,	0x755B,		false,	false,	0x0000,		0x0000> CRC_16_OPENSAFETY_B;
			typedef CRC_Base<uint16_t,	0x1DCF,		false,	false,	0xFFFF,		0xFFFF> CRC_16_PROFIBUS;
			typedef CRC_Base<uint16_t,	0x1021,		true,	true,	0x554D,		0x0000> CRC_16_RIELLO;
			typedef CRC_Base<uint16_t,	0x1021,		false,	false,	0x1D0F,		0x0000> CRC_16_SPI_FUJITSU;
			typedef CRC_Base<uint16_t,	0x8BB7,		false,	false,	0x0000,		0x0000> CRC_16_T10_DIF;
			typedef CRC_Base<uint16_t,	0xA097,		false,	false,	0x0000,		0x0000> CRC_16_TELEDISK;
			typedef CRC_Base<uint16_t,	0x1021,		true,	true,	0x3791,		0x0000> CRC_16_TMS37157;
			typedef CRC_Base<uint16_t,	0x8005,		false,	false,	0x0000,		0x0000> CRC_16_UMTS;
			typedef CRC_Base<uint16_t,	0x8005,		true,	true,	0xFFFF,		0xFFFF> CRC_16_USB;
			typedef CRC_Base<uint16_t,	0x1021,		false,	false,	0x0000,		0x0000> CRC_16_XMODEM;

			typedef CRC_Base<uint32_t,	0x1685B,	false,	false,	0x00000,	0x00000,	17> CRC_17_CAN_FD;

			typedef CRC_Base<uint32_t,	0x102899,	false,	false,	0x000000,	0x000000,	21> CRC_21_CAN_FD;

			typedef CRC_Base<uint32_t,	0x00065B,	true,	true,	0x555555,	0x000000,	24> CRC_24_BLE;
			typedef CRC_Base<uint32_t,	0x5D6DCB,	false,	false,	0xFEDCBA,	0x000000,	24> CRC_24_FLEXRAY_A;
			typedef CRC_Base<uint32_t,	0x5D6DCB,	false,	false,	0xABCDEF,	0x000000,	24> CRC_24_FLEXRAY_B;
			typedef CRC_Base<uint32_t,	0x328B63,	false,	false,	0xFFFFFF,	0xFFFFFF,	24> CRC_24_INTERLAKEN;
			typedef CRC_Base<uint32_t,	0x864CFB,	false,	false,	0x000000,	0x000000,	24> CRC_24_LTE_A;
			typedef CRC_Base<uint32_t,	0x800063,	false,	false,	0x000000,	0x000000,	24> CRC_24_LTE_B;
			typedef CRC_Base<uint32_t,	0x864CFB,	false,	false,	0xB704CE,	0x000000,	24> CRC_24_OPENPGP;
			typedef CRC_Base<uint32_t,	0x800063,	false,	false,	0xFFFFFF,	0xFFFFFF,	24> CRC_24_OS_9;

			typedef CRC_Base<uint32_t,	0x04C11DB7, true,	true,	0xFFFFFFFF, 0xFFFFFFFF> CRC_32;
		}
	}
}