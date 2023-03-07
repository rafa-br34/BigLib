#pragma once
#include "Configs.h"

#ifdef BIGLIB_PRINT_PREPROCESSED_DATA
#pragma message("-- BIGLIB COMPILATION INFO --\nArchitecture " ARCH_CURRENT_NAME "(ID: " STRINGFY(ARCH_CURRENT_ID) ")" "\n-- BIGLIB COMPILATION INFO --")
#endif

// Memory
#include "Modules/Memory/Memory.h"

// Code
#include "Modules/VMs/Code.h"

// Data
#include "Modules/Data/Bitwise.h"
#include "Modules/Data/Random.h"
#include "Modules/Data/Math.h"

// DataIntegrity
#include "Modules/Data/DataIntegrity/CRCs.h"
#include "Modules/Data/DataIntegrity/MD2-6.h"

// MultiThreading
#include "Modules/MultiThreading/ThreadSafeQueue/ThreadSafeQueue.h"
#include "Modules/MultiThreading/ThreadPool/ThreadPool.h"

// Types
#include "Modules/Types/Types.h"

// Machine Learning
#include "Modules/MachineLearning/NeuralNetwork.h"