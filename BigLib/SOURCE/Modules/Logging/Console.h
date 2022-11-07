/*
* Name: Logging/Consoles Library Header
* Desc: Library For Logging Info, Creating Consoles.
* Status: WIP
*/

#pragma once

// STD
#include <iostream>
#include <vector>
#include <string>
#include <mutex>

// Windows Libraries
#include <Windows.h>


#define LOGGING_USE_DEFAULT_CONSOLE 1

extern std::mutex LOGGING_MUTEX;

#define LOGGING_DEFAULT_CONSOLE (Console::GetLastLinked())
#define LOGGING_TAG(ConPtr, Tag, Back, Fore)\
if (ConPtr != nullptr) {\
	std::cout << "["; ConPtr->SetColor(Back, Fore); std::cout << Tag; ConPtr->ResetColor(); std::cout << "]";\
} else {\
	std::cout << "[" << Tag << "]";\
};std::cout


#define LOGGING_LOCK				LOGGING_MUTEX.lock();
#define LOGGING_UNLOCK				LOGGING_MUTEX.unlock();
#define LOGGING_LOCKED(Log)			LOGGING_LOCK;Log;LOGGING_UNLOCK;

#ifdef LOGGING_USE_DEFAULT_CONSOLE

#define LOGGING_ERROR			LOGGING_TAG(LOGGING_DEFAULT_CONSOLE, "ERROR", ConsoleColors::BLACK, ConsoleColors::RED)
#define LOGGING_WARNING			LOGGING_TAG(LOGGING_DEFAULT_CONSOLE, "WARNING", ConsoleColors::BLACK, ConsoleColors::YELLOW)
#define LOGGING_INFO			LOGGING_TAG(LOGGING_DEFAULT_CONSOLE, "INFO", ConsoleColors::BLACK, ConsoleColors::CYAN)

#define LOGGING_SUCCESS			LOGGING_TAG(LOGGING_DEFAULT_CONSOLE, "SUCCESS", ConsoleColors::BLACK, ConsoleColors::LIGHTGREEN)
#define LOGGING_DEBUG			LOGGING_TAG(LOGGING_DEFAULT_CONSOLE, "DEBUG", ConsoleColors::BLACK, ConsoleColors::PURPLE)
#define LOGGING_FAILED			LOGGING_TAG(LOGGING_DEFAULT_CONSOLE, "FAILED", ConsoleColors::BLACK, ConsoleColors::LIGHTRED)

#define LOGGING_LL_ERROR		LOGGING_TAG(LOGGING_DEFAULT_CONSOLE, "LOW_LEVEL_ERROR", ConsoleColors::BLACK, ConsoleColors::LIGHTRED)
#define LOGGING_LL_WARNING		LOGGING_TAG(LOGGING_DEFAULT_CONSOLE, "LOW_LEVEL_WARNING", ConsoleColors::BLACK, ConsoleColors::LIGHTYELLOW)
#define LOGGING_LL_INFO			LOGGING_TAG(LOGGING_DEFAULT_CONSOLE, "LOW_LEVEL_INFO", ConsoleColors::BLACK, ConsoleColors::LIGHTCYAN)
#else

#define LOGGING_ERROR(Con)		LOGGING_TAG(Con, "ERROR", ConsoleColors::BLACK, ConsoleColors::RED)
#define LOGGING_WARNING(Con)	LOGGING_TAG(Con, "WARNING", ConsoleColors::BLACK, ConsoleColors::YELLOW)
#define LOGGING_INFO(Con)		LOGGING_TAG(Con, "INFO", ConsoleColors::BLACK, ConsoleColors::CYAN)

#define LOGGING_SUCCESS(Con)	LOGGING_TAG(Con, "SUCCESS", ConsoleColors::BLACK, ConsoleColors::LIGHTGREEN)
#define LOGGING_DEBUG			LOGGING_TAG(LOGGING_DEFAULT_CONSOLE, "DEBUG", ConsoleColors::BLACK, ConsoleColors::PURPLE)
#define LOGGING_FAILED(Con)		LOGGING_TAG(Con, "FAILED", ConsoleColors::BLACK, ConsoleColors::LIGHTRED)


#define LOGGING_LL_ERROR(Con)	LOGGING_TAG(Con, "LOW_LEVEL_ERROR", ConsoleColors::BLACK, ConsoleColors::LIGHTRED)
#define LOGGING_LL_WARNING(Con)	LOGGING_TAG(Con, "LOW_LEVEL_WARNING", ConsoleColors::BLACK, ConsoleColors::LIGHTYELLOW)
#define LOGGING_LL_INFO(Con)	LOGGING_TAG(Con, "LOW_LEVEL_INFO", ConsoleColors::BLACK, ConsoleColors::LIGHTCYAN)
#endif



enum ConsoleColors {
	BLACK = 0,
	BLUE,
	GREEN,
	CYAN,
	RED,
	PURPLE,
	YELLOW,
	LIGHTGRAY,
	GRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTPURPLE,
	LIGHTYELLOW,
	WHITE,
};


class Console {
public:
	
	static Console* GetLastLinked();

	void SetColor(int BackgroundColor, int ForegroundColor);
	void ResetColor();
	void RenameTitle(const char* Title);

	bool AllocateConsole();
	bool InitBypasses();
	bool ResetBypasses();

	bool KillConsole();

	bool LinkConsole();
	bool LinkConsole(const std::string& StdoutFile, const std::string& StderrFile, const std::string& StdinFile);
	bool UnlinkConsole();

private:
	//bool p_RemovedFromList();
	//bool p_AddedToList();
	
	bool p_ResetBypasses();
	bool p_ExecBypasses();
	

	HWND p_ConsoleWindow = NULL;
	FILE* p_ConLinkFile = nullptr;
	bool p_IsLinked = false;
	bool p_IsOnList = false;
	bool p_Bypassed = false;

	
	std::vector<void*> p_Bypasses{};

};


