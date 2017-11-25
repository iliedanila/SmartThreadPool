#include "../inc/Worker.h"
#include "../inc/ThreadPool.h"

using namespace std::chrono_literals;

Worker::Worker(ThreadPool& aThreadPool)
:
    threadPool(aThreadPool)
{}

void Worker::operator()() const
{
    while (!threadPool.stop)
    {
        Job job;
        bool hasJob = false;
        
        {
            std::unique_lock<std::mutex> lock(threadPool.jobsMutex);
            if (threadPool.jobs.size())
            {
                job = std::move(threadPool.jobs.front());
                hasJob = true;
                threadPool.jobs.pop();
            }
        }

        if (hasJob)
        {
            job.run();
            {
                std::unique_lock<std::mutex> lock(threadPool.jobsMutex);
                threadPool.finishedJobs.push_back(job.ID());
            }
        }
    }
}
