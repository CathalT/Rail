#include "Model\StateBlock.h"

namespace rail
{
    namespace blocks
    {
        State::State(const ByteArray32 & _previousBlock,
            const ByteArray32 & _link,
            const ByteArray32 & _representative,
            const ByteArray16 & _balance,
            const SecureContainer<ByteArray32> & _privateKey,
            const ByteArray32 & _publicKey) : Block(_previousBlock, _privateKey, _publicKey, {_link,_representative})
        {
            _balance;
        }
    }
}

