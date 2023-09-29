#pragma once
#include <chrono>
#include <fstream>
#include "Singleton.h"

#define SG_TIMER (Timer::Instance())
class Timer : public Singleton<Timer> {
public:

    void Reset() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double ElapsedMilliseconds() const {
        auto end_time = std::chrono::high_resolution_clock::now();

        return std::chrono::duration<double, std::milli>(end_time - start_time).count();

    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

#define SG_LOGMGR LogManager::Instance()
class LogManager : public Singleton<LogManager>
{
public:
	void Record(std::string title, std::string info)
    {
        // Open the file for writing
        std::string totalPath = __FILE__ "\\..\\" + title + ".txt";

        std::ofstream outFile(totalPath, std::ios::app);

        if (!outFile.is_open()) {
            return;
        }

        outFile << info;
        outFile << "\n";

        // Close the file
        outFile.close();
    }

private:
    
};

