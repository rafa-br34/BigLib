#pragma once
#include "../../../Includes.h"


namespace BigLib {
	// TODO: Class...
	class CString {
	private:
		char* Buffer = nullptr;
		size_t Size = 0;
		
		CONST_EXPRESSION void Reallocate(size_t NewSize) {
			if (NewSize != this->Size) {
				FREE(this->Buffer);
				this->Size = NewSize;
				if (NewSize != 0) {
					this->Buffer = ALLOCATE(char, NewSize);
				}
			}

		}
	public:

	};
}