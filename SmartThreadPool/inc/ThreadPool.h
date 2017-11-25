#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <thread>
#include <vector>
#include <atomic>
#include <future>
#include <queue>

#include "Job.h"

class ThreadPool
{
public:
    explicit ThreadPool();
    ~ThreadPool();

    template <typename Func, typename... Args>
    size_t addJob(Func&& func, Args&&... args);

    template <typename Func, typename... Args>
    size_t addJob(std::vector<size_t> runAfterIDs, Func&& func, Args&&... args);

private:
    friend class Worker;
    std::vector<std::thread> threads;
    
    std::queue<Job> jobs;
    std::vector<std::size_t> finishedJobs;
    std::mutex jobsMutex;

    std::atomic<bool> stop;
};

template <typename Func, typename... Args>
size_t ThreadPool::addJob(Func&& func, Args&&... args)
{
    return addJob(std::vector<std::size_t>(), std::forward<Func>(func), std::forward<Args>(args)...);
}

template <typename Func, typename... Args>
size_t ThreadPool::addJob(const std::vector<size_t> runAfterIDs, Func&& func, Args&&... args)
{
    std::unique_lock<std::mutex> lock(jobsMutex);
    jobs.emplace(Job(std::async(std::launch::deferred, func, std::forward<Args>(args)...), runAfterIDs, jobs.size()));
    return jobs.size() - 1;
}

#endif
