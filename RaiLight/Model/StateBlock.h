#pragma once

#include "Model\BasicTypes.h"
#include "Crypto\SecureTypes.h"

namespace rail
{
    namespace blocks
    {
        //https://github.com/nanocurrency/raiblocks/wiki/Universal-Blocks-Specification
        class StateBlock
        {
        public:
            StateBlock(const ByteArray32& _previousBlock, const ByteArray32& _link, const ByteArray32& _representative,
                const ByteArray16& _balance, const SecureContainer<ByteArray32>& _privateKey,
                const ByteArray32& _publicKey, const uint64_t _work);

            ByteArray32 getBlockHash() const;

            std::string getAddress() const;
            std::string getPreviousBlock() const;
            std::string getDestinationAddress() const;
            std::string getDestinationPublicKey() const;
            std::string getRepresentative() const;
            std::string getBalance() const;
            std::string getSignature() const;
            std::string getWork() const;

        private:
            ByteArray32 previousBlock;
            ByteArray32 link;
            ByteArray32 representative;
            ByteArray16 balance;
            SecureContainer<ByteArray32> privateKey;
            ByteArray32 publicKey;
            uint64_t work;
            ByteArray32 preamble;
        };
    }
}