#pragma once

// Defines If The Target Is Windows OS
#define BIGLIB_TARGETS_WINDOWS		0 // Windows OS
#define BIGLIB_TARGETS_LINUX		1 // Linux
#define BIGLIB_TARGETS_MACOS		2 // MacOS
#define BIGLIB_TARGETS_ANDROID		3 // Android
#define BIGLIB_TARGETS_USER_DEFINED 4 // User Defined, Use This For "Low-Level" Use Cases(Kernels, UEFI, BIOS, Etc).

// Desired Target
#define BIGLIB_TARGET BIGLIB_TARGETS_WINDOWS

// Use Standard Library Files
#define USE_STD_LIB_FILES


#if BIGLIB_TARGET == BIGLIB_TARGETS_WINDOWS
#define WINDOWS_DBG_PRINT " WINDOWS_DBG_PRINT LastError: " << GetLastError() << " In File \"" << __FILE__ << "\" Line " << __LINE__ << '\n'
#endif // BIGLIB_TARGET
