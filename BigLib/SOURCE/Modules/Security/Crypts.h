#pragma once
#include "../../Includes.h"


namespace BigLib {
	namespace Security {
		namespace Crypts {

			// TODO: Remove STDs :)
			template <typename Class = int>
			Class Random(Class Min, Class Max) {
				std::random_device RandomDevice;
				std::mt19937 Generator((unsigned int)RandomDevice() * (unsigned int)(std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1)));
				std::uniform_int_distribution<Class> Distribution(Min, Max);

				return Distribution(Generator);
			}

			char* RandomStringA(unsigned int Length);
			wchar_t* RandomStringW(unsigned int Length);
		}

	}
}
