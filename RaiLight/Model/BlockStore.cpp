#include "Model\BlockStore.h"

#include "Model\BaseBlock.h"

namespace rail
{
    namespace blocks
    {
        BlockStore::~BlockStore() = default;

        BlockStore::BlockStore()
        {
            
        }

        bool BlockStore::addBlock(const Block & addBlock)
        {
            bool succeeded{ false };

            if (blocks.top().getBlockHash() != addBlock.getBlockHash())
            {
                blocks.push(addBlock);
            }

            return succeeded;
        }

        bool BlockStore::addPendingBlock(const ByteArray32 & addPendingBlock)
        {
            bool succeeded{ false };

            if (pendingBlocks.top() != addPendingBlock)
            {
                pendingBlocks.push(addPendingBlock);
            }

            return succeeded;
        }

        Block BlockStore::getLastBlock()
        {
            return blocks.top();
        }
    }
}
