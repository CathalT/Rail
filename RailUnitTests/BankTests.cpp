#include "catch\catch.hpp"

#include "RaiLight\Controllers\ICore.h"
#include "RailUnitTests\fakes\FakeCoreController.h"

#include "RaiLight\Controllers\Bank.h"

using namespace rail;
using namespace rail::control;

TEST_CASE("Send to account", "[Bank]")
{
    auto fCoreController = std::make_unique<FakeCoreController>();
    auto rawPtr = fCoreController.get();
    Bank testBank(rawPtr);
}