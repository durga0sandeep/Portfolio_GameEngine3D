/*!****************************************************************************
 * file  : JobSystem.h
 * brief : this is the job system template file
 * when construct the worker work
 * when destructor call it kill all the worker
 * 
 * author: Martin Chow
 * Course: CS550
 * date  : September 2025
 * Copyright © 2025 DigiPen Institute of Technology
 /*****************************************************************************/
#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
#include <atomic>

class MyJobSystem {
public:
    using Job = std::function<void()>;

    MyJobSystem(size_t workerCount = 1);
    ~MyJobSystem();

    void Submit(Job job);

private:
    std::vector<std::thread> m_workers;
    std::queue<Job> m_jobs;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_stop;
    std::atomic<int> m_pendingJobs = 0;
    std::condition_variable m_doneCV;
    std::mutex m_doneMutex;
};


