#include "RaiLight\Utilities\WorkLoop.h"

namespace rail
{
    WorkLoop::WorkLoop() : finished(false)
    {
        worker = std::thread(&WorkLoop::loop, this);
    }

    WorkLoop::~WorkLoop()
    {
        stop();
    }

    void WorkLoop::queue(std::function<void()> func)
    {
        std::thread([this, func]()
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            workQueue.push(func);
            condition.notify_all();
        }).detach();
    }

    void WorkLoop::stop()
    {
        finished = true;

        condition.notify_all();

        if (worker.joinable())
        {
            worker.join();
        }
    }

    void WorkLoop::loop()
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        while(!finished)
        {
            if(!workQueue.empty())
            {
                const auto copyFunc = workQueue.front();
                workQueue.pop();
                lock.unlock();
                copyFunc();
                lock.lock();
            }
            else
            {
                condition.wait(lock);
            }
        }
    }
}

