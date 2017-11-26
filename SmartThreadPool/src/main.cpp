#include <iostream>
#include "../inc/ThreadPool.h"

using namespace std::chrono_literals;

int main()
{
    // Jobs will run like this:
    //      0  1  2
    //      \  |  /
    //         3
    //      /  |  \
    //      4  5  6

    std::mutex coutMutex;

    ThreadPool threadPool;

    auto func = [&coutMutex](int value)
    {
        std::unique_lock<std::mutex> lock(coutMutex);
        std::cout << "My received value is: " << value << '\n';
    };

    auto id0 = threadPool.addJob(func, 0);
    auto id1 = threadPool.addJob(func, 1);
    auto id2 = threadPool.addJob(func, 2);

    std::vector<std::size_t> waitListForId3 = { id0, id1, id2 };
    auto id3 = threadPool.addJob(waitListForId3, func, 3);

    std::vector<std::size_t> waitListForId456 = { id3 };
    auto id4 = threadPool.addJob(waitListForId456, func, 4);
    auto id5 = threadPool.addJob(waitListForId456, func, 5);
    auto id6 = threadPool.addJob(waitListForId456, func, 6);

    threadPool.waitAll();
    std::cout << "That's all, folks!\nPress any key...";
    std::cin.get();
}
