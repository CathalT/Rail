#pragma once

#include <functional>

namespace rail
{
    class IWorkLoop
    {
    public:
        virtual void queue(std::function<void()> func) = 0;
        virtual void stop() = 0;
        virtual ~IWorkLoop() = default;
    };
}