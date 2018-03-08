#pragma once

#include "Model\BaseBlock.h"
#include "Model\BasicTypes.h"

namespace rail
{
    namespace blocks
    {
        class Open : public Block
        {
        public:
            Open(const ByteArray32& _privateKey, const ByteArray32& _publicKey, const ByteArray32& _sourceBlock, const ByteArray32& _representative);

            std::string getRepresentative() const;
            std::string getSourceBlock() const;
        private:
            ByteArray32 sourceBlock;
            ByteArray32 representative;
        };
    }
}
