/*
* Approachs.h
* File To Control Function Approaches/Methods And Overall Configurations.
* Please Note That Editing Approaches May Impact Performance, Binary Size, Or Even Functions.
*/
#pragma once


/*--Memory--*/
// Inline Type Used In The Memory Base Utilities
// 1: FORCE_INLINE
// 2: INLINE
// 3: None
#define APPROACH__MEMBASE_INLINE_TYPE 1




/*--Math--*/
// Approach To Use In The Absolute Function
// 1: Default If		(Most Compilers Will Heavily Optimize This Approach Which Makes It Recommended)
// 2: Branchless If		(Can Make Code Much Faster In Some Architectures But For The Absolute Function A Speed Boost Is Unlikely)
// 3: Sign Bit Flip		(Flips The Sign Bit Making It Run Faster In Some CPUs. Doesn't Work With Floats Or Doubles)
// NOTE: The Following Approachs May Not Support All Datatypes And May Break CONST_EXPRESSION
// 4: Use <math> Absolute Function(abs)
// 5: Use std::abs
#define APPROACH__MATH_ABSOLUTE 1