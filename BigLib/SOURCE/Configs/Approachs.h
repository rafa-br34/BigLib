/*
* Approachs.h
* File To Control Function Approachs And Overall Configurations.
* Please Note That Editing Approachs May Impact Performance For Better Or Worse.
*/

#pragma once


/*--Memory--*/
// Force Inline Memory Base Utilities
//#define MEMORY_BASE_UTILS_FORCEINLINE




/*--Math--*/

// Approach To Use In The Absolute Function
// 1: Default If		(Most Compilers Will Heavily Optimize This Approach Which Makes It Recommended)
// 2: Branchless If		(Can Make Code Much Faster In Some Architectures But For The Absolute Function A Speed Boost Is Unlikely)
// 3: Sign Bit Flip		(Flips The Sign Bit Making It Run Faster In Some CPUs. Doesn't Work With Floats Or Doubles)
// NOTE: The Following Approachs May Not Support All Datatypes And May Break CONST_EXPRESSION
// 4: Use <math> Absolute Function(abs)
// 5: Use std::abs
#define MATH_ABSOLUTE_APPROACH 1