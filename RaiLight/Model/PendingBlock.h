#pragma once

#include "Model\BasicTypes.h"

namespace rail
{
    namespace blocks
    {
        class PendingBlock
        {
        public:
            PendingBlock(const std::string& _blockHash, const std::string& _balance);

            ByteArray32 getBlockHash() const;
            uint64_t getBalance() const;

        private:
            ByteArray32 blockHash;
            uint64_t balance;
        };
    }
}