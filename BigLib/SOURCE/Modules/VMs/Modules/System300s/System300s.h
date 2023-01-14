#pragma once
#include "../../../../Includes.h"
#include "../../../Data/Bitwise.h"

namespace BigLib {
	namespace VMs {
		namespace System300s {

			enum OpcodeFormats {
				_AVAILABILITY_SYSTEM360,
				OPF_I,		// OP<8>	I<8>														Size: 2
				OPF_RR,		// OP<8>	(R1<4> R2<4>)<8>											Size: 2
				OPF_RS1,	// OP<8>	(R1<4> R3<4>)<8>	(B2<4> D2<12>)<16>						Size: 4
				OPF_RS2,	// OP<8>	(R1<4> M3<4>)<8>	(B2<4> D2<12>)<16>						Size: 4
				OPF_RX,		// OP<8>	(R1<4> X2<4>)<8>	(B2<4> D2<12>)<16>						Size: 4
				OPF_SI,		// OP<8>	I2<8>				(B1<4> D1<12>)<16>						Size: 4
				OPF_SS_A,	// OP<8>	L<8>				(B1<4> D1<12>)<16> (B2<4> D2<12>)<16>	Size: 6
				OPF_SS_B,	// OP<8>	(L1<4> L2<4>)		(B1<4> D1<12>)<16> (B2<4> D2<12>)<16>	Size: 6
				OPF_SS_C,	// OP<8>	(R1<4> R3<4>)		(B1<4> D1<12>)<16> (B2<4> D2<12>)<16>	Size: 6
				_AVAILABILITY_SYSTEM370,
				OPF_RRE,	// OP<16>	(?<8> R1<4> R2<4>)<16>										Size: 4
				OPF_S,		// OP<16>	(B2<4> D2<12>)<16>											Size: 4
				OPF_SSE,	// OP<16>	(B1<4> D1<12>)<16>	(B2<4> D2<12>)<16>						Size: 6
				
				// TODO: Maybe Add System/390 And System/Z?
			};


			enum Opcodes {
				SPM = 0x04,		// <RR>		Set Program Mask
				BALR,			// <RR>		Branch And Link Register
				BCTR,			// <RR>		Branch On Count Register
				BCR,			// <RR>		Branch On Condition Register, NOTE: BCR Has 2 Sub-Opcodes 
				/*
				NOPR = 0x070,	// <RR>		No Operation Register
				BR = 0x07F,		// <RR>		Branch Register (Unconditional)
				*/
				SSK = 0x08,		// <RR>		Set Storage Key
				ISK,			// <RR>		Set Storage Key
				SVC,			// <I>		Supervisor Call
				BASR,			// <RR>		Branch And Save Register
				
				LPR = 0x10,		// <RR>		Load Positive Register
				LNR,			// <RR>		Load Negative Register
				LTR,			// <RR>		Load And Test Register
				LCR,			// <RR>		Load and Complement Register
				NR,				// <RR>		And Register
				CLR,			// <RR>		Compare Logical Register
				OR,				// <RR>		Or Register
				XR,				// <RR>		Exclusive-Or Register
				LR,				// <RR>		Load Register
				CR,				// <RR>		Compare Register
				AR,				// <RR>		Add Register
				SB,				// <RR>		Subtract Register
				MR,				// <RR>		Multiply Register
				DR,				// <RR>		Divide Register
				ALR,			// <RR>		Add Logical Register
				SLR,			// <RR>		Subtract Logical Register
				LPDR,			// <RR>		Load Positive(Long)				
				LNDR,			// <RR>		Load Negative
				LTDR,			// <RR>		Load And Test
				LCDR,			// <RR>		Load Complement(Long)
				HDR,			// <RR>		Halve
				LRDR,			// <RR>		Load Rounded
				MXR,			// <RR>		Multiply
				MXDR,			// <RR>		Multiply
				LDR,			// <RR>		Load
				CDR,			// <RR>		Compare Double Register
				ADR,			// <RR>		Add Double Register
				SDR,			// <RR>		Subtract Double Register
				MDR,			// <RR>		Multiply Double Register
				DDR,			// <RR>		Divide Double Register
				AWR,			// <RR>		Add Unnormalized
				SWR,			// <RR>		Subtract Unnormalized
				LPER,			// <RR>		Load Positive(Short)
				LNER,			// <RR>		Load Negative
				LTER,			// <RR>		Load And Test
				LCER,			// <RR>		Load Complement(Short)
				HER,			// <RR>		Halve
				LRER,			// <RR>		Load Rounded 
				AXR,			// <RR>		Add Normalized
				SXR,			// <RR>		Subtract Normalized
				LER,			// <RR>		Load
				CER,			// <RR>		Compare
				AER,			// <RR>		Add Normalized
				SER,			// <RR>		Subtract Normalized
				MER,			// <RR>		Multiply Normalized
				DER,			// <RR>		Divide Normalized
				AUR,			// <RR>		Add Unnormalized
				SUR,			// <RR>		Subtract Unnormalized
				STH,			// <RX>		Store Halfword
				LA,				// <RX>		Load Address
				STC,			// <RX>		Store Character
				IC,				// <RX>		Insert Character
				EX,				// <RX>		Execute
				BAL,			// <RX>		Branch And Link
				BCT,			// <RX>		Branch On Count
				
				BC,				// <RX>		Branch On Condition, NOTE: BC Has 16 Sub-Opcodes
				/*
				NOP = 0x470,	// <RX>		No Operation
				BO,				// <RX>		Branch On Overflow/Ones
				BH,				// <RX>		Branch (A High)
				BP,				// <RX>		Branch On Plus
				BL,				// <RX>		Branch (A Low)
				BM,				// <RX>		Branch On Minus/Mixed
				BNE,			// <RX>		Branch Not Equal
				BNZ,			// <RX>		Branch Not Zero
				BE,				// <RX>		Branch (A Equal B)
				BZ,				// <RX>		Branch On Zero
				BNL,			// <RX>		Branch (A Not Low)
				BNM,			// <RX>		Branch Not Minus
				BNH,			// <RX>		Branch Not High
				BNP,			// <RX>		Branch Not Plus
				BNO,			// <RX>		Branch Not Ones
				B,				// <RX>		Branch (Unconditional)
				*/
				LH = 0x48,		// <RX>		Load Halfword
				CH,				// <RX>		Compare Halfword
				AH,				// <RX>		Add Halfword
				SH,				// <RX>		Subtract Halfword
				MH,				// <RX>		Multiply Halfword
				BAS,			// <RX>		Branch And Save
				CVD,			// <RX>		Convert to Decimal
				CVB,			// <RX>		Convert to Binary
				ST,				// <RX>		Store
				N,				// <RX>		And
				CL,				// <RX>		Compare Logical
				O,				// <RX>		Or
				X,				// <RX>		Exclusive-Or
				L,				// <RX>		Load
				C,				// <RX>		Compare
				A,				// <RX>		Add
				S,				// <RX>		Subtract
				M,				// <RX>		Multiply
				D,				// <RX>		Divide
				AL,				// <RX>		Add Logical
				SL,				// <RX>		Subtract Logical
				STD,			// <RX>		Store Double
				MXD,			// <RX>		Multiply Double
				LD,				// <RX>		Load Double
				CD,				// <RX>		Compare Double
				AD,				// <RX>		Add Double Normalized
				SD,				// <RX>		Subtract Double Normalized
				MD,				// <RX>		Multiply Double
				DD,				// <RX>		Divide Double
				AW,				// <RX>		Add Unnormalized Word
				SW,				// <RX>		Subtract Unnormalized Word
				STE,			// <RX>		Store Short
				LE,				// <RX>		Load Short
				CE,				// <RX>		Compare Short
				AE,				// <RX>		Add Normalized Short
				SE,				// <RX>		Subtract Normalized Short
				ME,				// <RX>		Multiply Short
				DE,				// <RX>		Divide Short
				AU,				// <RX>		Add Unnormalized Short
				SU,				// <RX>		Subtract Unnormalized Short
				SSE,			// <SI>		Set System Mask
				LPSW,			// <S>		Load Program Status Word 
				DIAGNOSE,		// <RX>		Diagnose
				WRD,			// <RX>		Write Direct
				RDD,			// <RX>		Read Direct
				BXH,			// <RS>		Branch On Index High
				BXLE,			// <RS>		Branch On Index Low Or Equal 
				SRL,			// <RS>		Shift Right Single Logical 
				SLL,			// <RS>		Shift Left Logical
				SRA,			// <RS>		Shift Right Arithmetic
				SLA,			// <RS>		Shift Left Arithmetic
				SRDL,			// <RS>		Shift Right Double Logical
				SLDL,			// <RS>		Shift Left Double Logical
				SRDA,			// <RS>		Shift Right Double Arithmetic
				SLDA,			// <RS>		Shift Left Double Arithmetic
				STM,			// <RS>		Store Multiple
				TM,				// <SI>		Test Under Mask
				MVI,			// <SI>		Move Immediate
				TS,				// <SI>		Test And Set
				NI,				// <SI>		And Immediate 
				CLI,			// <SI>		Compare Logical Immediate
				OI,				// <SI>		Or Immediate
				XI,				// <SI>		Exclusive-Or Immediate 
				LM,				// <RS>		Load Multiple
				SIO,			// <SI>		Start I/O
				TIO,			// <SI>		Test I/O
				HIO,			// <SI>		Halt I/O 
				TCH,			// <SI>		Test Channel
				MVN,			// <SS-A>	Move Numeric
				MVC,			// <SS-A>	Move Character 
				NC,				// <SS-A>	And Character
				CLC,			// <SS-A>	Compare Logical Character
				OC,				// <SS-A>	Or Character
				XC,				// <SS-A>	Exclusive-Or Character 
				TR,				// <SS-A>	Translate
				TRT,			// <SS-A>	Translate And Test
				ED,				// <SS-A>	Edit
				EDMK,			// <SS-A>	Edit And Mark
				SRP,			// <SS-C>	Shift And Round Packed
				MVO,			// <SS-B>	Move With Offset 
				PACK,			// <SS-B>	Pack
				UNPK,			// <SS-B>	Unpack
				ZAP,			// <SS-B>	Zero And Add Packed
				CP,				// <SS-B>	Compare Packed
				AP,				// <SS-B>	Add Packed
				SP,				// <SS-B>	Subtract Packed
				MP,				// <SS-B>	Multiply Packed
				DP,				// <SS-B>	Divide Packed
				// TODO: Add Others
			};

			enum Architectures {
				SYSTEM_360,
				SYSTEM_370,
				SYSTEM_390,
				SYSTEM_Z,
			};

			struct System300s {
				Architectures Architecture;

				// 128-Bits On The Z/System 
				uint64_t	PSW[2];
				// 4 Double Registers On The 360 Enumerated: 0, 2, 4, 6. 370/S 390/S Z/System All Have 16 Registers
				double		DREGISTERS[16];
				// 16 Privileged 32-Bit Control Registers, Used To Control Various Operations Of The Computer (Not Available In The 360)
				uint32_t	CR32[16];

				// 16 32-Bit Registers On The 360, Z/System Has 16 64-Bit Registers
				uint64_t	R64[16];

				
				void* Memory = nullptr;


				uint8_t RunInstruction(uint32_t MemoryOffset) {
					#define INSTRUCTION_RR(Byte) uint8_t R0 = Byte >> 4, R1 = Byte & 0x0F;


					uint8_t* Instruction = (uint8_t*)Memory + MemoryOffset;
					
					#define VMCASE(Opcode) case Opcode:
					switch (*Instruction++) {
						VMCASE(Opcodes::SPM) {
							INSTRUCTION_RR(*Instruction++);
							this->PS_SET_PM((uint8_t)R64[R0] & 0x0F);
							return 2;
						}
						
						VMCASE(Opcodes::BALR) {
							INSTRUCTION_RR(*Instruction++);
							auto AM = this->GetCurrentAddressingMode();


							return 2;
						}
						VMCASE(Opcodes::BCTR)
							break;
						VMCASE(Opcodes::BCR)
							break; // Note
					
							break;
						VMCASE(Opcodes::SSK)
							break;
						VMCASE(Opcodes::ISK)
							break;
						VMCASE(Opcodes::SVC)
							break;
						VMCASE(Opcodes::BASR)
							break;
						VMCASE(Opcodes::LPR)
							break;
						VMCASE(Opcodes::LNR)
							break;
						VMCASE(Opcodes::LTR)
							break;
						VMCASE(Opcodes::LCR)
							break;
						VMCASE(Opcodes::NR)
							break;
						VMCASE(Opcodes::CLR)
							break;
						VMCASE(Opcodes::OR)
							break;
						VMCASE(Opcodes::XR)
							break;
						VMCASE(Opcodes::LR)
							break;
						VMCASE(Opcodes::CR)
							break;
						VMCASE(Opcodes::AR)
							break;
						VMCASE(Opcodes::SB)
							break;
						VMCASE(Opcodes::MR)
							break;
						VMCASE(Opcodes::DR)
							break;
						VMCASE(Opcodes::ALR)
							break;
						VMCASE(Opcodes::SLR)
							break;
						VMCASE(Opcodes::LPDR)
							break;
						VMCASE(Opcodes::LNDR)
							break;
						VMCASE(Opcodes::LTDR)
							break;
						VMCASE(Opcodes::LCDR)
							break;
						VMCASE(Opcodes::HDR)
							break;
						VMCASE(Opcodes::LRDR)
							break;
						VMCASE(Opcodes::MXR)
							break;
						VMCASE(Opcodes::MXDR)
							break;
						VMCASE(Opcodes::LDR)
							break;
						VMCASE(Opcodes::CDR)
							break;
						VMCASE(Opcodes::ADR)
							break;
						VMCASE(Opcodes::SDR)
							break;
						VMCASE(Opcodes::MDR)
							break;
						VMCASE(Opcodes::DDR)
							break;
						VMCASE(Opcodes::AWR)
							break;
						VMCASE(Opcodes::SWR)
							break;
						VMCASE(Opcodes::LPER)
							break;
						VMCASE(Opcodes::LNER)
							break;
						VMCASE(Opcodes::LTER)
							break;
						VMCASE(Opcodes::LCER)
							break;
						VMCASE(Opcodes::HER)
							break;
						VMCASE(Opcodes::LRER)
							break;
						VMCASE(Opcodes::AXR)
							break;
						VMCASE(Opcodes::SXR)
							break;
						VMCASE(Opcodes::LER)
							break;
						VMCASE(Opcodes::CER)
							break;
						VMCASE(Opcodes::AER)
							break;
						VMCASE(Opcodes::SER)
							break;
						VMCASE(Opcodes::MER)
							break;
						VMCASE(Opcodes::DER)
							break;
						VMCASE(Opcodes::AUR)
							break;
						VMCASE(Opcodes::SUR)
							break;
						VMCASE(Opcodes::STH)
							break;
						VMCASE(Opcodes::LA)
							break;
						VMCASE(Opcodes::STC)
							break;
						VMCASE(Opcodes::IC)
							break;
						VMCASE(Opcodes::EX)
							break;
						VMCASE(Opcodes::BAL)
							break;
						VMCASE(Opcodes::BCT)
							break;
						VMCASE(Opcodes::BC)
							break; // Note
						
						
						VMCASE(Opcodes::LH)
							break;
						VMCASE(Opcodes::CH)
							break;
						VMCASE(Opcodes::AH)
							break;
						VMCASE(Opcodes::SH)
							break;
						VMCASE(Opcodes::MH)
							break;
						VMCASE(Opcodes::BAS)
							break;
						VMCASE(Opcodes::CVD)
							break;
						VMCASE(Opcodes::CVB)
							break;
						VMCASE(Opcodes::ST)
							break;
						VMCASE(Opcodes::N)
							break;
						VMCASE(Opcodes::CL)
							break;
						VMCASE(Opcodes::O)
							break;
						VMCASE(Opcodes::X)
							break;
						VMCASE(Opcodes::L)
							break;
						VMCASE(Opcodes::C)
							break;
						VMCASE(Opcodes::A)
							break;
						VMCASE(Opcodes::S)
							break;
						VMCASE(Opcodes::M)
							break;
						VMCASE(Opcodes::D)
							break;
						VMCASE(Opcodes::AL)
							break;
						VMCASE(Opcodes::SL)
							break;
						VMCASE(Opcodes::STD)
							break;
						VMCASE(Opcodes::MXD)
							break;
						VMCASE(Opcodes::LD)
							break;
						VMCASE(Opcodes::CD)
							break;
						VMCASE(Opcodes::AD)
							break;
						VMCASE(Opcodes::SD)
							break;
						VMCASE(Opcodes::MD)
							break;
						VMCASE(Opcodes::DD)
							break;
						VMCASE(Opcodes::AW)
							break;
						VMCASE(Opcodes::SW)
							break;
						VMCASE(Opcodes::STE)
							break;
						VMCASE(Opcodes::LE)
							break;
						VMCASE(Opcodes::CE)
							break;
						VMCASE(Opcodes::AE)
							break;
						VMCASE(Opcodes::SE)
							break;
						VMCASE(Opcodes::ME)
							break;
						VMCASE(Opcodes::DE)
							break;
						VMCASE(Opcodes::AU)
							break;
						VMCASE(Opcodes::SU)
							break;
						VMCASE(Opcodes::SSE)
							break;
						VMCASE(Opcodes::LPSW)
							break;
						VMCASE(Opcodes::DIAGNOSE)
							break;
						VMCASE(Opcodes::WRD)
							break;
						VMCASE(Opcodes::RDD)
							break;
						VMCASE(Opcodes::BXH)
							break;
						VMCASE(Opcodes::BXLE)
							break;
						VMCASE(Opcodes::SRL)
							break;
						VMCASE(Opcodes::SLL)
							break;
						VMCASE(Opcodes::SRA)
							break;
						VMCASE(Opcodes::SLA)
							break;
						VMCASE(Opcodes::SRDL)
							break;
						VMCASE(Opcodes::SLDL)
							break;
						VMCASE(Opcodes::SRDA)
							break;
						VMCASE(Opcodes::SLDA)
							break;
						VMCASE(Opcodes::STM)
							break;
						VMCASE(Opcodes::TM)
							break;
						VMCASE(Opcodes::MVI)
							break;
						VMCASE(Opcodes::TS)
							break;
						VMCASE(Opcodes::NI)
							break;
						VMCASE(Opcodes::CLI)
							break;
						VMCASE(Opcodes::OI)
							break;
						VMCASE(Opcodes::XI)
							break;
						VMCASE(Opcodes::LM)
							break;
						VMCASE(Opcodes::SIO)
							break;
						VMCASE(Opcodes::TIO)
							break;
						VMCASE(Opcodes::HIO)
							break;
						VMCASE(Opcodes::TCH)
							break;
						VMCASE(Opcodes::MVN)
							break;
						VMCASE(Opcodes::MVC)
							break;
						VMCASE(Opcodes::NC)
							break;
						VMCASE(Opcodes::CLC)
							break;
						VMCASE(Opcodes::OC)
							break;
						VMCASE(Opcodes::XC)
							break;
						VMCASE(Opcodes::TR)
							break;
						VMCASE(Opcodes::TRT)
							break;
						VMCASE(Opcodes::ED)
							break;
						VMCASE(Opcodes::EDMK)
							break;
						VMCASE(Opcodes::SRP)
							break;
						VMCASE(Opcodes::MVO)
							break;
						VMCASE(Opcodes::PACK)
							break;
						VMCASE(Opcodes::UNPK)
							break;
						VMCASE(Opcodes::ZAP)
							break;
						VMCASE(Opcodes::CP)
							break;
						VMCASE(Opcodes::AP)
							break;
						VMCASE(Opcodes::SP)
							break;
						VMCASE(Opcodes::MP)
							break;
						VMCASE(Opcodes::DP)
							break;
						default:
							break;
					}
					#undef VMCASE

					#undef INSTRUCTION_RR
				}

				private:
					// Program State
					enum PS : uint64_t {
						// 0
						PS_R = BIT(1),
						// 0 2-4 3
						PS_T = BIT(5),
						PS_IO = BIT(6),
						PS_EX = BIT(7),
						// KEY 8-11 4
						PS_TYPE = BIT(12),
						PS_M = BIT(13),
						PS_W = BIT(14),
						PS_P = BIT(15),
						// AS 16-17 2
						// CC 18-19 2
						// PM 20-23 4
					};

					// 64-Bit Program Status Value
					enum PS64 : uint64_t {
						// 0 24-31 8
						PS64_A = BIT(32),
						// InstructionAddress 33-63
					};

					// 128-Bit Program Status Values
					enum PS128 : uint64_t {
						PS128_RI = BIT(24),
						// 0 25-30 6
						PS128_EA = BIT(31),
						PS128_BA = BIT(32),
						// 0 33-63 31
						// InstructionAddress 64-127
					};
					

					enum AddressingMode {
						// 24-Bit Addressing Mode(EA = 0, BA = 0 Or A = 0)
						AM_24,
						// 32-Bit Addressing Mode(EA = 1, BA = 0 Or A = 1)
						AM_32,
						// 64-Bit Addressing Mode(EA = 1, BA = 1)
						AM_64,
						// Specification Exception(EA = 1, BA = 0)
						AM_SPECIFICATION_EXCEPTION,
					};

					FORCE_INLINE AddressingMode GetCurrentAddressingMode() {
						if (this->PSW[0] & PS_TYPE) {
							// 64-Bit Program State
							if (!(this->PSW[0] & PS64_A))
								return AddressingMode::AM_24;
							else
								return AddressingMode::AM_32;
						}
						else {
							// 128-Bit Program State
							if (!(this->PSW[0] & PS128_EA) && !(this->PSW[0] & PS128_BA))
								return AddressingMode::AM_24;

							else if (!(this->PSW[0] & PS128_EA) && (this->PSW[0] & PS128_BA))
								return AddressingMode::AM_32;

							else if ((this->PSW[0] & PS128_EA) && (this->PSW[0] & PS128_BA))
								return AddressingMode::AM_64;

							else if ((this->PSW[0] & PS128_EA) && !(this->PSW[0] & PS128_BA))
								return AddressingMode::AM_SPECIFICATION_EXCEPTION;
						}
					}

					FORCE_INLINE void PS_SET_PM(uint8_t ProgramMask) {
						this->PSW[0] = SET_BITS(this->PSW[0], 20, 23, ProgramMask & 0x0F);
					}
					FORCE_INLINE uint8_t PS_GET_PM() {
						return GET_BITS(this->PSW[0], 20, 23);
					}
			};
		}
	}
}