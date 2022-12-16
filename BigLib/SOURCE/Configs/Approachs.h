/*
* Approachs.h
* File To Control Function Approaches/Methods And Overall Configurations.
* Please Note That Editing Approaches May Impact Performance, Binary Size, Or Even Functionality.
*/
#pragma once


/*--Memory--*/
// Inline Type Used In The Memory Base Utilities
// 1: FORCE_INLINE
// 2: INLINE
// 3: None
#define APPROACH__MEMBASE_INLINE_TYPE 1

/*--Bitwise--*/
// Approach Used In The BinaryReflect Function
// 1: Normal "If" 1 Bit			(Normal "If" Statement, Ran Each Per Bit)
// 2: Branchless "If" 1 Bit		(Branchless "If" Statement, Ran Each Per Bit)
#define APPROACH__BINARYREFLECT 1


/*--Math--*/
// Approach To Use In The Absolute Function
// 1: Normal "If"		(Most Compilers Will Heavily Optimize This Approach Which Makes It Recommended)
// 2: Branchless "If"	(Can Make Code Much Faster In Some Architectures But For The Absolute Function A Speed Boost Is Unlikely)
// 3: Sign Bit Flip		(Flips The Sign Bit Making It Run Faster In Some CPUs. Doesn't Work With Floats Or Doubles)
#define APPROACH__MATH_ABSOLUTE 1