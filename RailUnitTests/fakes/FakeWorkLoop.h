#pragma once

#include "RaiLight\Utilities\IWorkLoop.h"

class FakeWorkLoop : public rail::IWorkLoop
{
    FakeWorkLoop();

    void queue(std::function<void()> func) override;
    void stop() override;

    ~FakeWorkLoop();
};