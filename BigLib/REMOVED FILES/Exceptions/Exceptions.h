#pragma once
#include "../../Includes.h"
#include "../Software/Software.h"
#include "../Data/Bitwise.h"

#ifdef _WIN64
#define REG(Register) R ## Register
#else
#define REG(Register) E ## Register
#endif // _WIN32



namespace Exceptions {
	class Exception {
	public:
		EXCEPTION_RECORD ExceptionRecord{};
		CONTEXT ContextRecord{};
		UINT ErrorCode{};

		Exception(UINT ErrorCode, const CONTEXT& CPUContext, const EXCEPTION_RECORD& Record) {
			this->ErrorCode = ErrorCode;
			this->ContextRecord = CPUContext;
			this->ExceptionRecord = Record;
		}

		std::string ToString() {
			std::string Text = "";

			Text.append("\nError Code: " + this->TypeToString(this->ErrorCode, "%X"));
			
			Text.append("\n--<Exception Recording>--");
			Text.append("\n\tException Address: " + this->TypeToString(this->ExceptionRecord.ExceptionAddress, "%X"));
			Text.append("\n\tException Code: " + this->TypeToString(this->ExceptionRecord.ExceptionCode, "%X"));
			Text.append("\n\tException Flags: " + this->TypeToString(this->ExceptionRecord.ExceptionFlags, "%X"));
			
			Text.append("\n--<Exception CPU Context>--");

			Text.append("\n<General Purpose Registers>");
			Text.append("\n\tEAX: " + this->TypeToString(this->ContextRecord.REG(ax), "%X"));
			Text.append("\n\tECX: " + this->TypeToString(this->ContextRecord.REG(cx), "%X"));
			Text.append("\n\tEDX: " + this->TypeToString(this->ContextRecord.REG(dx), "%X"));
			Text.append("\n\tEBX: " + this->TypeToString(this->ContextRecord.REG(bx), "%X"));

			Text.append("\n<Pointer Registers>");
			Text.append("\n\tEBP: " + this->TypeToString(this->ContextRecord.REG(bp), "%X"));
			Text.append("\n\tESP: " + this->TypeToString(this->ContextRecord.REG(sp), "%X"));
			Text.append("\n\tEIP: " + this->TypeToString(this->ContextRecord.REG(ip), "%X"));

			Text.append("\n<Index Registers>");
			Text.append("\n\tEDI: " + this->TypeToString(this->ContextRecord.REG(di), "%X"));
			Text.append("\n\tESI: " + this->TypeToString(this->ContextRecord.REG(si), "%X"));
			
			
			const char* FlagsNames[] = {
				// FLAGS
				"CF",
				"R1",
				"PF",
				"R_",
				"AF",
				"R_",
				"ZF",
				"SF",
				"TF",
				"IF",
				"DF",
				"OF",
				"IOPL",
				"NT",
				"R1",
				// EFLAGS
				"RF",
				"VM",
				"AC",
				"VIF",
				"VIP",
				"ID",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				// RFLAGS
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
				"R_",
			};

			Text.append("\n<EFLAGS>\n");
			// TODO: EFlags Is Actually RFlags In 64 So Once Microsoft Adds It We Should Use REG On It.
			for (UINT i = 0; i < sizeof(this->ContextRecord.EFlags) * 8; i++) {
				Text.append(std::string(FlagsNames[i]) + ": " + std::string(HAS_BIT(this->ContextRecord.EFlags, i + 1) ? "TRUE" : "FALSE") + '\n');
			}

			return Text;
		}

	private:

		template <typename Class>
		std::string TypeToString(Class Value, const char* Format) {
			auto BufferSize = snprintf(0, NULL, Format, Value) + 1;

			char* Buffer = new (std::nothrow) char[BufferSize];
			sprintf_s(Buffer, BufferSize, Format, Value);
			return std::string(Buffer);
		}
	};

	void InitHandlers(_se_translator_function CustomTranslator);
	void InitHandlers();
}

#undef REG

