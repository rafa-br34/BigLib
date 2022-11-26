#pragma once

// MSVC
#if _MSC_VER
#define CONST_EXPRESSION constexpr
#define FORCE_INLINE __forceinline
#define NO_INLINE __declspec(noinline)
#define INLINE inline

#define UI64(Literal) Literal ## ui64
#define UI32(Literal) Literal ## ui32
#define UI16(Literal) Literal ## ui16
#define UI8(Literal) Literal ## ui8

#define I64(Literal) Literal ## i64
#define I32(Literal) Literal ## i32
#define I16(Literal) Literal ## i16
#define I8(Literal) Literal ## i8

#if _WIN64
#define PROGRAM_64
#else
#define PROGRAM_32
#endif
#endif

// GCC
#if __GNUC__
#define CONST_EXPRESSION constexpr
#define FORCE_INLINE __attribute__((always_inline)) inline
#define NO_INLINE __attribute__((noinline))
#define INLINE inline

#define UI64(Literal) Literal ## ULL
#define UI32(Literal) Literal ## UL
#define UI16(Literal) Literal ## U
#define UI8(Literal) Literal ## U

#define I64(Literal) Literal ## LL
#define I32(Literal) Literal ## L
#define I16(Literal) Literal
#define I8(Literal) Literal

#if __x86_64__ || __ppc64__
#define PROGRAM_64
#else
#define PROGRAM_32
#endif
#endif

/*
* 
* Architecture
* 
*/

#define ARCH_X86_64    0
#define ARCH_X86_32    1
#define ARCH_ARM2      2
#define ARCH_ARM3      3
#define ARCH_ARM4T     4
#define ARCH_ARM5      5
#define ARCH_ARM6T2    6
#define ARCH_ARM6      7
#define ARCH_ARM7      8
#define ARCH_ARM7A     9
#define ARCH_ARM7R     10
#define ARCH_ARM7M     11
#define ARCH_ARM7S     12
#define ARCH_ARM64     13
#define ARCH_MIPS      14
#define ARCH_SUPERH    15
#define ARCH_POWERPC   16
#define ARCH_POWERPC64 17
#define ARCH_SPARC     18
#define ARCH_M68K      19
#define ARCH_UNKNOWN   20


#if defined(__x86_64__) || defined(_M_X64)
#define ARCH_NAME "x86_64"
#define ARCH_ID 0
#elif defined(i386)				|| defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define ARCH_NAME "x86_32"
#define ARCH_ID 1
#elif defined(__ARM_ARCH_2__)
#define ARCH_NAME "ARM2"
#define ARCH_ID 2
#elif defined(__ARM_ARCH_3__)	|| defined(__ARM_ARCH_3M__)
#define ARCH_NAME "ARM3"
#define ARCH_ID 3
#elif defined(__ARM_ARCH_4T__)	|| defined(__TARGET_ARM_4T)
#define ARCH_NAME "ARM4T"
#define ARCH_ID 4
#elif defined(__ARM_ARCH_5_)	|| defined(__ARM_ARCH_5E_)
#define ARCH_NAME "ARM5"
#define ARCH_ID 5
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
#define ARCH_NAME "ARM6T2"
#define ARCH_ID 6
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
#define ARCH_NAME "ARM6"
#define ARCH_ID 7
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARCH_NAME "ARM7"
#define ARCH_ID 8
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARCH_NAME "ARM7A"
#define ARCH_ID 9
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARCH_NAME "ARM7R"
#define ARCH_ID 10
#elif defined(__ARM_ARCH_7M__)
#define ARCH_NAME "ARM7M"
#define ARCH_ID 11
#elif defined(__ARM_ARCH_7S__)
#define ARCH_NAME "ARM7S"
#define ARCH_ID 12
#elif defined(__aarch64__) || defined(_M_ARM64)
#define ARCH_NAME "ARM64"
#define ARCH_ID 13
#elif defined(mips) || defined(__mips__) || defined(__mips)
#define ARCH_NAME "MIPS"
#define ARCH_ID 14
#elif defined(__sh__)
#define ARCH_NAME "SUPERH"
#define ARCH_ID 15
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
#define ARCH_NAME "POWERPC"
#define ARCH_ID 16
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#define ARCH_NAME "POWERPC64"
#define ARCH_ID 17
#elif defined(__sparc__) || defined(__sparc)
#define ARCH_NAME "SPARC"
#define ARCH_ID 18
#elif defined(__m68k__)
#define ARCH_NAME "M68K"
#define ARCH_ID 19
#else
#define ARCH_NAME "UNKNOWN";
#define ARCH_ID 20
#endif
  


// Override Compiler Dependant Options If Needed
/*
#define CONST_EXPRESSION constexpr
#define FORCE_INLINE __forceinline
#define NO_INLINE __declspec(noinline)
#define INLINE inline
*/


