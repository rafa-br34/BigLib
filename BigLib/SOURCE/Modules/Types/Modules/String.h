#pragma once
#include "../../../Includes.h"


namespace BigLib {
	// TODO: Class...
	class CString {
	private:
		char* Buffer = nullptr;
		umax Size = 0;
		
		CONST_EXPRESSION void p_Reallocate(umax NewSize) {
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