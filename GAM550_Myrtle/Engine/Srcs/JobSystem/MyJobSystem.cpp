#include "pch.h"
#include "JobSystem\MyJobSystem.h"

/**
 * $BRIEF
 * constructor of JobSystem
 * create workerCount number of worker thread
 * \param workerCount
 */
MyJobSystem::MyJobSystem(size_t workerCount) : m_stop(false)
{
    for (size_t i = 0; i < workerCount; ++i) {
        m_workers.emplace_back([this] {
            while (true) {
                Job job;
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_cv.wait(lock, [this] { return m_stop || !m_jobs.empty(); });

                    if (m_stop && m_jobs.empty())
                        return;

                    job = std::move(m_jobs.front());
                    m_jobs.pop();
                }
                job(); // Execute job
            }
            });
    }
}

/**
 * $BRIEF
 * submit a lamdas function to the job system
 * \param job
 */
void MyJobSystem::Submit(Job job)
{
    m_pendingJobs.fetch_add(1);
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_jobs.push([this, job] {
            job();
            if (m_pendingJobs.fetch_sub(1) == 1) {
                std::lock_guard<std::mutex> lock(m_doneMutex);
                m_doneCV.notify_all();
            }
            });
    }
    m_cv.notify_one();
}

/**
 * $BRIEF
 * destructor of JobSystem
 * kill all the worker thread
 */
MyJobSystem::~MyJobSystem()
{
   {
       std::unique_lock<std::mutex> lock(m_mutex);
       m_stop = true;
   }
   m_cv.notify_all();
   for (std::thread& worker : m_workers)
       worker.join();
}
