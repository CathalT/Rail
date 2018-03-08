#pragma once

#include "Model\IBlockStore.h"
#include "Model\BaseBlock.h"

#include "Model\BasicTypes.h"

#include <stack>

namespace rail
{
    namespace blocks
    {
        class Block; 
        class BlockStore : public IBlockStore
        {
        public:
            BlockStore();
            virtual ~BlockStore();

            virtual bool addBlock(const Block& addBlock) override;
            virtual bool addPendingBlock(const ByteArray32& addPendingBlock) override;
            virtual Block getLastBlock() override;


        private:
            std::stack<Block> blocks;
            std::stack<ByteArray32> pendingBlocks;
        };
    }
}
