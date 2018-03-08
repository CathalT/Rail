#pragma once

#include "Model\BaseBlock.h"
#include "Model\BasicTypes.h"

#include <vector>

namespace rail
{
    using VectorOfBytes = std::vector<ByteArray32>;

    namespace blocks
    {
        class Block
        {
        public:
            Block(const ByteArray32& _prevBlock, const ByteArray32& _privateKey, const ByteArray32& _publicKey, const VectorOfBytes& _toBeHashed);
            virtual ~Block() = default;

            virtual std::string getAddress() const;
            virtual std::string getPrivateKey() const;
            virtual std::string getPublicKey() const;
            virtual std::string getPreviousBlock() const;
            virtual ByteArray32 getPreviousBlockBytes() const;
            virtual ByteArray32 getPublicKeyBytes() const;
            virtual ByteArray32 getBlockHash() const;
            virtual std::string getSignature() const;

        private:
            VectorOfBytes toBeHashed;
            ByteArray32   previousBlockHash;
            ByteArray32   privateKey;
            ByteArray32   publicKey;
            int64_t       work;

            std::string   signature;
        };
    }
}
