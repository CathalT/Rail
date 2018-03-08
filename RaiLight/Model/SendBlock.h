#pragma once

#include "Model\BaseBlock.h"
#include "Model\BasicTypes.h"

namespace rail
{
    namespace blocks
    {
        class Send : public Block
        {
        public:
            Send(const ByteArray32& _previousBlock, const ByteArray32& _destination,
                const ByteArray16& _amountToSend, const ByteArray32& _privateKey,
                const ByteArray32& _publicKey);

            std::string getDestinationAddress() const;
            std::string getAmountToSend() const;

            virtual ByteArray32 getBlockHash() const override;

        private:
            ByteArray32 destinationAccount;
            ByteArray16 sendAmount;
        };
    }
}