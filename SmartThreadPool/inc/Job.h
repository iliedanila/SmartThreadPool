#ifndef _JOB_H_
#define _JOB_H_
#include <future>

class Job
{
public:
    explicit Job();

    explicit Job(
        std::future<void> aFuture,
        std::vector<std::size_t> aRunAfterIDs,
         const std::size_t anID);

    Job(const Job& other) = delete;
    Job& operator = (const Job& other) = delete;

    Job(Job&& other);
    Job& operator= (Job&& other);

    std::size_t ID() const;
    void run();
    
    void addWaitingJobID(std::size_t jobID);
    std::vector<std::size_t> getWaitingJobID() const;
    
    void increaseFinishedIDsCount();
    std::size_t getFinishedIDsCount() const;

private:
    std::future<void> future;
    
    std::vector<std::size_t> runAfterIDs;
    std::size_t finishedIDsCount;
    
    std::vector<std::size_t> waitingJobIDs;
    std::size_t id;
};

#endif

