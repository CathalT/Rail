#include "RailUnitTests/fakes/FakeWorkLoop.h"

FakeWorkLoop::FakeWorkLoop() = default;
FakeWorkLoop::~FakeWorkLoop() = default;

void FakeWorkLoop::queue(std::function<void()> /*func*/)
{

}

void FakeWorkLoop::stop()
{

}
