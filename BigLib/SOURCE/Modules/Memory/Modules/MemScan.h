#pragma once
#include "../../../Includes.h"
#include "../../Utils/Utils.h"

namespace BigLib {
	namespace Software {
		// Namespace With All Basic Memory Utilities
		namespace Memory {
			// 8 Bits Used For The Actual Descriptor, 8 Bits For Wildcard Descriptor
			typedef uint16_t MemoryScanByte;

			// Makes A MemoryScanByte List From Characters, Use '?' For Wildcards
			// Ex: "81 F? ?0 D8 FF ?? 7E"
			INLINE size_t MaskFromStringA(const char* String) {
				for (size_t i = 0; i < Utils::CStringLength(String); i++) {

				}
			}
		}
	}
}