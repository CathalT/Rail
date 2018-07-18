#include "catch\catch.hpp"

#include "RaiLight\Controllers\ICore.h"
#include "RaiLight\Database\DatabaseKeys.h"

#include "RailUnitTests\fakes\FakeCoreController.h"
#include "RailUnitTests\fakes\FakeWorkLoop.h"
#include "RailUnitTests\fakes\FakeDb.h"
#include "RailUnitTests\TestUtils.h"

#include "RaiLight\Controllers\Bank.h"
#include "RaiLight\Crypto\SecretsStore.h"
#include "RaiLight\Model\Marshaller.h"
#include "RaiLight\Crypto\CryptoUtils.h"
#include "RaiLight\Crypto\PasswordHasher.h"
#include "RaiLight\Utilities\Conversions.h"


using namespace rail;
using namespace rail::control;

class TransactionTestFixture
{
public:
    TransactionTestFixture() : testBank(&fCore)
    {

    }

    FakeCoreController fCore;
    Bank testBank;
};

TEST_CASE("NoSeedSet", "[BankInitTests]")
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

TEST_CASE("SeedSetInMemory", "[BankInitTests]")
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

TEST_CASE("SendNano", "[BankTransactionTests]")
{
    FakeCoreController fCoreController;
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

TEST_CASE_METHOD(TransactionTestFixture, "ReceiveNano", "[BankTransactionTests]")
{
   // auto clientAddress = testBank.getAllAddresses().at(0);
    //fCore.marshaller->transactionsUpdated
}
