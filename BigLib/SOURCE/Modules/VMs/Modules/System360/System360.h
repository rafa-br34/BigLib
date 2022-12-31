#pragma once
#include "../../../../Includes.h"

namespace BigLib {
	namespace VMs {
		namespace System360 {

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
				BCR,			// <RR>		Branch On Condition Register
				NOPR = 0x070,	// <RR>		No Operation Register
				BR = 0x07F,		// <RR>		Branch Register (Unconditional)
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
				BC,				// <RX>		Branch On Condition
				
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

			struct VMState {
				uint64_t	PSW;
				double		D[4];
				uint32_t	R[16];


			};
		}
	}
}