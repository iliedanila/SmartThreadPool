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
        std::shared_ptr<Job> job;

        {
            std::unique_lock<std::mutex> lock(threadPool.jobsMutex);

            if (threadPool.availableJobs.size())
            {
                job = threadPool.availableJobs.front();
                threadPool.availableJobs.pop();
            }
        }

        if (job)
        {
            job->run();
            threadPool.processJobFinished(job->ID());
        }
    }
}
