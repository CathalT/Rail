#pragma once

#include "RaiLight\Utilities\IWorkLoop.h"

#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>


namespace rail
{
    class WorkLoop : public IWorkLoop
    {
    public:
        WorkLoop();
        ~WorkLoop();
        void queue(std::function<void()> func) override;
        void stop() override;
  
    private:
        void loop();

    private:
        std::thread worker;
        std::mutex queueMutex;
        std::condition_variable condition;
        std::queue<std::function<void()>> workQueue;
        std::atomic_bool finished;
    };
}
