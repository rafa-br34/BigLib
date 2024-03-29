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
#define PREFS_MEMBASE_INLINE_TYPE 1

/*--Bitwise--*/
// Approach Used In The BinaryReflect Function
// 1: Normal "If" 1 Bit			(Normal "If" Statement, Ran Each Per Bit)
// 2: Branchless "If" 1 Bit		(Branchless "If" Statement, Ran Each Per Bit)
#define PREFS_BINARYREFLECT 1


/*--Math--*/
// Approach To Use In The Absolute Function
// 1: Normal "If"			(Most Compilers Will Heavily Optimize This Approach Which Makes It Recommended)
// 2: Branchless "If"		(Can Make Code Much Faster In Some Architectures But For The Absolute Function A Speed Boost Is Unlikely)
// 3: Sign Bit Flip		(Flips The Sign Bit Making It Run Faster In Some CPUs. Doesn't Work With Floats Or Doubles)
#define PREFS_MATH_ABSOLUTE 1