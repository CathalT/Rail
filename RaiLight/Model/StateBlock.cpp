#include "RaiLight\Model\StateBlock.h"

#include "RaiLight\Utilities\Conversions.h"
#include "RaiLight\Crypto\CryptoUtils.h"
#include "RaiLight\Crypto\ProofOfWork.h"

#include <blake2\blake2.h>

namespace rail
{
    namespace blocks
    {
        StateBlock::StateBlock(const ByteArray32 & _previousBlock,
            const ByteArray32 & _link,
            const ByteArray32 & _representative,
            const ByteArray16 & _balance,
            const SecureContainer<ByteArray32> & _privateKey,
            const ByteArray32 & _publicKey,
            const uint64_t _work) :
            previousBlock(_previousBlock),
            link(_link),
            representative(_representative),
            balance(_balance),
            privateKey(_privateKey),
            publicKey(_publicKey),
            work(_work)
        {
            preamble = Conversions::intToByteArray<ByteArray32, uint64_t>(6);
        }

        ByteArray32 StateBlock::getBlockHash() const
        {
            ByteArray32 output;
            blake2b_state hash;
            blake2b_init(&hash, output.size());

            blake2b_update(&hash, preamble.data(), preamble.size());
            blake2b_update(&hash, publicKey.data(), publicKey.size());
            blake2b_update(&hash, previousBlock.data(), previousBlock.size());
            blake2b_update(&hash, representative.data(), representative.size());
            blake2b_update(&hash, balance.data(), balance.size());
            blake2b_update(&hash, link.data(), link.size());

            blake2b_final(&hash, output.data(), output.size());

            return output;
        }

        std::string StateBlock::getAddress() const 
        {
            return CryptoUtils::encodePublicKeyToAddress(publicKey);
        }

        std::string StateBlock::getPreviousBlock() const
        {
            return Conversions::encodeToHexStr(previousBlock);
        }

        /////
        //For Send
        std::string StateBlock::getDestinationAddress() const
        {
            return CryptoUtils::encodePublicKeyToAddress(link);
        }

        std::string StateBlock::getDestinationPublicKey() const
        {
            return Conversions::encodeToHexStr(link);
        }
        ////

        std::string StateBlock::getRepresentative() const
        {
            return CryptoUtils::encodePublicKeyToAddress(representative);
        }

        std::string StateBlock::getBalance() const
        {
            return Conversions::hexBytesToInt<uInt128_t>(balance).str();
        }

        std::string StateBlock::getSignature() const
        {
            auto copyPrvKey = privateKey;
            auto decryptedPrvKey = copyPrvKey.getContainer();
            const auto sig = CryptoUtils::signMessage(decryptedPrvKey, publicKey, getBlockHash());
            CryptoPP::SecureWipeArray(decryptedPrvKey.data(), decryptedPrvKey.size());
            const auto sigStr = Conversions::encodeToHexStr(sig);
            return sigStr;
        }

        std::string StateBlock::getWork() const
        {
            std::string workStr;

            if (work == 0)
            {
                workStr = Conversions::uint64ToHexStr(Pow::generateWork(previousBlock));
            }
            else
            {
                workStr = Conversions::uint64ToHexStr(work);
            }

            return workStr;
        }
    }
}

