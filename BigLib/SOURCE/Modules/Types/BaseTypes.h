#pragma once

/*
* Compiler Data Types:
Type:       LP32    ILP32   LP64    LLP64   ILP64   SILP64

char:       8       8       8       8       8       8
short:      16      16      16      16      16      64
int:        16      32      32      32      64      64
long:       32      32      64      32      64      64
long long:  -       -       64      64      64      64
pointer:    32      32      64      64      64      64
*/
#define CHECKSIZE(Type, SizeBits) (sizeof(Type) == (SizeBits) / 8)
CONST_EXPRESSION bool DT_LP32 		= CHECKSIZE(char, 8) && CHECKSIZE(short, 16) && CHECKSIZE(int, 16) && CHECKSIZE(long, 32) /*  Skip long long  */ && CHECKSIZE(void*, 32); // Defines If The Compiler Data Type Is LP32
CONST_EXPRESSION bool DT_ILP32		= CHECKSIZE(char, 8) && CHECKSIZE(short, 16) && CHECKSIZE(int, 32) && CHECKSIZE(long, 32) /*  Skip long long  */ && CHECKSIZE(void*, 32); // Defines If The Compiler Data Type Is ILP32
CONST_EXPRESSION bool DT_LP64 		= CHECKSIZE(char, 8) && CHECKSIZE(short, 16) && CHECKSIZE(int, 32) && CHECKSIZE(long, 64) && CHECKSIZE(long long, 64) && CHECKSIZE(void*, 64); // Defines If The Compiler Data Type Is LP64
CONST_EXPRESSION bool DT_LLP64 		= CHECKSIZE(char, 8) && CHECKSIZE(short, 16) && CHECKSIZE(int, 32) && CHECKSIZE(long, 32) && CHECKSIZE(long long, 64) && CHECKSIZE(void*, 64); // Defines If The Compiler Data Type Is LLP64
CONST_EXPRESSION bool DT_ILP64 		= CHECKSIZE(char, 8) && CHECKSIZE(short, 16) && CHECKSIZE(int, 64) && CHECKSIZE(long, 64) && CHECKSIZE(long long, 64) && CHECKSIZE(void*, 64); // Defines If The Compiler Data Type Is ILP64
CONST_EXPRESSION bool DT_SILP64		= CHECKSIZE(char, 8) && CHECKSIZE(short, 64) && CHECKSIZE(int, 64) && CHECKSIZE(long, 64) && CHECKSIZE(long long, 64) && CHECKSIZE(void*, 64); // Defines If The Compiler Data Type Is SILP64


CONST_EXPRESSION bool VIRTUALIZE_DT8 = !CHECKSIZE(char, 8); // Defines If 8 Bit Datatypes Need To Be Virtualized
CONST_EXPRESSION bool VIRTUALIZE_DT16 = DT_SILP64	|| !CHECKSIZE(short, 16); // Defines If 16 Bit Datatypes Need To Be Virtualized
CONST_EXPRESSION bool VIRTUALIZE_DT32 = DT_ILP64		|| DT_SILP64		|| !CHECKSIZE(int, 32); // Defines If 32 Bit Datatypes Need To Be Virtualized
CONST_EXPRESSION bool VIRTUALIZE_DT64 = DT_LP32		|| DT_ILP32		|| !CHECKSIZE(void*, 64); // Defines If 64 Bit Datatypes Need To Be Virtualized
#undef CHECKSIZE



using int8 = signed char; // 8-Bit Signed Integer
using int16 = short; // 16-Bit Signed Integer
using int32 = int; // 32-Bit Signed Integer
using int64 = long long; // 64-Bit Signed Integer

using uint8 = unsigned char; // 8-Bit Unsigned Integer
using uint16 = unsigned short; // 16-Bit Unsigned Integer
using uint32 = unsigned int; // 32-Bit Unsigned Integer
using uint64 = unsigned long long; // 64-Bit Unsigned Integer

using byte = uint8; // Byte
using smax = int64; // Signed Maximum
using umax = uint64; // Unsigned Maximum

STATIC_ASSERT(sizeof(int8) == 1, FILELINE_SUFFIX("Invalid Size For int8"));
STATIC_ASSERT(sizeof(uint8) == 1, FILELINE_SUFFIX("Invalid Size For uint8"));

STATIC_ASSERT(sizeof(int16) == 2, FILELINE_SUFFIX("Invalid Size For int16"));
STATIC_ASSERT(sizeof(uint16) == 2, FILELINE_SUFFIX("Invalid Size For uint16"));

STATIC_ASSERT(sizeof(int32) == 4, FILELINE_SUFFIX("Invalid Size For int32"));
STATIC_ASSERT(sizeof(uint32) == 4, FILELINE_SUFFIX("Invalid Size For uint32"));

STATIC_ASSERT(sizeof(int64) == 8, FILELINE_SUFFIX("Invalid Size For int64"));
STATIC_ASSERT(sizeof(uint64) == 8, FILELINE_SUFFIX("Invalid Size For uint64"));