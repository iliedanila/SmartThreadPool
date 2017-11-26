#include "../inc/Job.h"

#include <vector>

Job::Job()
:
    finishedIDsCount(0),
    id(0)
{
}

Job::Job(
    std::future<void> aFuture,
    std::vector<std::size_t> aRunAfterIDs,
    const std::size_t anID)
:
    future(std::move(aFuture)),
    runAfterIDs(std::move(aRunAfterIDs)), 
    finishedIDsCount(0),
    id(anID)
{
}

Job::Job(Job&& other)
    :
    future(std::move(other.future)),
    runAfterIDs(std::move(other.runAfterIDs)),
    finishedIDsCount(std::move(other.finishedIDsCount)),
    id(std::move(other.id))
{
}

Job& Job::operator= (Job&& other)
{
    future = std::move(other.future);
    runAfterIDs = std::move(other.runAfterIDs);
    id = std::move(other.id);
    finishedIDsCount = std::move(other.finishedIDsCount);

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

std::vector<std::size_t> Job::getWaitingJobIDs() const
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

std::vector<std::size_t> Job::getRunAfterIDs() const
{
    return runAfterIDs;
}

bool Job::isReadyToRun() const
{
    return runAfterIDs.size() == finishedIDsCount;
}
