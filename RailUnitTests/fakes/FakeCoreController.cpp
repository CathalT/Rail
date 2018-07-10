#include "RailUnitTests\fakes\FakeCoreController.h"
#include "RailUnitTests\fakes\FakeDb.h"
#include "RailUnitTests\fakes\FakeWorkLoop.h"
#include "RailUnitTests\fakes\FakeEndpoint.h"

#include "RaiLight\Database\IDbWrapper.h"
#include "RaiLight\Model\Marshaller.h"
#include "RaiLight\Crypto\SecretsStore.h"
#include "RaiLight\Controllers\IBank.h"
#include "RaiLight\Endpoints\IEndpoint.h"
#include "RaiLight\Utilities\IWorkLoop.h"

FakeCoreController::FakeCoreController()
{

}

FakeCoreController::~FakeCoreController() = default;

rail::endpoint::IEndpoint* FakeCoreController::getEndpoint()
{
    return nullptr;
}

rail::control::IBank* FakeCoreController::getBank()
{
    return nullptr;
}

rail::Marshaller* FakeCoreController::getMarshaller()
{
    return nullptr;
}

rail::IWorkLoop* FakeCoreController::getWorkLoop()
{
    return nullptr;
}

rail::IDbWrapper * FakeCoreController::getDatabase()
{
    return nullptr;
}

rail::SecretsStore * FakeCoreController::getSecretsStore()
{
    return nullptr;
}
