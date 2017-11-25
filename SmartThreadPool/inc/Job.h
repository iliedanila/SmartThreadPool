#ifndef _JOB_H_
#define _JOB_H_
#include <future>

class Job
{
public:
    Job()
    :
        id(0)
    {}

    explicit Job(
        std::future<void> aFuture,
        std::vector<std::size_t> aRunAfterIDs,
        const std::size_t anID)
    :
        future(std::move(aFuture)),
        runAfterIDs(std::move(aRunAfterIDs)), 
        id(anID)
    {}

    Job(const Job& other) = delete;
    Job& operator = (const Job& other) = delete;

    Job(Job&& other)
    :
        future(std::move(other.future)),
        runAfterIDs(std::move(other.runAfterIDs)),
        id(std::move(other.id))
    {}

    Job& operator= (Job&& other)
    {
        future = std::move(other.future);
        runAfterIDs = std::move(other.runAfterIDs);
        id = std::move(other.id);
        return *this;
    }

    std::size_t ID() const { return id; }
    void run() { future.get(); }

private:
    std::future<void> future;
    std::vector<std::size_t> runAfterIDs;
    std::size_t id;
};

#endif

