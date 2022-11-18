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
// TODO: Fix
#define UI16(Literal) Literal ## U
#define UI8(Literal) Literal ## U

#define I64(Literal) Literal ## LL
#define I32(Literal) Literal ## L
// TODO: Fix
#define I16(Literal) Literal
#define I8(Literal) Literal

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


