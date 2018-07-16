#include "catch\catch.hpp"

#include "RaiLight\Controllers\ICore.h"
#include "RaiLight\Database\DatabaseKeys.h"

#include "RailUnitTests\fakes\FakeCoreController.h"
#include "RailUnitTests\fakes\FakeWorkLoop.h"
#include "RailUnitTests\fakes\FakeDb.h"
#include "RailUnitTests\TestUtils.h"

#include "RaiLight\Controllers\Bank.h"
#include "RaiLight\Crypto\SecretsStore.h"
#include "RaiLight\Crypto\CryptoUtils.h"
#include "RaiLight\Crypto\PasswordHasher.h"
#include "RaiLight\Utilities\Conversions.h"


using namespace rail;
using namespace rail::control;

TEST_CASE("NoSeedSet", "[BankTests]")
{
    FakeCoreController fCoreController;

    Bank testBank(&fCoreController);

    testBank.init();

    fCoreController.workLoop->runAllTasks();
    
    REQUIRE(fCoreController.getSecretsStore()->isSeedSet());
    auto seed = fCoreController.getSecretsStore()->getSeed();

    REQUIRE(seed);
    REQUIRE(!(*seed).empty());

    auto vAddresses = testBank.getAllAddresses();
    REQUIRE(vAddresses.size() == 1);
}

TEST_CASE("SeedSetInMemory", "[BankTests]")
{
    FakeCoreController fCoreController(true);

    Bank testBank(&fCoreController);

    testBank.init();

    fCoreController.workLoop->runAllTasks();

    REQUIRE(fCoreController.getSecretsStore()->isSeedSet());
    auto seed = fCoreController.getSecretsStore()->getSeed();
    
    REQUIRE(seed);
    REQUIRE(!(*seed).empty());

    REQUIRE(*seed == TestUtils::TEST_SEED_ONE_BYTES);

    auto vAddresses = testBank.getAllAddresses();
    REQUIRE(vAddresses.size() == 1);
}

TEST_CASE("SeedEncryptedInDatabase", "[BankTests]")
{
    FakeCoreController fCoreController(false);

    Bank testBank(&fCoreController);

    testBank.init();

    fCoreController.workLoop->runAllTasks();

    REQUIRE(fCoreController.getSecretsStore()->isSeedSet());
    auto seed = fCoreController.getSecretsStore()->getSeed();

    REQUIRE(seed);
    REQUIRE(!(*seed).empty());

    REQUIRE(*seed == TestUtils::TEST_SEED_ONE_BYTES);

    auto vAddresses = testBank.getAllAddresses();
    REQUIRE(vAddresses.size() == 1);
}
