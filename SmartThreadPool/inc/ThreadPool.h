#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <thread>
#include <vector>
#include <atomic>
#include <future>
#include <queue>
#include <memory>
#include <unordered_map>

#include "Job.h"
#include <unordered_set>

class ThreadPool
{
public:
    explicit ThreadPool();
    ~ThreadPool();

    template <typename Func, typename... Args>
    size_t addJob(Func&& func, Args&&... args);

    template <typename Func, typename... Args>
    size_t addJob(std::vector<size_t> runAfterIDs, Func&& func, Args&&... args);

    void waitAll();

private:
    friend class Worker;
    std::size_t addJob(std::shared_ptr<Job> aJob);
    void processJobFinished(std::size_t jobID);
    
    std::vector<std::thread> threads;
    std::queue<std::shared_ptr<Job>> availableJobs;
    
    std::unordered_map<std::size_t, std::shared_ptr<Job>> jobs;
    std::unordered_set<std::size_t> finishedJobs;
    std::mutex jobsMutex;

    std::atomic<bool> stop;
    std::atomic<int> addedJobsCount;
    std::atomic<int> finishedJobsCount;
};

template <typename Func, typename... Args>
size_t ThreadPool::addJob(Func&& func, Args&&... args)
{
    return addJob(std::vector<std::size_t>(), std::forward<Func>(func), std::forward<Args>(args)...);
}

template <typename Func, typename... Args>
size_t ThreadPool::addJob(const std::vector<size_t> runAfterIDs, Func&& func, Args&&... args)
{
    auto currentID = jobs.size();
    auto job = std::make_shared<Job>(
        std::async(std::launch::deferred, func, std::forward<Args>(args)...),
        runAfterIDs,
        currentID);

    return addJob(job);
}

#endif
