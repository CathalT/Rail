#include "Controllers\Bank.h"

#include "Controllers\ICore.h"

#include "Database\RailDb.h"
#include "Database\DatabaseKeys.h"

#include "Endpoints\IEndpoint.h"

#include "Model\EndpointTypes.h"
#include "Model\BasicTypes.h"
#include "Model\StateBlock.h"
#include "Model\PendingBlock.h"
#include "Model\Account.h"
#include "Model\Marshaller.h"

#include "Utilities\Conversions.h"
#include "Utilities\WorkLoop.h"

#include "Crypto\CryptoUtils.h"
#include "Crypto\SecretsStore.h"

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
                amountToSend = std::stoull(amount);
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

                    ByteArray32 representative;
                    ByteArray32 fromAddressPublicKey;
                    uint64_t cachedWork{ 0 };

                    std::string lastHash;
                    {
                        std::shared_lock<std::shared_mutex> lock(accountsMutex);
                        const auto itr = accounts.find(fromAddress);
                        if (itr != accounts.end())
                        {
                            representative = itr->second->representative;
                            fromAddressPublicKey = itr->second->publicKey;
                            cachedWork = itr->second->cachedWork;

                            if (!itr->second->latestBlocks.empty())
                            {
                                previousBlock = itr->second->latestBlocks.top();
                                foundPreviousBlock = true;
                            }
                        }
                    }

                    if (foundPreviousBlock)
                    {
                        if (auto destinationPublicKey = CryptoUtils::decodeAccountIdToPublicKey(toAddress))
                        {
                            const auto newBalanceBytes = Conversions::uIntToByteArray<ByteArray16>(newBalance);

                            blocks::StateBlock sendBlock(previousBlock, destinationPublicKey.value(), representative,newBalanceBytes, getPrivateKeyForAddress(fromAddress), fromAddressPublicKey, cachedWork);
                            const auto nextBlock = coreController->getEndpoint()->sendStateBlock(sendBlock);
                            if (!nextBlock.empty())
                            {
                                storeLatestBlock(fromAddress, nextBlock);
                            }  
                        }
                        else
                        {
                            QMessageLogger().warning() << "Tried to send to invalid account Id";
                        }
                    }
                    else
                    {
                        lastHash = coreController->getEndpoint()->getFrontiersSync(fromAddress);
                        if (auto prvBlock = Conversions::decodeHexFromString(lastHash))
                        {
                            if (auto destinationPublicKey = CryptoUtils::decodeAccountIdToPublicKey(toAddress))
                            {
                                const auto newBalanceBytes = Conversions::uIntToByteArray<ByteArray16>(newBalance);

                                blocks::StateBlock sendBlock(prvBlock.value(), destinationPublicKey.value(), representative, newBalanceBytes, getPrivateKeyForAddress(fromAddress), fromAddressPublicKey, 0);
                                const auto nextBlock = coreController->getEndpoint()->sendStateBlock(sendBlock);
                                if (!nextBlock.empty())
                                {
                                    storeLatestBlock(fromAddress, nextBlock);
                                }
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
            const auto lasthash = coreController->getEndpoint()->getFrontiersSync(currentAccount);
            if (auto previousBlock = Conversions::decodeHexFromString(lasthash))
            {
                if (auto publicKey = CryptoUtils::decodeAccountIdToPublicKey(representativeId))
                {
                    const auto currentBalance = Conversions::uIntToByteArray<ByteArray16>(getAccountBalance(currentAccount));
                    blocks::StateBlock changeBlock(previousBlock.value(), { std::byte(0) }, publicKey.value(), currentBalance, getPrivateKeyForAddress(currentAccount), getPublicKeyForAddress(currentAccount), getCachedWorkForAddress(currentAccount));

                    coreController->getEndpoint()->sendStateBlock(changeBlock);
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

        uint64_t Bank::getAccountBalance(const std::string& address)
        {
            uint64_t balance{ 0 };

            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                balance = itr->second->balance;
            }

            return balance;
        }

        uint64_t Bank::getAccountPendingBalance(const std::string& address)
        {
            uint64_t pendingBalance{ 0 };

            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                pendingBalance = itr->second->pendingBalance;
            }

            return pendingBalance;
        }

        uint64_t Bank::getCachedWorkForAddress(const std::string& address)
        {
            uint64_t cachedWork{ 0 };

            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                cachedWork = itr->second->cachedWork;
            }

            return cachedWork;
        }

        void Bank::init()
        {
            coreController->getWorkLoop()->queue([this]()
            {
                auto isSeedSet = coreController->getSecretsStore()->isSeedSet();
                if (!isSeedSet)
                {
                    if (auto dbSeed = coreController->getDatabase()->getDynamicValue<std::vector<std::byte>>(key::bytes::SEED))
                    {
                        auto seedIv = coreController->getDatabase()->getValue<ByteArray16>(key::bytes::SEED_IV);
                        auto decryptedSeed = CryptoUtils::AESDecryptByteArray32(*dbSeed, coreController->getSecretsStore()->getPasswordKey(), *seedIv);

                        if (const auto accIdx = coreController->getDatabase()->getValue<uint32_t>(key::uint::ACCOUNT_INDEX))
                        {
                            for (uint32_t i = 0; i < accIdx; ++i)
                            {
                                addAccount(generateNewAccountFromSeed(decryptedSeed));
                            }
                        }
                        else
                        {
                            addAccount(generateNewAccountFromSeed(decryptedSeed));
                        }

                        coreController->getSecretsStore()->setSeed(decryptedSeed);

                        syncCurrentAccounts();
                    }
                    else
                    {
                        auto generatedSeed = CryptoUtils::getRandom32ByteBlock();

                        auto seedCopy = generatedSeed;
                        SecureContainer<ByteArray32> secureSeed(seedCopy, coreController->getSecretsStore()->getPasswordKey());

                        coreController->getSecretsStore()->setSeed(generatedSeed);

                        coreController->getDatabase()->storeValue(key::bytes::SEED, secureSeed.getEncryptedData(), true);
                        coreController->getDatabase()->storeValue(key::bytes::SEED_IV, secureSeed.getIv(), true);

                        addAccount(generateNewAccountFromSeed(generatedSeed));

                        finishRetrievingAccounts();
                    }
                }
                else
                {
                    syncAccountsFromSeed();
                }
            });
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

        SecureContainer<ByteArray32> Bank::getPrivateKeyForAddress(const std::string & address)
        {
            SecureContainer<ByteArray32> privateKey;

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
            auto newPrivateKey = CryptoUtils::generateKeyFromSeedAndIndex(seed, index);

            const auto publicKey = CryptoUtils::generatePublicKeyFromPrivateKey(newPrivateKey);
            const auto address = CryptoUtils::encodePublicKeyToAddress(publicKey);

            const auto securePrivateKey = SecureContainer<ByteArray32>(newPrivateKey, coreController->getSecretsStore()->getPasswordKey());

            if (index > 0)
            {
                coreController->getDatabase()->storeValue(key::uint::ACCOUNT_INDEX, index);
            }

            ++nextSeedIndex;
            return std::make_unique<Account>(securePrivateKey, publicKey, index, address);
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

        bool Bank::addPendingBlocksToAccount(const std::string & address, const std::vector<blocks::PendingBlock>& pendingBlocks)
        {
            bool succeeded{ false };
            std::lock_guard<std::shared_mutex> lock(accountsMutex);
            const auto itr = accounts.find(address);
            if (itr != accounts.end())
            {
                for (const auto b : pendingBlocks)
                {
                    if (!itr->second->latestBlocks.empty() && b.getBlockHash() == itr->second->latestBlocks.top()) continue;
                    itr->second->pendingBlocks.push_back(b);
                    succeeded = true;
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

        void Bank::updatePendingBlocks(const std::string& address, const std::vector<blocks::PendingBlock>& pendingBlocks)
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

        void Bank::proccessCallbackBlocks(const std::string& address, const std::string& /*incomingHash*/ )
        {
            //TODO
            updatePendingBlocks(address, { /*incomingHash*/ });
        }

        //TODO: Move out/refactor
        void Bank::syncAccountsFromSeed()
        {
            auto seed = coreController->getSecretsStore()->getSeed();

            if (!seed) return;

            while (retrievingAccounts)
            {
                auto nextAccount = generateNewAccountFromSeed(*seed);

                if (verifyAccount(nextAccount.get()))
                {
                    auto accountId = nextAccount->accountId;
                    addAccount(std::move(nextAccount));
                    coreController->getEndpoint()->getAccountBalance(accountId);
                    coreController->getEndpoint()->getAccountHistory(accountId, 100);
                    coreController->getEndpoint()->getPendingBlocks(accountId, 100);
                }
                else
                {
                    //last account doesn't exist
                    --nextSeedIndex;

                    finishRetrievingAccounts();
                }
            }

            CryptoPP::SecureWipeArray(seed->data(), seed->size());
        }

        bool Bank::verifyAccount(Account* account)
        {
            const auto status = coreController->getEndpoint()->getAccountStatusSync(account->accountId);

            if (status.isValid)
            {
                if (const auto block = Conversions::decodeHexFromString(status.frontier))
                {
                    account->latestBlocks.push(block.value());
                    coreController->getDatabase()->storeValue(key::bytes::LATEST_BLOCK, account->index, block.value());
                }

                if (auto repPubKey = CryptoUtils::decodeAccountIdToPublicKey(status.representative))
                {
                    account->representative = repPubKey.value();
                }
                
                account->accountOpen = status.isValid;
                coreController->getDatabase()->storeValue(key::bools::ACCOUNT_OPEN, account->index, status.isValid, true);
            }

            return account->accountOpen;
        }

        void Bank::syncCurrentAccounts()
        {
            std::shared_lock<std::shared_mutex> lock(accountsMutex);
            for (const auto& a : accounts)
            {
                const auto& accountPtr = a.second.get();
                const auto& accountId = accountPtr->accountId;

                if (const auto isOpen = coreController->getDatabase()->getValue<bool>(key::bools::ACCOUNT_OPEN, a.second->index))
                {
                    a.second->accountOpen = isOpen.value();
                }
                else
                {
                    verifyAccount(accountPtr);
                }

                coreController->getEndpoint()->getAccountBalance(accountId);
                coreController->getEndpoint()->getAccountHistory(accountId, 100);
                coreController->getEndpoint()->getPendingBlocks(accountId, 100);
            }

            finishRetrievingAccounts();
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
                            const auto newBalance = account->balance + pending.getBalance();
                            const auto newBalanceBytes = Conversions::uIntToByteArray<ByteArray16>(newBalance);

                            blocks::StateBlock receiveBlock(lastBlock, pending.getBlockHash(), account->representative, newBalanceBytes, account->privateKey, account->publicKey, account->cachedWork);
                            nextBlockHash = coreController->getEndpoint()->sendStateBlock(receiveBlock);
                            if (!nextBlockHash.empty())
                            {
                                account->pendingBlocks.pop_back();
                            }
                        }
                        else
                        {
                            const auto lastHash = coreController->getEndpoint()->getFrontiersSync(address);

                            if (!lastHash.empty())
                            {
                                if (const auto lastBlock = Conversions::decodeHexFromString(lastHash))
                                {
                                    const auto newBalance = account->balance + pending.getBalance();
                                    const auto newBalanceBytes = Conversions::uIntToByteArray<ByteArray16>(newBalance);

                                    blocks::StateBlock receiveBlock(lastBlock.value(), pending.getBlockHash(), account->representative, newBalanceBytes, account->privateKey, account->publicKey, account->cachedWork);
                                    nextBlockHash = coreController->getEndpoint()->sendStateBlock(receiveBlock);
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
                        const auto balanceBytes = Conversions::uIntToByteArray<ByteArray16>(pending.getBalance());

                        blocks::StateBlock openBlock({ std::byte(0) }, pending.getBlockHash(), account->representative, balanceBytes, account->privateKey, account->publicKey, account->cachedWork);
                        nextBlockHash = coreController->getEndpoint()->sendStateBlock(openBlock);
                        if (!nextBlockHash.empty())
                        {
                            account->accountOpen = true;
                            account->pendingBlocks.pop_back();
                        }
                        else
                        {
                            QMessageLogger().warning() << "Failed to open account.";
                        }
                    }
                }
            }

            return nextBlockHash;
        }
    }
}