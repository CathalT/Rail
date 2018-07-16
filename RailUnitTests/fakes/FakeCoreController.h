#pragma once

#include "RaiLight\Controllers\ICore.h"

#include <memory>

namespace rail
{
    namespace endpoint
    {
        class IEndpoint;
    }
    
    namespace control
    {
        class IBank;
    }

    class Marshaller;
    class IWorkLoop;
    class IDbWrapper;
    class SecretsStore;
}

class FakeWorkLoop;

class FakeCoreController : public rail::control::ICore
{
public:
    FakeCoreController();
    FakeCoreController(bool setSeedInMemory);

    ~FakeCoreController();

    rail::endpoint::IEndpoint* getEndpoint() override;
    rail::control::IBank* getBank() override;
    rail::Marshaller* getMarshaller() override;
    rail::IWorkLoop* getWorkLoop() override;
    rail::IDbWrapper * getDatabase() override;
    rail::SecretsStore * getSecretsStore() override;

public:
    std::unique_ptr<FakeWorkLoop>               workLoop;
    std::unique_ptr<rail::control::IBank>       bank;
    std::unique_ptr<rail::Marshaller>           marshaller;
    std::unique_ptr<rail::endpoint::IEndpoint>  endpoint;
    std::unique_ptr<rail::IDbWrapper>           database;
    std::unique_ptr<rail::SecretsStore>         secretsStore;

private:
    void setupMembers();
};