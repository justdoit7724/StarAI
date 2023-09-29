#include "pch.h"
#include "StopWatch.h"

StopWatch::StopWatch()
	: start_time(std::chrono::high_resolution_clock::now()) {}

void StopWatch::reset()
{
    start_time = std::chrono::high_resolution_clock::now();
}

float StopWatch::elapsed() const
{
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    return (duration.count()/1000.0); 
}
