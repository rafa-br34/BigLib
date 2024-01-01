#pragma once

/* Misc */
#define STRINGFY(Item) MAKE_STRING(Item)
#define MAKE_STRING(Item) #Item

/* IO */
// Replace These To Custom Allocate/Deallocate Methods
#define ALLOCATE(Type, Amount) (Type*)BigLib::Allocate(sizeof(Type) * (Amount)) // new Type[Amount]
#define FREE(Pointer) BigLib::Free((void*)Pointer) // delete[] Pointer

#define BIGGEST_DATATYPE long long

#define PLACE_DEBUG(DebugObject) DebugObject

/* Debug */
// Exceptions
#define BASE_EXCEPTION_MESSAGE " File: \"" __FILE__ " Line " STRINGFY(__LINE__)
#define FILELINE_SUFFIX(Message) Message " <Line: " STRINGFY(__LINE__) " File: " __FILE__ ">"

#define THROW(String) throw String;

// Inverse Assert
#define IASSERT(Condition)				PLACE_DEBUG(if (Condition) { THROW("Assertion \"" STRINGFY(Condition) "\" Failed " BASE_EXCEPTION_MESSAGE) })
// Inverse Extended Assert
#define IASSERT_EX(Condition, Message)	PLACE_DEBUG(if (Condition) { THROW(Message " " BASE_EXCEPTION_MESSAGE) })

// Assert
#define ASSERT(Condition)				PLACE_DEBUG(IASSERT(!(Condition)))
// Extended Assert
#define ASSERT_EX(Condition, Message)	PLACE_DEBUG(IASSERT_EX(!(Condition), Message))

