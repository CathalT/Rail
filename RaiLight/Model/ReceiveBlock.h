#pragma once

#include "Model\BaseBlock.h"
#include "Model\BasicTypes.h"
#include "Crypto\SecureTypes.h"

namespace rail
{
    namespace blocks
    {
        class Receive : public Block
        {
        public:
            Receive(const ByteArray32& _previousBlock, const ByteArray32& _sourceBlock, const SecureContainer<ByteArray32>& _privateKey, const ByteArray32& _publicKey);
            
            std::string Receive::getSourceBlock() const;

        private:
            ByteArray32 sourceBlock;
        };
    }
}