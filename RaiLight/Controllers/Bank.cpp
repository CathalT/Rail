#include "Controllers\Bank.h"

#include "Controllers\ICore.h"

#include "Database\RailDb.h"
#include "Database\DatabaseKeys.h"

#include "Endpoints\IEndpoint.h"

#include "Model\EndpointTypes.h"
#include "Model\BasicTypes.h"
#include "Model\BlockStore.h"
#include "Model\OpenBlock.h"
#include "Model\ReceiveBlock.h"
#include "Model\SendBlock.h"
#include "Model\ChangeBlock.h"
#include "Model\Account.h"
#include "Model\Marshaller.h"

#include "Utilities\Conversions.h"
#include "Utilities\WorkLoop.h"

#include "Crypto\CryptoUtils.h"

#include <QMessageLogger>
#include <QDebug>

const static std::string TEST_SEED = "∞È|^í¡áù{˜¯§[òU……ç:MnÆa≈¯h";
const static std::string HEX_SEED = "F0B0E97C5E92C1879D7B02F7F8A4115B05985512C9C98D3AF04D6EAE61C5F868";
const static std::string PUBLIC_HEX = "DAE2141B817005B4C45EBC70B95C031E204AECC4837B1745871925F4654057EB";

namespace rail
{
    namespace control
    {
        Bank::~Bank() = default;

        Bank::Bank(ICore* _coreController) :
            coreController(_coreController),
            nextSeedIndex(0),
            retrievingAccounts(true),
            processingPendingTransactions(false)
        {
        }

        bool Bank::sendToAccount(const std::string& fromAddress, const std::string& toAddress, const std::string& amount)
        {
            //TODO:Refactor entire function.
            uint32_t balance{ 0 };
            bool succeeded{ false };
            {
                //TODO:Refactor into account class. Getters.
                std::shared_lock<std::shared_mutex> lock(accountsMutex);
                const auto itr = accounts.find(fromAddress);
                if (itr != accounts.end())
                {
                    balance = itr->second->balance;
                }
            }

            uint32_t amountToSend{ 0 };

            try
            {
                amountToSend = std::stoul(amount);
            }
            catch (std::exception e)
            {
                amountToSend = 0;
            }

            if (balance > 0  && balance >= amountToSend)
            {
                auto newBalance = balance - amountToSend;
                coreController->getWorkLoop()->queue([this, fromAddress, toAddress, newBalance]()
                {
                    ByteArray32 previousBlock;
                    bool foundPreviousBlock = false;

                    std::string lastHash;
                    {
                        std::shared_lock<std::shared_mutex> lock(accountsMutex);
                        const auto itr = accounts.find(fromAddress);
                        if (itr != accounts.end())
                        {
                            if (!itr->second->latestBlocks.empty())
                            {
                                previousBlock = itr->second->latestBlocks.top();
                                foundPreviousBlock = true;
                            }
                        }
                    }

                    if (foundPreviousBlock)
                    {
                        ByteArray32 publicKey;
                        if (CryptoUtils::decodeAccountIdToPublicKey(toAddress, publicKey))
                        {
                            const auto newBalanceBytes = Conversions::uIntToByteArray<ByteArray16>(newBalance);

                            blocks::Send sendBlock(previousBlock, publicKey, newBalanceBytes, getPrivateKeyForAddress(fromAddress), getPublicKeyForAddress(fromAddress));
                            const auto nextBlock = coreController->getEndpoint()->send(sendBlock);
                            storeLatestBlock(fromAddress, nextBlock);

                        }
                        else
                        {
                            QMessageLogger().warning() << "Tried to send to invalid account Id";
                        }
                    }
                    else
                    {
                        lastHash = coreController->getEndpoint()->getFrontiers(fromAddress);
                        if (auto prvBlock = Conversions::decodeHexFromString(lastHash))
                        {
                            ByteArray32 publicKey;
                            if (CryptoUtils::decodeAccountIdToPublicKey(toAddress, publicKey))
                            {
                                const auto newBalanceBytes = Conversions::uIntToByteArray<ByteArray16>(newBalance);

                                blocks::Send sendBlock(prvBlock.value(), publicKey, newBalanceBytes, getPrivateKeyForAddress(fromAddress), getPublicKeyForAddress(fromAddress));
                                const auto nextBlock = coreController->getEndpoint()->send(sendBlock);
                                storeLatestBlock(fromAddress, nextBlock);
                            }
                            else
                            {
                                QMessageLogger().warning() << "Tried to send to invalid account Id";
                            }
                        }
                    }
                    coreController->getEndpoint()->getAccountBalance(fromAddress);
                });

                succeeded = true;
            }

            return succeeded;
        }

        //TODO: implement fully, expose to UI.
        bool Bank::changeRepresentative(const std::string & currentAccount, const std::string & representativeId)
        {
            bool succeeded{ false };

            //TODO: Store latest hash, grab from memory.    
            const auto lasthash = coreController->getEndpoint()->getFrontiers(currentAccount);
            if (auto previousBlock = Conversions::decodeHexFromString(lasthash))
            {
                ByteArray32 publicKey;
                if (CryptoUtils::decodeAccountIdToPublicKey(representativeId, publicKey))
                {
                    blocks::Change changeBlock(previousBlock.value(), publicKey, getPrivateKeyForAddress(currentAccount), getPublicKeyForAddress(currentAccount));
                    coreController->getEndpoint()->change(changeBlock);
                    succeeded = true;
                }
            }

            return succeeded;
        }

        void Bank::updateAccountBalances(const std::string & address, const std::string & balance, const std::string & pending)
        {
            std::lock_guard<std::shared_mutex> lock(accountsMutex);

            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                if (!balance.empty())
                {
                    itr->second->balance = std::stoul(balance);
                }
                if (!pending.empty())
                {
                    itr->second->pendingBalance = std::stoul(pending);
                }
            }
        }

        uint32_t Bank::getAccountBalance(const std::string& address)
        {
            uint32_t balance(0);

            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                balance = itr->second->balance;
            }

            return balance;
        }

        uint32_t Bank::getAccountPendingBalance(const std::string& address)
        {
            uint32_t pendingBalance(0);

            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                pendingBalance = itr->second->pendingBalance;
            }

            return pendingBalance;
        }

        void Bank::initWithSeed(const std::string & _seed)
        {
            if (_seed.empty())
            {
                if (const auto dbSeed = coreController->getDatabase()->getValue<ByteArray32>(key::bytes::SEED))
                {
                    if (const auto accIdx = coreController->getDatabase()->getValue<uint32_t>(key::uint::ACCOUNT_INDEX))
                    {
                        for (uint32_t i = 0; i < accIdx; ++i)
                        {
                            addAccount(generateNewAccountFromSeed(dbSeed.value()));
                        }
                    }
                    else
                    {
                        addAccount(generateNewAccountFromSeed(dbSeed.value()));
                    }

                    syncCurrentAccounts();
                }
                else
                {
                    const auto generatedSeed = CryptoUtils::getRandom32ByteBlock();
                    coreController->getDatabase()->storeValue(key::bytes::SEED, generatedSeed, true);

                    //const auto debugSeed = Conversions::encodeToHexStr(generatedSeed); debugSeed;
                    addAccount(generateNewAccountFromSeed(generatedSeed));

                    finishRetrievingAccounts();
                }
            }
            else
            {
                if (const auto decodedSeed = Conversions::decodeHexFromString(_seed))
                {
                    syncAccountsFromSeed(decodedSeed.value());
                }
                else
                {
                    QMessageLogger().critical() << "Failed to decode seed.";
                }
            }
        }

        std::vector<std::string> Bank::getAllAddresses()
        {
            std::shared_lock<std::shared_mutex> lock(accountsMutex);

            std::vector<std::string> aIds;

            for (const auto& a : accounts)
            {
                aIds.push_back(a.second->accountId);
            }

            return aIds;
        }

        ByteArray32 Bank::getPublicKeyForAddress(const std::string & address)
        {
            ByteArray32 publicKey{};

            std::shared_lock<std::shared_mutex> lock(accountsMutex);

            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                publicKey = itr->second->publicKey;
            }

            return publicKey;
        }

        ByteArray32 Bank::getPrivateKeyForAddress(const std::string & address)
        {
            ByteArray32 privateKey{};

            std::shared_lock<std::shared_mutex> lock(accountsMutex);

            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                privateKey = itr->second->privateKey;
            }

            return privateKey;
        }

        //TODO: move to new class account factory
        std::unique_ptr<Account> Bank::generateNewAccountFromSeed(const ByteArray32& seed)
        {
            auto index(nextSeedIndex.load());
            const auto newPrivateKey = CryptoUtils::generateKeyFromSeedAndIndex(seed, index);
            const auto publicKey = CryptoUtils::generatePublicKeyFromPrivateKey(newPrivateKey);
            const auto address = CryptoUtils::encodePublicKeyToAddress(publicKey);
            
            if (index > 0)
            {
                coreController->getDatabase()->storeValue(key::uint::ACCOUNT_INDEX, index);
            }

            ++nextSeedIndex;
            return std::make_unique<Account>(newPrivateKey, publicKey, index, address);
        }

        void Bank::addAccount(std::unique_ptr<Account> account)
        {
            std::lock_guard<std::shared_mutex> lock(accountsMutex);
            accounts[account->accountId] = std::move(account);
        }

        bool Bank::isRetrievingAccounts()
        {
            return retrievingAccounts;
        }

        void Bank::finishRetrievingAccounts()
        {
            retrievingAccounts = false;
            coreController->getMarshaller()->notifyAccountsFinishedSyncing();
            coreController->getEndpoint()->startWebServer();
        }

        bool Bank::addPendingBlocksToAccount(const std::string & address, const std::vector<std::string>& pendingBlocks)
        {
            bool succeeded{ false };
            std::lock_guard<std::shared_mutex> lock(accountsMutex);
            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                for (const auto b : pendingBlocks)
                {
                    if (const auto pendingBlock = Conversions::decodeHexFromString(b))
                    {
                        if (!itr->second->latestBlocks.empty() && pendingBlock == itr->second->latestBlocks.top()) continue;
                        itr->second->pendingBlocks.push_back(pendingBlock.value());
                        succeeded = true;
                    }
                    else
                    {
                        succeeded = false;
                        break;
                    }
                }
            }
            else
            {
                succeeded = false;
            }
            return succeeded;
        }

        void Bank::processPendingBlocks(const std::string & address)
        {
            std::string nextBlockHash;
            //revisit this...
            while (1)
            {
                nextBlockHash = processReceiveOrOpen(address);
                if (nextBlockHash.empty()) break;
                storeLatestBlock(address, nextBlockHash);   
            }
            coreController->getEndpoint()->getAccountBalance(address);
        }

        void Bank::storeLatestBlock(const std::string& address, const std::string& latestBlockHash)
        {
            if (const auto nextBlock = Conversions::decodeHexFromString(latestBlockHash))
            {
                std::lock_guard<std::shared_mutex> lock(accountsMutex);
                const auto itr = accounts.find(address);
                if (itr != accounts.end())
                {
                    auto& lastestBlocks = itr->second->latestBlocks;
                    if (lastestBlocks.size() > 0)
                    {
                        itr->second->latestBlocks.pop();
                        coreController->getDatabase()->deleteValue(key::bytes::LATEST_BLOCK, itr->second->index);
                    }
                    itr->second->latestBlocks.push(nextBlock.value());
                    coreController->getDatabase()->storeValue(key::bytes::LATEST_BLOCK, itr->second->index, nextBlock.value());
                }
                else
                {
                    QMessageLogger().warning() << "Account not found: " << address.c_str();
                }
            }
            else
            {
                QMessageLogger().critical() << "Failed to decode latest hash.";
            }
        }

        void Bank::updatePendingBlocks(const std::string& address, const std::vector<std::string>& pendingBlocks)
        {
            if (addPendingBlocksToAccount(address, pendingBlocks))
            {
                processPendingBlocks(address);
            }
        }

        void Bank::updateTransactionHistory(const std::string & address, const std::vector<Transaction>& transactions)
        {
            std::lock_guard<std::shared_mutex> lock(accountsMutex);
            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                auto& previousTransactions = itr->second->previousTransactions;
                previousTransactions.insert(previousTransactions.end(), transactions.begin(), transactions.end());
            }
        }

        std::vector<rail::Transaction> Bank::getTransactionHistory(const std::string & address)
        {
            std::vector<Transaction> transactionHistory;
            std::shared_lock<std::shared_mutex> lock(accountsMutex);
            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                transactionHistory = itr->second->previousTransactions;
            }
            return transactionHistory;
        }

        void Bank::proccessCallbackBlocks(const std::string& address, const std::string& incomingHash )
        {
            updatePendingBlocks(address, { incomingHash });
        }

        //TODO: Move out/refactor
        void Bank::syncAccountsFromSeed(const ByteArray32 & seed)
        {
            coreController->getWorkLoop()->queue([this, seed]()
            {
                while (retrievingAccounts)
                {
                    auto nextAccount = generateNewAccountFromSeed(seed);
                    const auto areBlocksPending = coreController->getEndpoint()->arePendingBlocks(nextAccount->accountId);
                    const auto status = coreController->getEndpoint()->getAccountStatus(nextAccount->accountId);
                    if (status.isValid || areBlocksPending)
                    {
                        if (status.isValid)
                        {
                            const auto lastBlock = coreController->getEndpoint()->getFrontiers(nextAccount->accountId);
                            if (const auto block = Conversions::decodeHexFromString(lastBlock))
                            {
                                nextAccount->latestBlocks.push(block.value());
                                coreController->getDatabase()->storeValue(key::bytes::LATEST_BLOCK, nextAccount->index, block.value());
                            }
                        }

                        nextAccount->accountOpen = status.isValid;
                        coreController->getDatabase()->storeValue(key::bools::ACCOUNT_OPEN, nextAccount->index, status.isValid, true);

                        addAccount(std::move(nextAccount));
                        coreController->getEndpoint()->getAccountBalance(nextAccount->accountId);
                        coreController->getEndpoint()->getAccountHistory(nextAccount->accountId, 100);
                        coreController->getEndpoint()->getPendingBlocks(nextAccount->accountId, 100);
                    }
                    else
                    {
                        //last account doesn't exist
                        --nextSeedIndex;

                        finishRetrievingAccounts();
                    }
                }
            });
        }

        void Bank::syncCurrentAccounts()
        {
            std::shared_lock<std::shared_mutex> lock(accountsMutex);
            for (const auto& a : accounts)
            {
                coreController->getEndpoint()->getAccountBalance(a.second->accountId);
                coreController->getEndpoint()->getAccountHistory(a.second->accountId, 100);
                coreController->getEndpoint()->getPendingBlocks(a.second->accountId, 100);
                if (const auto isOpen = coreController->getDatabase()->getValue<bool>(key::bools::ACCOUNT_OPEN, a.second->index))
                {
                    a.second->accountOpen = isOpen.value();
                }
            }
        }

        //TODO: Refactor this mess as well.
        std::string Bank::processReceiveOrOpen(const std::string & address)
        {
            std::string nextBlockHash;
            std::shared_lock<std::shared_mutex> lock(accountsMutex);
            const auto itr = accounts.find(address);

            if (itr != accounts.end())
            {
                auto& account = itr->second;
                if (!account->pendingBlocks.empty())
                {
                    const auto pending = account->pendingBlocks.back();

                    if (account->accountOpen)
                    {
                        if (account->latestBlocks.size() > 0)
                        {
                            const auto lastBlock = account->latestBlocks.top();
                            blocks::Receive receiveBlock(lastBlock, pending, account->privateKey, account->publicKey);
                            nextBlockHash = coreController->getEndpoint()->receive(receiveBlock);
                            if (!nextBlockHash.empty())
                            {
                                account->pendingBlocks.pop_back();
                            }
                        }
                        else
                        {
                            const auto lastHash = coreController->getEndpoint()->getFrontiers(address);

                            if (!lastHash.empty())
                            {
                                if (const auto lastBlock = Conversions::decodeHexFromString(lastHash))
                                {
                                    blocks::Receive receiveBlock(lastBlock.value(), pending, account->privateKey, account->publicKey);
                                    nextBlockHash = coreController->getEndpoint()->receive(receiveBlock);
                                    if (!nextBlockHash.empty())
                                    {
                                        account->pendingBlocks.pop_back();
                                    }
                                }
                            }
                            else
                            {
                                QMessageLogger().debug() << "No previous block, could not process receive.";
                            }
                        }
                    }
                    else
                    {
                        blocks::Open openBlock(account->privateKey, account->publicKey, pending, account->publicKey);
                        nextBlockHash = coreController->getEndpoint()->open(openBlock);
                        if (!nextBlockHash.empty())
                        {
                            account->accountOpen = true;
                            account->pendingBlocks.pop_back();
                        }
                        else
                        {
                            QMessageLogger().warning() << "Failed to open account->";
                        }
                    }
                }
            }

            return nextBlockHash;
        }
    }
}