#ifndef _WORKER_H_
#define _WORKER_H_

class ThreadPool;

class Worker
{
public:
    explicit Worker(ThreadPool& aThreadPool);

    void operator()() const;

private:
    ThreadPool& threadPool;
};

#endif
