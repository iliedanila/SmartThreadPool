#include "../inc/Job.h"

#include <vector>

Job::Job()
:
    id(0)		
{}

Job::Job(
    std::future<void> aFuture,
    std::vector<std::size_t> aRunAfterIDs,
    const std::size_t anID)
:
    future(std::move(aFuture)),
    runAfterIDs(std::move(aRunAfterIDs)),
    id(anID)
{}

Job::Job(Job&& other)
:
    future(std::move(other.future)),
    runAfterIDs(std::move(other.runAfterIDs)),
    id(std::move(other.id))
{}

Job& Job::operator= (Job&& other)
{
    future = std::move(other.future);
    runAfterIDs = std::move(other.runAfterIDs);
    id = std::move(other.id);
    return *this;
}

std::size_t Job::ID() const
{
    return id;
}

void Job::run()
{
    future.get();
}

void Job::addWaitingJobID(std::size_t jobID)
{
    waitingJobIDs.push_back(jobID);
}

std::vector<std::size_t> Job::getWaitingJobID() const
{
    return waitingJobIDs;
}

void Job::increaseFinishedIDsCount()
{
    ++finishedIDsCount;
}

std::size_t Job::getFinishedIDsCount() const
{
    return finishedIDsCount;
}
