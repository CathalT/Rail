#pragma once

#include "Model\BasicTypes.h"

namespace rail
{
    namespace blocks
    {
        
        class Block;
        class IBlockStore
        {
        public:

            virtual bool addBlock(const Block& addBlock) = 0;
            virtual bool addPendingBlock(const ByteArray32& addBlock) = 0;
            virtual Block getLastBlock() = 0;

            virtual ~IBlockStore() = default;
        };
    }
}
