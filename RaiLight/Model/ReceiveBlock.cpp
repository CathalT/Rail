#include "Model\ReceiveBlock.h"

#include "Utilities\Conversions.h"

namespace rail
{
    namespace blocks
    {
        Receive::Receive(const ByteArray32 & _previousBlock, const ByteArray32 & _sourceBlock, const ByteArray32 & _privateKey, const ByteArray32 & _publicKey) :
            Block(_previousBlock, _privateKey, _publicKey, { _previousBlock , _sourceBlock }),
            sourceBlock(_sourceBlock)
        {

        }

        std::string Receive::getSourceBlock() const
        {
            return Conversions::encodeToHexStr(sourceBlock);
        }
    }
}
