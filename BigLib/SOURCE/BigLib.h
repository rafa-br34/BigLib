#pragma once
#include "Configs.h"

#define BIGLIB_VERSION_COMMITS 50

#ifdef BIGLIB_PRINT_PREPROCESSED_DATA
#pragma message("-- BIGLIB COMPILATION INFO --\nBigLib Version " STRINGFY(BIGLIB_VERSION_COMMITS) "\nArchitecture " ARCH_CURRENT_NAME "(ID: " STRINGFY(ARCH_CURRENT_ID) ")" "\n-- BIGLIB COMPILATION INFO --")
#endif

// Memory
#include "Modules/Memory/Memory.h"

// Code
#include "Modules/Code/Code.h"

// Data
#include "Modules/Data/Bitwise.h"
#include "Modules/Data/Random.h"
#include "Modules/Data/Math.h"

// Crypts
#include "Modules/Security/Crypts.h"

// MultiThreading
#include "Modules/MultiThreading/ThreadSafeQueue/ThreadSafeQueue.h"
#include "Modules/MultiThreading/ThreadPool/ThreadPool.h"

// Types
#include "Modules/Types/Types.h"

// Machine Learning
#include "Modules/MachineLearning/NeuralNetwork.h"


