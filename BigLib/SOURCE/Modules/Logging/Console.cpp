/*
* Name: Logging/Consoles Library Source
* Desc: Library For Logging Info, Creating Consoles.
* Status: Needs A Bit Of Rewriting
*/

#include "Console.h"


std::mutex LOGGING_MUTEX{};


enum BypassType {
	ChangeFuncToRet,
	CallFunc,
};


struct BypassItem {
	const char* Name;				// Bypass Name
	BypassType Type;				// Bypass Type
	void* Function;					// Bypass Function
	unsigned char Cache[32] = {};	// Cache For Reseting the bypass
};

static std::vector<Console*> g_AllocatedConsoles{};
Console* g_LastLinkedCon = nullptr;

// yes, yes, ik it is totally over engineered, but it is useful to add console bypasses
// or even mods, that would be fun
static BypassItem g_Bypasses[] = {
	{"FreeConsoleBypass", BypassType::ChangeFuncToRet, FreeConsole},
};







bool Console::AllocateConsole() {
	AllocConsole();
	this->p_ConsoleWindow = GetConsoleWindow();
	g_AllocatedConsoles.push_back(this);
	return true;
}

bool Console::InitBypasses() {
	return this->p_ExecBypasses();
}

bool Console::ResetBypasses() {
	return this->p_ResetBypasses();
}

bool Console::LinkConsole() {
	return this->LinkConsole("CONOUT$", "CONOUT$", "CONIN$");
}


bool Console::LinkConsole(const std::string& StdoutFile, const std::string& StderrFile, const std::string& StdinFile) {
	FILE* File = nullptr;
	freopen_s(&File, StdoutFile.c_str(), "w", stdout);
	freopen_s(&File, StderrFile.c_str(), "w", stderr);
	freopen_s(&File, StdinFile.c_str(), "r", stdin);
	std::ios::sync_with_stdio(true);
	this->p_ConLinkFile = File;
	g_LastLinkedCon = this;
	this->p_IsLinked = true;
	return true;
}

bool Console::UnlinkConsole() {
	fclose(this->p_ConLinkFile);
	FILE* File = nullptr;
	freopen_s(&File, "NUL:", "w", stdout);
	freopen_s(&File, "NUL:", "w", stderr);
	freopen_s(&File, "NUL:", "r", stdin);
	fclose(File);
	if (g_LastLinkedCon == this) {
		g_LastLinkedCon = nullptr;
	}
	this->p_IsLinked = false;
	return true;
}


Console* Console::GetLastLinked() {
	return g_LastLinkedCon;
}

bool Console::KillConsole() {
	if (this->p_IsLinked) {
		this->UnlinkConsole();
	}
	if (this->p_Bypassed) {
		this->ResetBypasses();
	}
	// Thanks stackoverflow
	g_AllocatedConsoles.erase(
		std::remove(g_AllocatedConsoles.begin(), g_AllocatedConsoles.end(), this),
		g_AllocatedConsoles.end()
	);
	FreeConsole();
	return true;
}

void Console::SetColor(int BackgroundColor, int ForegroundColor) {
	static HANDLE ConHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(ConHandle, (WORD)((16 * BackgroundColor) + ForegroundColor));
}

void Console::ResetColor() {
	this->SetColor(ConsoleColors::BLACK, ConsoleColors::WHITE);
}

void Console::RenameTitle(const char* Title) {
	SetConsoleTitleA(Title);
}

bool Console::p_ExecBypasses() {
	for (uintptr_t i = 0; i < (uintptr_t)(sizeof(g_Bypasses) / sizeof(BypassItem)); i++) {
		// Create A New Bypass Item Since We Are Using Pointers
		// We Could Just Let The Vector Copy It
		// But Then The BypassItem Struct Needs To Be Exported
		BypassItem* Item = new (std::nothrow) BypassItem;
		*Item = g_Bypasses[i];

		switch (Item->Type)
		{
		case BypassType::ChangeFuncToRet:
			// Make The Compiler Belive It Is A Byte* Instead Of A const Function Pointer
			Item->Cache[0] = *reinterpret_cast<byte*>(Item->Function);
			// Save First Byte And The change To Ret Near
			DWORD OldProtect;
			VirtualProtect(Item->Function, 1, PAGE_EXECUTE_READWRITE, &OldProtect);
			
			*reinterpret_cast<byte*>(Item->Function) = 0xC3;

			VirtualProtect(Item->Function, 1, OldProtect, 0);
			break;

		case BypassType::CallFunc:
			// Create Function Def, And Call Hoping It Won't Kill Our DLL
			typedef uint8_t Function(bool DoBypass);
			((Function*)Item->Function)(true);
			break;
		default:
			break;
		}
		// Push Our Brand New Bypass To The Vector
		this->p_Bypasses.push_back(Item);
	}
	this->p_Bypassed = true;
	return true; // TODO: In A Case Of Exception It Wouldn't Return False...
}




bool Console::p_ResetBypasses() {
	for (uintptr_t i = 0; i < this->p_Bypasses.size(); i++) {
		// Get The Bypasses In Our Executed Bypasses List
		BypassItem* Bypass = (BypassItem*)(this->p_Bypasses[i]);
		

		switch (Bypass->Type)
		{
		case BypassType::ChangeFuncToRet:
			// Reset bypass
			DWORD OldProtect;
			VirtualProtect(Bypass->Function, 1, PAGE_EXECUTE_READWRITE, &OldProtect);
			std::cout << Bypass->Cache[0] << std::endl;
			*reinterpret_cast<byte*>(Bypass->Function) = Bypass->Cache[0];

			VirtualProtect(Bypass->Function, 1, OldProtect, 0);
			break;

		case BypassType::CallFunc:
			// Create Function Def, And Call To Revert The Bypass
			typedef uint8_t Function(bool DoBypass);
			((Function*)Bypass->Function)(false);
			break;
		default:
			break;
		}
		delete Bypass;
	}
	this->p_Bypassed = false;
	return true; // TODO: Exception Handling
}