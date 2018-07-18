#pragma once

#include "RaiLight\Crypto\SecureTypes.h"
#include "RaiLight\Model\BasicTypes.h"

#include <vector>
#include <string>

namespace rail
{
    struct Transaction;
    namespace blocks
    {
        class PendingBlock;
    }

    namespace control
    {
        class IBank
        {
        public:
            
            virtual void init() = 0;
            virtual std::vector<std::string> getAllAddresses() = 0;
            virtual ByteArray32 getPublicKeyForAddress(const std::string& address) = 0;
            virtual SecureContainer<ByteArray32> getPrivateKeyForAddress(const std::string& address) = 0;
            virtual bool sendToAccount(const std::string& fromAddress, const std::string& toAddress, const std::string& amount) = 0;
            virtual bool changeRepresentative(const std::string& currentAccount, const std::string& representativeId) = 0;
            virtual void updateAccountBalances(const std::string& address, const std::string& balance, const std::string& pending) = 0;
            virtual void updatePendingBlocks(const std::string& address, const std::vector<blocks::PendingBlock>& pendingBlocks) = 0;
            virtual void updateTransactionHistory(const std::string& address, const std::vector<Transaction>& transactions) = 0;
            virtual std::vector<Transaction> getTransactionHistory(const std::string & address) = 0;

            virtual uint64_t getAccountBalance(const std::string& address) = 0;
            virtual uint64_t getAccountPendingBalance(const std::string& address) = 0;
            virtual uint64_t getCachedWorkForAddress(const std::string& address) = 0;

            virtual bool isRetrievingAccounts() = 0;
            virtual void proccessCallbackBlocks(const std::string& address, const blocks::PendingBlock& pending) = 0;

            virtual ~IBank() = default;
        };
    }
}
