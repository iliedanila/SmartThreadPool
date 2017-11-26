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
        // Get next job (if there is one).
        // Run the job.
        // Notify ThreadPool about the finished job.
    }
}
