#pragma once

// Defines If The Target Is Windows OS
#define TARGET_WINDOWS
#define USE_STD_LIB_FILES


#ifdef TARGET_WINDOWS
#define WINDOWS_DBG_PRINT " WINDOWS_DBG_PRINT LastError: " << GetLastError() << " In File \"" << __FILE__ << "\" Line " << __LINE__ << '\n'
#endif // TARGET_WINDOWS
