#pragma once

#include "RaiLight\Model\BasicTypes.h"
#include "RaiLight\Crypto\SecureTypes.h"
#include "RaiLight\Model\PendingBlock.h"

#include <stack>

namespace rail
{
    enum class Direction
    {
        Send,
        Receive
    };

    struct Transaction
    {
        std::string otherAddress;
        Direction   direction;
        uint64_t    amount{ 0 };
    };

    struct Account
    {
        ~Account();
        Account() = default;
        Account(const SecureContainer<ByteArray32>& _privateKey,
                                 const ByteArray32& _publicKey,
                                           uint32_t _index,
                                        std::string _accountId);

        SecureContainer<ByteArray32>                privateKey;
        ByteArray32                                 publicKey;
        uint32_t                                    index{ 0 };
        std::string                                 accountId;
        ByteArray32                                 representative;
        uint64_t                                    balance{ 0 };
        uint64_t                                    pendingBalance{ 0 };
        std::stack<ByteArray32>                     latestBlocks;
        std::vector<blocks::PendingBlock>           pendingBlocks;
        std::vector<Transaction>                    previousTransactions;
        bool                                        accountOpen{ false };
        uint64_t                                    cachedWork{ 0 };
    };
}
