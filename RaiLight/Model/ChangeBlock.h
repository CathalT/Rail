#pragma once

#include "Model\BaseBlock.h"
#include "Model\BasicTypes.h"

namespace rail
{
    namespace blocks
    {
        class Change : public Block
        {
        public:
            Change(const ByteArray32& _previousBlock, const ByteArray32& _representative, const ByteArray32& _privateKey, const ByteArray32& _publicKey);

            std::string getRepresentativeAddress() const;

        private:
            ByteArray32 representative;
        };
    }
}