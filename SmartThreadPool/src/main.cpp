#include <iostream>
#include "../inc/ThreadPool.h"

using namespace std::chrono_literals;

int main()
{
    std::mutex coutMutex;

    ThreadPool threadPool;

    threadPool.addJob([&coutMutex](auto value1, auto value2)
    {
        std::unique_lock<std::mutex> lock(coutMutex);
        std::cout << value1 << ' ' << value2 << '\n'; 
    }, 7, 7);

    auto func = [&coutMutex](auto value1, auto value2)
    {
        std::unique_lock<std::mutex> lock(coutMutex);
        std::cout << value1 << ' ' << value2 << '\n';
    };

    threadPool.addJob(func, 77, 777);

    auto add = [](auto value1, auto value2) -> decltype(value1 + value2)
    {
        return value1 + value2;
    };

    int sum;
    auto funcAdd = [&sum, add](auto value1, auto value2)
    {
        sum = add(value1, value2);
    };
    threadPool.addJob(funcAdd, 7, 770);

    std::this_thread::sleep_for(2s);
    std::cout << sum << '\n';
    std::this_thread::sleep_for(2s);
}