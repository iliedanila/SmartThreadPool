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

void ThreadPool::waitAll()
{
    bool finished;
    
    do
    {
        finished = addedJobsCount == finishedJobsCount;
    } while (!finished);
}

std::size_t ThreadPool::addJob(std::shared_ptr<Job> aJob)
{
    ++addedJobsCount;

    std::unique_lock<std::mutex> lock(jobsMutex);

    jobs.insert(std::make_pair(aJob->ID(), aJob));

    for (auto jobID : aJob->getRunAfterIDs())
    {
        jobs[jobID]->addWaitingJobID(aJob->ID());

        if (finishedJobs.find(jobID) != finishedJobs.end())
        {
            aJob->increaseFinishedIDsCount();
        }
    }

    if (aJob->isReadyToRun())
    {
        availableJobs.push(aJob);
    }

    return aJob->ID();
}

void ThreadPool::processJobFinished(std::size_t jobID)
{
    ++finishedJobsCount;

    std::unique_lock<std::mutex> lock(jobsMutex);
    
    auto job = jobs[jobID];
    finishedJobs.insert(jobID);

    for (auto waitingJobID : job->getWaitingJobIDs())
    {
        jobs[waitingJobID]->increaseFinishedIDsCount();
        
        if (jobs[waitingJobID]->isReadyToRun())
        {
            availableJobs.push(jobs[waitingJobID]);
        }
    }
}
