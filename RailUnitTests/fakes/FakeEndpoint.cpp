#include "RailUnitTests\fakes\FakeEndpoint.h"

FakeEndpoint::FakeEndpoint() = default;
FakeEndpoint::~FakeEndpoint() = default;

rail::endpoint::BlockCounts FakeEndpoint::getLedgerBlockCountSync()
{
    return rail::endpoint::BlockCounts();
}

rail::endpoint::AccountStatus FakeEndpoint::getAccountStatusSync(const std::string& /*account*/)
{
    return rail::endpoint::AccountStatus();
}

std::string FakeEndpoint::getFrontiersSync(const std::string& /*account*/)
{
    return "";
}

bool FakeEndpoint::arePendingBlocksSync(const std::string& /*address*/)
{
    return true;
}

void FakeEndpoint::getPendingBlocks(const std::string& /*account*/, const uint32_t /*count*/)
{

}

void FakeEndpoint::getAccountHistory(const std::string& /*account*/, const uint32_t /*count*/)
{

}

bool FakeEndpoint::getAccountBalanceSync(const std::string& /*account*/)
{

}

void FakeEndpoint::startWebServer()
{

}

std::string FakeEndpoint::sendStateBlock(rail::blocks::StateBlock& /*sendBlock*/)
{
    return "";
}
