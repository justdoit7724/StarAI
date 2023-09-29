#pragma once

#include <chrono>

class StopWatch {
public:
    StopWatch();

    void reset();

    float elapsed() const;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};
