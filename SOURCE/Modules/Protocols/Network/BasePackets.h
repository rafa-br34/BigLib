#pragma once
#include "../../../Includes.h"
#include "../../Data/DataIntegrity/SimpleChecksums.h"
#include "../../Data/BaseMemory.h"

namespace BigLib::Protocols::Network {
	
	template<typename BufferType = uint8>
	class DynamicPacket {
	private:
		Memory::MemoryBuffer p_Buffer;
	};
}