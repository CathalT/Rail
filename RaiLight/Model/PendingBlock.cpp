#include "Model\PendingBlock.h"

#include "Utilities\Conversions.h"

namespace rail
{
    namespace blocks
    {
        PendingBlock::PendingBlock(const std::string& _blockHash, const std::string& _balance)
        {
            if (auto h = Conversions::decodeHexFromString(_blockHash))
            {
                blockHash = h.value();
            }

            try
            {
                balance = std::stoull(_balance);
            }
            catch (std::exception e)
            {
                balance = 0;
            }
        }

        ByteArray32 PendingBlock::getBlockHash() const
        {
            return blockHash;
        }

        uint64_t PendingBlock::getBalance() const
        {
            return balance;
        }
    }
}