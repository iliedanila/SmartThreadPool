#include "../inc/ThreadPool.h"
#include "../inc/Worker.h"

#include <thread>

ThreadPool::ThreadPool()
{
    for (unsigned int index = 0; index < std::thread::hardware_concurrency(); index++)
    {
        threads.emplace_back(std::thread(Worker(*this)));
    }
}

ThreadPool::~ThreadPool()
{
    stop = true;
    for (unsigned int index = 0; index < threads.size(); index++)
    {
        threads[index].join();
    }
}

void ThreadPool::processJobFinished(std::size_t jobID)
{
    std::unique_lock<std::mutex> lock(jobsMutex);
    
    auto job = jobs[jobID];
    for (auto waitingJobID : job->getWaitingJobID())
    {
        jobs[waitingJobID]->increaseFinishedIDsCount();
        
        if (jobs[waitingJobID]->getFinishedIDsCount() == 0)
        {
            availableJobs.push(jobs[waitingJobID]);
        }
    }
}
