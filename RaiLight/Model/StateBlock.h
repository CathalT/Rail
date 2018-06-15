#pragma once

#include "Model\BaseBlock.h"
#include "Model\BasicTypes.h"
#include "Crypto\SecureTypes.h"

namespace rail
{
    namespace blocks
    {
        class State : public Block
        {
        public:
            State(const ByteArray32& _previousBlock, const ByteArray32& _link, const ByteArray32& _representative,
                const ByteArray16& _balance, const SecureContainer<ByteArray32>& _privateKey,
                const ByteArray32& _publicKey);
        };
    }
}