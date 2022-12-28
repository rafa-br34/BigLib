#pragma once
#include "Configs.h"

#ifdef USE_STD_LIB_FILES
#include <condition_variable>
#include <atomic>
#include <mutex>

#include <stdint.h>
#include <math.h>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <tuple>
#endif

#if BIGLIB_TARGET == BIGLIB_TARGETS_WINDOWS
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <winternl.h>

#include <intrin.h>
#include <eh.h>
#endif // BIGLIB_TARGET

