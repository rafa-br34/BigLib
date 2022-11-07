#pragma once

// MSVC
#if _MSC_VER
#define CONST_EXPRESSION constexpr
#define FORCE_INLINE __forceinline
#define NO_INLINE __declspec(noinline)
#define INLINE inline
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
#if __x86_64__ || __ppc64__
#define PROGRAM_64
#else
#define PROGRAM_32
#endif
#endif


// Override Compiler Dependant Options If Needed
/*
#define CONST_EXPRESSION constexpr
#define FORCE_INLINE __forceinline
#define NO_INLINE __declspec(noinline)
#define INLINE inline
*/


