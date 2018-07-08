#include "Model\Marshaller.h"

#include "Controllers\ICore.h"
#include "Controllers\IBank.h"
#include "Model\BasicTypes.h"
#include "Model\Account.h"
#include "Model\PendingBlock.h"
#include "Utilities\Conversions.h"

#pragma warning( push )
#pragma warning( disable : 4266)
#include <cpprest\json.h>
#pragma warning( pop ) 


#define U(x) _XPLATSTR(x)

namespace rail
{
    Marshaller::~Marshaller() = default;

    Marshaller::Marshaller(control::ICore* _coreController) : coreController(_coreController)
    {

    }

    void Marshaller::parseBalances(const web::json::value& jsonBlob, const std::string& address)
    {
        std::string balance;
        std::string pendingBalance;

        if(jsonBlob.has_field(U("balance")))
        {
            balance = Conversions::toStdString(jsonBlob.at(U("balance")).as_string());
        }
        if (jsonBlob.has_field(U("pending")))
        {
            pendingBalance = Conversions::toStdString(jsonBlob.at(U("pending")).as_string());
        }

        coreController->getBank()->updateAccountBalances(address, balance, pendingBalance);
        updateUIBalances(address, balance, pendingBalance);
    }

    void Marshaller::parseCallbackResponse(const web::json::value& jsonBlob)
    {
        if (jsonBlob.has_field(U("block")))
        {
            auto block = jsonBlob.at(U("block")).as_string();
            const auto parsedBlock = web::json::value::parse(block);
            if (parsedBlock.has_field(U("type")))
            {
                if (parsedBlock.at(U("type")).as_string() == U("send"))
                {
                    if(parsedBlock.has_field(U("destination")))
                    { 
                        utility::string_t blockHash;
                        const auto destinationAccount = parsedBlock.at(U("destination")).as_string();
                        if (jsonBlob.has_field(U("hash")))
                        {
                            blockHash = jsonBlob.at(U("hash")).as_string();
                            coreController->getBank()->proccessCallbackBlocks(Conversions::toStdString(destinationAccount), Conversions::toStdString(blockHash));
                        }
                    }
                }
            }
        }
    }

    void Marshaller::parsePendingBlocks(const std::string& address, const web::json::object & pendingBlocks)
    {
        std::vector<rail::blocks::PendingBlock> blocks;
        blocks.reserve(pendingBlocks.size());

        for(const auto block : pendingBlocks)
        {
            const auto blockHash = Conversions::toStdString(block.first);

            if (block.second.has_field(U("amount")))
            {
                auto amount = block.second.at(U("amount")).to_string();
                amount.erase(amount.begin());
                amount.erase(amount.end()-1);
                blocks.emplace_back(blockHash, Conversions::toStdString(amount));
            }
        }

        coreController->getBank()->updatePendingBlocks(address, blocks);
    }

    void Marshaller::parseAccountHistory(const std::string & address, const web::json::array & history)
    {
        std::vector<Transaction> transactions;
        transactions.reserve(history.size());

        for (const auto b : history)
        {
            Transaction historyBlock;

            if(b.has_field(U("type")))
            { 
                if (b.at(U("type")).as_string() == U("receive"))
                {
                    historyBlock.direction = Direction::Receive;
                }
                else if (b.at(U("type")).as_string() == U("send"))
                {
                    historyBlock.direction = Direction::Send;
                }
            }
            if (b.has_field(U("account")))
            {
                historyBlock.otherAddress = Conversions::toStdString(b.at(U("account")).as_string());
            }
            if (b.has_field(U("amount")))
            {
                historyBlock.amount = std::stoull(b.at(U("amount")).as_string());
            }

            transactions.push_back(historyBlock);
        }

        coreController->getBank()->updateTransactionHistory(address, transactions);
        emit transactionsUpdated(QString::fromStdString(address));
    }

    void Marshaller::updateUIBalances(const std::string& address, const std::string& balance, const std::string& pendingBalance)
    {
        const auto qStrAccountId = QString::fromStdString(address);
        emit updateBalance(QString::fromStdString(balance), qStrAccountId);
        emit updatePendingBalance(QString::fromStdString(pendingBalance), qStrAccountId);
    }

    void Marshaller::notifyAccountsFinishedSyncing()
    {
        emit accountFinishedSyncing();
    }
}