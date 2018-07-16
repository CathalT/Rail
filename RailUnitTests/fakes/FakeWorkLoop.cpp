#include "RailUnitTests/fakes/FakeWorkLoop.h"

FakeWorkLoop::FakeWorkLoop() = default;
FakeWorkLoop::~FakeWorkLoop() = default;

void FakeWorkLoop::queue(std::function<void()> func)
{
    workQueue.push(func);
}

void FakeWorkLoop::stop()
{
    std::queue<std::function<void()>> emptyQueue;
    workQueue.swap(emptyQueue);
}

void FakeWorkLoop::runNextTask()
{
    auto task = workQueue.front();
    task();
    workQueue.pop();
}

void FakeWorkLoop::runAllTasks()
{
    for (size_t i = 0; i < workQueue.size(); ++i)
    {
        runNextTask();
    }
}