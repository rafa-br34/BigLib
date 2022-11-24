#pragma once


#define STRINGFY(Item) MAKE_STRING(Item)
#define MAKE_STRING(Item) #Item


#define DEBUG_PRINT(Printer) Printer

// Replace These To Custom Allocate/Deallocate Methods
#define ALLOCATE(Type, Amount) new Type[Amount]
#define FREE(Pointer) delete[] Pointer

// Exceptions
#define BASE_EXCEPTION_MESSAGE "At File: \"" __FILE__ "\" Function: \"" __FUNCSIG__ "\" Line " STRINGFY(__LINE__)
#define THROW(String) throw String;

// Inverse Assert
#define IASSERT(Condition)				if (Condition) { THROW("Assertion \"" STRINGFY(Condition) "\" Failed " BASE_EXCEPTION_MESSAGE) }
// Inverse Extended Assert
#define IASSERT_EX(Condition, Message)	if (Condition) { THROW(Message " " BASE_EXCEPTION_MESSAGE) }

// Assert
#define ASSERT(Condition)				IASSERT(!(Condition))
// Extended Assert
#define ASSERT_EX(Condition, Message)	IASSERT_EX(!(Condition), Message)



#define BIGGEST_DATATYPE long long
