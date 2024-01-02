#pragma once

// MSVC
#if _MSC_VER
#define STATIC_ASSERT(Value, Message) static_assert(Value, Message)
#define CONST_EXPRESSION constexpr
#define FORCE_INLINE __forceinline
#define NO_INLINE __declspec(noinline)
#define VOLATILE volatile
#define INLINE inline
#define STATIC static
#define CONST const
#define STATIC_CONST STATIC CONST

#define UI64(Literal) Literal ## ui64
#define UI32(Literal) Literal ## ui32
#define UI16(Literal) Literal ## ui16
#define UI8(Literal) Literal ## ui8

#define I64(Literal) Literal ## i64
#define I32(Literal) Literal ## i32
#define I16(Literal) Literal ## i16
#define I8(Literal) Literal ## i8
#endif

// GCC
#if __GNUC__
#define STATIC_ASSERT(Value, Message) static_assert(Value, Message)
#define CONST_EXPRESSION constexpr
#define FORCE_INLINE __attribute__((always_inline)) inline
#define NO_INLINE __attribute__((noinline))
#define VOLATILE volatile
#define INLINE inline
#define STATIC static
#define CONST const
#define STATIC_CONST STATIC CONST

#define UI64(Literal) Literal ## ULL
#define UI32(Literal) Literal ## UL
#define UI16(Literal) Literal ## U
#define UI8(Literal) Literal ## U

#define I64(Literal) Literal ## LL
#define I32(Literal) Literal ## L
#define I16(Literal) Literal
#define I8(Literal) Literal
#endif


#define BIGGEST_LITERAL UI64

/*
* 
* Architecture
* 
*/
namespace BigLib {
	namespace Architectures {
		enum ArchitectureEnum {
			_UNKNOWN = 0,

			X86_X64, X86_X32,

			IA_64,

			ARM_64,
			ARM_2,
			ARM_3, ARM_3M,
			ARM_4T,
			ARM_5, ARM_5E,
			ARM_5T, ARM_5TE, ARM_5TEJ,
			ARM_6, ARM_6J, ARM_6K, ARM_6Z, ARM_6ZK,
			ARM_6T2,
			ARM_7, ARM_7A, ARM_7R, ARM_7M, ARM_7S,

			ALPHA_EV4, ALPHA_EV5, ALPHA_EV6,

			M68K,
			BLACKFIN,
			EPIPHANY,
			MIPS,
			SUPERH,
			SPARC,
			POWER_PC, POWER_PC64,

			PA_RISC1_0, PA_RISC1_1, PA_RISC2_0,

			_ENUM_LAST
		};

		const char* const ArchitectureName[] = {
			"_UNKNOWN",

			"X86_X64", "X86_X32",

			"IA_64",

			"ARM_64",
			"ARM_2",
			"ARM_3", "ARM_3M",
			"ARM_4T",
			"ARM_5", "ARM_5E",
			"ARM_5T", "ARM_5TE", "ARM_5TEJ",
			"ARM_6", "ARM_6J", "ARM_6K", "ARM_6Z", "ARM_6ZK",
			"ARM_6T2",
			"ARM_7", "ARM_7A", "ARM_7R", "ARM_7M", "ARM_7S",

			"ALPHA_EV4", "ALPHA_EV5", "ALPHA_EV6",

			"M68K",
			"BLACKFIN",
			"EPIPHANY",
			"MIPS",
			"SUPERH",
			"SPARC",
			"POWER_PC", "POWER_PC64",

			"PA_RISC1_0", "PA_RISC1_1", "PA_RISC2_0",

			"_ENUM_LAST"
		};
	}
}

#define SET_ARCH(_Arch_) namespace BigLib { constexpr Architectures::ArchitectureEnum c_MachineArchitecture = Architectures::ArchitectureEnum::_Arch_; }

#if (__x86_64)||(__x86_64__)||(__amd64)||(__amd64__)||(_M_X64)||(_M_AMD64)
SET_ARCH(X86_X64)

#elif (i386)||(__i386__)||(__i386)||(_M_IX86)
SET_ARCH(X86_X32)

#elif (__ia64__)||(_IA64)||(__IA64__)||(__ia64)||(_M_IA64)||(__itanium__)
SET_ARCH(IA_64)

#elif (__aarch64__)||(_M_ARM64)
SET_ARCH(ARM_64)

#elif (__ARM_ARCH_2__)
SET_ARCH(ARM_2)

#elif (__ARM_ARCH_3__)
SET_ARCH(ARM_3)

#elif (__ARM_ARCH_3M__)
SET_ARCH(ARM_3M)

#elif (__ARM_ARCH_4T__)||(__TARGET_ARM_4T)
SET_ARCH(ARM_4T)

#elif (__ARM_ARCH_5__)
SET_ARCH(ARM_5)

#elif (__ARM_ARCH_5E__)
SET_ARCH(ARM_5E)

#elif (__ARM_ARCH_5T__)
SET_ARCH(ARM_5T)

#elif (__ARM_ARCH_5TE__)
SET_ARCH(ARM_5TE)

#elif (__ARM_ARCH_5TEJ__)
SET_ARCH(ARM_5TEJ)

#elif (__ARM_ARCH_6__)
SET_ARCH(ARM_6)

#elif (__ARM_ARCH_6J__)
SET_ARCH(ARM_6J)

#elif (__ARM_ARCH_6K__)
SET_ARCH(ARM_6K)

#elif (__ARM_ARCH_6Z__)
SET_ARCH(ARM_6Z)

#elif (__ARM_ARCH_6ZK__)
SET_ARCH(ARM_6ZK)

#elif (__ARM_ARCH_6T2_)||(__ARM_ARCH_6T2_)
SET_ARCH(ARM_6T2)

#elif (__ARM_ARCH_7__)
SET_ARCH(ARM_7)

#elif (__ARM_ARCH_7A__)
SET_ARCH(ARM_7A)

#elif (__ARM_ARCH_7R__)
SET_ARCH(ARM_7R)

#elif (__ARM_ARCH_7M__)
SET_ARCH(ARM_7M)

#elif (__ARM_ARCH_7S__)
SET_ARCH(ARM_7S)

#elif (__alpha_ev4__)
SET_ARCH(ALPHA_EV4)

#elif (__alpha_ev5__)
SET_ARCH(ALPHA_EV5)

#elif (__alpha_ev6__)
SET_ARCH(ALPHA_EV6)

#elif (__m68k__)
SET_ARCH(M68K)

#elif (__bfin)||(__BFIN__)
SET_ARCH(BLACKFIN)

#elif (__epiphany__)
SET_ARCH(EPIPHANY)

#elif (mips)||(__mips__)||(__mips)
SET_ARCH(MIPS)

#elif (__sh__)
SET_ARCH(SUPERH)

#elif (__sparc__)||(__sparc)
SET_ARCH(SPARC)

#elif (__powerpc64__)||(__PPC64__)||(__ppc64__)||(_ARCH_PPC64)
SET_ARCH(POWER_PC64)

#elif (__powerpc)||(__powerpc__)||(__POWERPC__)||(__ppc__)||(__PPC)||(__PPC__)||(_ARCH_PPC)
SET_ARCH(POWER_PC)

#else
SET_ARCH(UNKNOWN)
#endif

#undef SET_ARCH



// Override Compiler Dependant Options If Needed
/*
#define CONST_EXPRESSION constexpr
#define FORCE_INLINE __forceinline
#define NO_INLINE __declspec(noinline)
#define INLINE inline
*/