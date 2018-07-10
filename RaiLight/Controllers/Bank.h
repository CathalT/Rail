#pragma once

#include "RaiLight\Controllers\IBank.h"

#include "RaiLight\Crypto\SecureTypes.h"
#include "RaiLight\Model\BasicTypes.h"

#include <string>
#include <memory>
#include <atomic>
#include <map>
#include <shared_mutex>

namespace rail
{
    struct Account;
    struct Transaction;

    namespace endpoint
    {
        class IEndpoint;
    }

    namespace blocks
    {
        class IBlockStore;
    }

    namespace control
    {
        class ICore;
        class Bank : public IBank
        {
        public:
            Bank(ICore* _coreController);
            ~Bank();

            virtual void init() override;
            virtual std::vector<std::string> getAllAddresses() override;
            virtual ByteArray32 getPublicKeyForAddress(const std::string& address) override;
            virtual SecureContainer<ByteArray32> getPrivateKeyForAddress(const std::string& address) override;
            virtual bool sendToAccount(const std::string& fromAddress, const std::string& toAddress, const std::string& amount) override;
            virtual bool changeRepresentative(const std::string& currentAccount, const std::string& representativeId) override;
            virtual void updateAccountBalances(const std::string& address, const std::string& balance, const std::string& pending) override;
            virtual void updatePendingBlocks(const std::string& address, const std::vector<blocks::PendingBlock>& pendingBlocks) override;
            virtual void updateTransactionHistory(const std::string& address, const std::vector<Transaction>& transactions) override;
            virtual std::vector<Transaction> getTransactionHistory(const std::string & address) override;


            virtual uint64_t getAccountBalance(const std::string& address) override;
            virtual uint64_t getAccountPendingBalance(const std::string& address) override;
            virtual uint64_t getCachedWorkForAddress(const std::string& address) override;

            virtual bool isRetrievingAccounts() override;

            virtual void proccessCallbackBlocks(const std::string & address, const std::string & incomingHash) override;

        private:
            void finishRetrievingAccounts();
            bool addPendingBlocksToAccount(const std::string& address, const std::vector<blocks::PendingBlock>& pendingBlocks);
            void processPendingBlocks(const std::string& address);
            void storeLatestBlock(const std::string & address, const std::string & latestBlockHash);
            
            bool verifyAccount(Account* account);
            void syncAccountsFromSeed();
            void syncCurrentAccounts();

            std::string processReceiveOrOpen(const std::string & address);

        private:
            ICore*                                           coreController;

            std::atomic<uint32_t>                            nextSeedIndex;
            std::atomic<bool>                                retrievingAccounts;
            std::atomic<bool>                                processingPendingTransactions;

            std::shared_mutex                                accountsMutex;
            std::map<std::string, std::unique_ptr<Account> > accounts;

        private:
            std::unique_ptr<Account> generateNewAccountFromSeed(const ByteArray32& seed);
            void addAccount(std::unique_ptr<Account> account);
        };
    }
}
