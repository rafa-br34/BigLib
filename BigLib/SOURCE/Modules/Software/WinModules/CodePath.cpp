#include "../WinSoftware.h"


// Calculate Relative JMP Address
#define RELATIVE_JUMP_ADDRESS(From, To, InstructionSize) (((To) - (From)) - (InstructionSize))


namespace BigLib {
	namespace ReversingEngineering {
		namespace Software {
			bool Memory::Detour32(void* FlowStart, void* FlowTarget, size_t PadSize) {
				constexpr size_t Size = 5;
				uint8_t Opcode[Size] = { 0xE9ui8 };	// E9 ?? ?? ?? ??

				// Min 5 Bytes For JMP Instruction
				if (PadSize < Size) return false;

				*(int32_t*)(Opcode + 1) = (int32_t)RELATIVE_JUMP_ADDRESS((int64_t)FlowStart, (int64_t)FlowTarget, Size);

				// Pad The Area After The JMP With NOPs
				Memory::FillAddress((uint8_t*)FlowStart + Size, 0x90, PadSize - Size);
				return Memory::PatchAddress(FlowStart, Opcode, PadSize);
			}

			bool Memory::Detour64(void* FlowStart, void* FlowTarget, size_t PadSize) {
				constexpr size_t Size = 9;
				uint8_t Opcode[Size] = { 0xE9ui8 };	// E9 ?? ?? ?? ?? ?? ?? ?? ??

				// Min 9 Bytes For JMP Instruction
				if (PadSize < Size) return false;

				*(int64_t*)(Opcode + 1) = (int64_t)RELATIVE_JUMP_ADDRESS((int64_t)FlowStart, (int64_t)FlowTarget, Size);

				// Pad The Area After The JMP With NOPs
				Memory::FillAddress((uint8_t*)FlowStart + Size, 0x90, PadSize - Size);
				return Memory::PatchAddress(FlowStart, Opcode, PadSize);
			}

			void* Memory::HookFunction32(void* Target, void* NewFunction, size_t PaddingSize) {
				auto GatewaySize = PaddingSize + 5;

				uint8_t* Gateway = ALLOCATE(uint8_t, GatewaySize);

				// Read First Target Bytes
				if (!Memory::ReadAddress(Target, Gateway, GatewaySize, true)) { FREE(Gateway); return nullptr; }

				// Gateway + Padding -> Function + Padding 
				Memory::Detour32(Gateway + PaddingSize, (uint8_t*)Target + PaddingSize);

				// Function -> Hook
				Memory::Detour32(Target, NewFunction, PaddingSize);

				// The Function Should Call The Gateway Which Will Finally Jump To The Gateway And Link The Hook
				return Gateway;
			}

			void* Memory::HookFunction64(void* Target, void* NewFunction, size_t PaddingSize) {
				auto GatewaySize = PaddingSize + 9;

				uint8_t* Gateway = ALLOCATE(uint8_t, GatewaySize);

				// Read First Target Bytes
				if (!Memory::ReadAddress(Target, Gateway, GatewaySize, true)) { FREE(Gateway); return nullptr; }

				// Gateway + Padding -> Function + Padding 
				Memory::Detour32(Gateway + PaddingSize, (uint8_t*)Target + PaddingSize);

				// Function -> Hook
				Memory::Detour32(Target, NewFunction, PaddingSize);

				// The Function Should Call The Gateway Which Will Finally Jump To The Gateway And Link The Hook
				return Gateway;
			}



			bool Memory::TrampolineHook::Unhook(bool OverwritePageProtection) {
				return Memory::PatchAddress(this->Target, (uint8_t*)this->Gateway, this->PaddingSize, OverwritePageProtection);
			}

			void* Memory::TrampolineHook::Hook32(void* TargetFunction, void* NewFunction, size_t Padding) {
				this->Target = TargetFunction;
				this->PaddingSize = Padding;
				this->Gateway = Memory::HookFunction32(TargetFunction, NewFunction, Padding);
				return this->Gateway;
			}

			void* Memory::TrampolineHook::Hook64(void* TargetFunction, void* NewFunction, size_t Padding) {
				this->Target = TargetFunction;
				this->PaddingSize = Padding;
				this->Gateway = Memory::HookFunction64(TargetFunction, NewFunction, Padding);
				return this->Gateway;
			}
		}
	}
}




