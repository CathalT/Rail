#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>


namespace rail
{
    class WorkLoop
    {
    public:
        WorkLoop();
        ~WorkLoop();
        void queue(std::function<void()> func);
        void stop();
  
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
