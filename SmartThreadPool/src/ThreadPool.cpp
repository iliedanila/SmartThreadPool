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

