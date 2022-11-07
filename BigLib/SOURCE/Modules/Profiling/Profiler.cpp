#include "Profiler.h"

using namespace Profiler;



void TimeProfiler::StartProfiler() {
	QueryPerformanceFrequency(&this->ProfilerVar);
	this->PCFrequency = double(this->ProfilerVar.QuadPart) / this->Resolution;

	QueryPerformanceCounter(&this->ProfilerVar);
	this->CounterStart = this->ProfilerVar.QuadPart;
}

double TimeProfiler::EndProfiler() {
	QueryPerformanceCounter(&this->ProfilerVar);
	return double(this->ProfilerVar.QuadPart - this->CounterStart) / this->PCFrequency;
}




