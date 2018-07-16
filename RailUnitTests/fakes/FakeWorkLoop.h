#pragma once

#include "RaiLight\Utilities\IWorkLoop.h"

#include <queue>

class FakeWorkLoop : public rail::IWorkLoop
{
public:
    FakeWorkLoop();
    ~FakeWorkLoop();

    void queue(std::function<void()> func) override;
    void stop() override;

    void runNextTask();
    void runAllTasks();

private:

    std::queue<std::function<void()>> workQueue;
};