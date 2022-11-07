/*
* Name: Profiler Library
* Desc: Profiling Library For Function Speed Measures
* Status: Done, More Features To Add
*/
#pragma once
#include "../../Includes.h"
#include "../../Configs.h"



namespace Profiler {
	

	class TimeProfiler {
	public:

		double Resolution = 1000000.0;
		FORCE_INLINE void StartProfiler();
		FORCE_INLINE double EndProfiler();

	private:
		LARGE_INTEGER ProfilerVar;
		INT64 CounterStart;
		double PCFrequency;
	};
}




