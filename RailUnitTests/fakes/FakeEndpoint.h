#pragma once

#include "RaiLight\Endpoints\IEndpoint.h"
#include "RaiLight\Model\EndpointTypes.h"
#include "RaiLight\Model\StateBlock.h"

class FakeEndpoint : public rail::endpoint::IEndpoint
{
public:
    FakeEndpoint();
    ~FakeEndpoint();
    rail::endpoint::BlockCounts getLedgerBlockCountSync() override;
    rail::endpoint::AccountStatus getAccountStatusSync(const std::string& account) override;
    std::string getFrontiersSync(const std::string& account) override;
    bool arePendingBlocksSync(const std::string& address) override;
    void getPendingBlocks(const std::string& account, const uint32_t count) override;
    void getAccountHistory(const std::string& account, const uint32_t count) override;
    bool getAccountBalanceSync(const std::string& account) override;
    void startWebServer() override;
    std::string sendStateBlock(rail::blocks::StateBlock& sendBlock) override;
};