/*!****************************************************************************
 * file  : FramerateController.h
 * brief : 
 * this class is use to controll the framerate, From Professor,
 * I have not use it, but it is simple to use
 * just add it to the game loop in main and get the dt
 * author: Martin Chow
 * Course: CS550
 * date  : Oct 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include <Windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>
#include <chrono>

#pragma comment(lib,"pdh.lib")

class FramerateController
{
public:
    static constexpr double DEFAULT_FIXED_TIME_STEP = 1.0 / 240.0;
    ~FramerateController() {};

    void SetTargetFramerate(unsigned int framerate);
    void SetFrameAtLevelStart(const int time);
    void FrameStart();
    void FrameEnd();
    void MarkTimeAtFrame(int frameNumber);
    float GetTimeAtFrameLastMarkedFrame();
    double GetFrameTime() const;
    float GetFPS() const;
    int GetFrameNumber() const;
    double GetElapsedTimeSinceLastMarkedFrame();
    void ResetFrameNummber();
    double GetRenderTime() const;
    double GetFrameAtLevelStart() const;
    double GetTime() const;
    double GetSeconds() const;
    //debug only
    void startPerfCollection()
    {
        if (PdhOpenQuery(NULL, NULL, &hQuery) != ERROR_SUCCESS) {
            throw std::runtime_error("Failed to open PDH query");
        }

        if (PdhAddCounter(hQuery, TEXT("\\Processor(_Total)\\% Processor Time"), NULL, &hCounter) != ERROR_SUCCESS) {
            throw std::runtime_error("Failed to add PDH counter");
        }

        if (PdhCollectQueryData(hQuery) != ERROR_SUCCESS) {
            throw std::runtime_error("Failed to collect PDH query data");
        }
    };
    double GetCpuUsage() {
        if (count != 10) {
            count++;
            return lastcpu;
        }
        else
        {
            count = 0;
        }
        PDH_FMT_COUNTERVALUE counterVal;
        PDH_STATUS status;
        status = PdhCollectQueryData(hQuery);
        if (status != ERROR_SUCCESS) {
            std::cerr << "Failed to collect PDH query data: " << status << std::endl;
            return lastcpu;
        }

        status = PdhGetFormattedCounterValue(hCounter, PDH_FMT_DOUBLE, NULL, &counterVal);
        if (status != ERROR_SUCCESS) {
            std::cerr << "Failed to get formatted counter value: " << status << std::endl;
            return lastcpu;
        }
        lastcpu = counterVal.doubleValue;
        return counterVal.doubleValue;
        /*if (PdhCollectQueryData(hQuery) != ERROR_SUCCESS) {
            throw std::runtime_error("Failed to collect PDH query data");
        }

        PDH_FMT_COUNTERVALUE counterVal;
        if (PdhGetFormattedCounterValue(hCounter, PDH_FMT_DOUBLE, NULL, &counterVal) != ERROR_SUCCESS) {
            throw std::runtime_error("Failed to get formatted counter value");
        }

        return counterVal.doubleValue;*/
    }

    double GetMemoryUsage() {
        MEMORYSTATUSEX memStatus;
        memStatus.dwLength = sizeof(memStatus);
        if (GlobalMemoryStatusEx(&memStatus)) {
            double totalMemory = static_cast<double>(memStatus.ullTotalPhys);
            double usedMemory = totalMemory - static_cast<double>(memStatus.ullAvailPhys);
            return (usedMemory / totalMemory) * 100.0;  // Return memory usage as a percentage
        }
        else {
            throw std::runtime_error("Failed to get memory status");
        }
    }
    void StopPerfCollection()
    {
        PdhCloseQuery(hQuery);
    }

    float DeltaTime = 0;

    bool ShouldUpdatePhysics();
    void ConsumePhysicsTime();
    double GetAccumulatorAlpha() const { return accumulator / DEFAULT_FIXED_TIME_STEP; }

    void initialize();
    void shutdown();

    static FramerateController& instance()
    {
        static FramerateController instance;
        return instance;
    }

private:

    FramerateController();
    FramerateController(const FramerateController&) = delete;
    FramerateController& operator=(const FramerateController&) = delete;
    FramerateController(FramerateController&&) = delete;
    FramerateController& operator=(FramerateController&&) = delete;


    int frameCount = 0;
    double frameAtLevelStart = 0;
    double timeAtFrameStart = 0;
    double timeAtFrameEnd = 0;
    double timeAtSpecificFrame = 0;
    double lastFrameTime = 0;
    double lastRenderTime = 0;
    int targetFrameTime = 0;
    double lastcpu = 0;
    double lastmem = 0;


    int count = 0;
    double accumulator = 0;
    PDH_HQUERY hQuery;
    PDH_HCOUNTER hCounter;

};

