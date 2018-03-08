#include "Model\SendBlock.h"

#include "Utilities\Conversions.h"
#include "Crypto\CryptoUtils.h"

#include <blake2\blake2.h>

namespace rail
{
    namespace blocks
    {
        Send::Send(const ByteArray32 & _previousBlock, const ByteArray32 & _destination,
            const ByteArray16 & _amountToSend, const ByteArray32 & _privateKey,
            const ByteArray32 & _publicKey) :
            Block(_previousBlock, _privateKey, _publicKey, {}),
            destinationAccount(_destination),
            sendAmount(_amountToSend)
        {

        }

        std::string Send::getDestinationAddress() const
        {
            return CryptoUtils::encodePublicKeyToAddress(destinationAccount);
        }

        std::string Send::getAmountToSend() const
        {
            return Conversions::encodeToHexStr(sendAmount);
        }

        ByteArray32 Send::getBlockHash() const
        {
            ByteArray32 output;

            blake2b_state hash;
            blake2b_init(&hash, 32);

            blake2b_update(&hash, getPreviousBlockBytes().data(), getPreviousBlockBytes().size());
            blake2b_update(&hash, destinationAccount.data(), destinationAccount.size());
            blake2b_update(&hash, sendAmount.data(), sendAmount.size());

            blake2b_final(&hash, output.data(), 32);

            return output;
        }
    }
}
