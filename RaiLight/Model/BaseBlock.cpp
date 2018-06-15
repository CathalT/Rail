#include "Model\BaseBlock.h"

#include "Crypto\CryptoUtils.h"
#include "Utilities\Conversions.h"

#include <cryptopp\misc.h>
#include <blake2\blake2.h>

namespace rail
{
    namespace blocks
    {
        Block::Block(const ByteArray32& _prevBlock, const SecureContainer<ByteArray32>& _privateKey, const ByteArray32& _publicKey, const VectorOfBytes& _toBeHashed) :
            previousBlockHash(_prevBlock),
            privateKey(_privateKey),
            publicKey(_publicKey),
            toBeHashed(_toBeHashed)
        {
            
        }

        std::string Block::getAddress() const
        {
            return CryptoUtils::encodePublicKeyToAddress(publicKey);
        }

        /*std::string Block::getPrivateKey() const
        {
            return Conversions::encodeToHexStr(privateKey);
        }*/

        std::string Block::getPublicKey() const
        {
            return Conversions::encodeToHexStr(publicKey);
        }

        std::string Block::getPreviousBlock() const
        {
            return Conversions::encodeToHexStr(previousBlockHash);
        }

        ByteArray32 Block::getPreviousBlockBytes() const
        {
            return previousBlockHash;
        }

        ByteArray32 Block::getPublicKeyBytes() const
        {
            return publicKey;
        }

        ByteArray32 Block::getBlockHash() const
        {
            return CryptoUtils::blockHash(toBeHashed);
        }

        std::string Block::getSignature()
        {
            auto decryptedPrvKey = privateKey.getContainer();
            const auto sig = CryptoUtils::signMessage(decryptedPrvKey, publicKey, getBlockHash());
            CryptoPP::SecureWipeArray(decryptedPrvKey.data(), decryptedPrvKey.size());
            const auto sigStr = Conversions::encodeToHexStr(sig);
            return sigStr;
        }
    }
}

