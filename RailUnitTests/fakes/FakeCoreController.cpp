#include "RailUnitTests\fakes\FakeCoreController.h"
#include "RailUnitTests\fakes\FakeDb.h"
#include "RailUnitTests\fakes\FakeWorkLoop.h"
#include "RailUnitTests\fakes\FakeEndpoint.h"
#include "RailUnitTests\TestUtils.h"

#include "RaiLight\Database\IDbWrapper.h"
#include "RaiLight\Database\DatabaseKeys.h"
#include "RaiLight\Model\Marshaller.h"
#include "RaiLight\Model\BasicTypes.h"
#include "RaiLight\Crypto\SecretsStore.h"
#include "RaiLight\Crypto\CryptoUtils.h"
#include "RaiLight\Controllers\IBank.h"
#include "RaiLight\Controllers\Bank.h"
#include "RaiLight\Endpoints\IEndpoint.h"
#include "RaiLight\Utilities\IWorkLoop.h"

void FakeCoreController::setupMembers()
{
    workLoop = std::make_unique<FakeWorkLoop>();
    marshaller = std::make_unique<rail::Marshaller>(this);
    endpoint = std::make_unique<FakeEndpoint>();
    database = std::make_unique<FakeDb>();
    bank = std::make_unique<rail::control::Bank>(this);

    auto generatedKey = rail::CryptoUtils::getRandom32ByteBlock();
    secretsStore = std::make_unique<rail::SecretsStore>(this, generatedKey, "");
}

FakeCoreController::FakeCoreController()
{
    setupMembers();
    secretsStore->setPassword(TestUtils::TEST_PASSWORD);
}

FakeCoreController::FakeCoreController(bool setSeedInMemory)
{
    setupMembers();

    if (setSeedInMemory)
    {
        secretsStore->setSeed(TestUtils::TEST_SEED_ONE);
    }
    else
    {
        database->storeByteArray32(rail::key::bytes::KEY_SALT, TestUtils::TEST_PASS_KEY_SALT, false);
        database->storeByteArray16(rail::key::bytes::SEED_IV, TestUtils::TEST_SEED_ONE_IV, false);
        database->storeVectorOfBytes(rail::key::bytes::SEED, TestUtils::ENCRYPTED_SEED_ONE, false);

        secretsStore->setPassword(TestUtils::TEST_PASSWORD);
    }
}


FakeCoreController::~FakeCoreController() = default;

rail::endpoint::IEndpoint* FakeCoreController::getEndpoint()
{
    return endpoint.get();
}

rail::control::IBank* FakeCoreController::getBank()
{
    return bank.get();
}

rail::Marshaller* FakeCoreController::getMarshaller()
{
    return marshaller.get();
}

rail::IWorkLoop* FakeCoreController::getWorkLoop()
{
    return workLoop.get();
}

rail::IDbWrapper * FakeCoreController::getDatabase()
{
    return database.get();
}

rail::SecretsStore * FakeCoreController::getSecretsStore()
{
    return secretsStore.get();
}
