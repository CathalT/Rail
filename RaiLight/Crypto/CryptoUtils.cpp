#include "Crypto\CryptoUtils.h"

#include "Utilities\Conversions.h"
#include "Crypto\AccountEncoding.h"
#include "Crypto\RandomData.h"

#include <ed25519-donna\ed25519.h>
#include <blake2\blake2.h>

#include <QMessageLogger>
#include <QDebug>

namespace rail
{
    namespace CryptoUtils
    {
        ByteArray32 getRandom32ByteBlock()
        {
            ByteArray32 bytes;

            CryptoUtils::fillWithRandomData(bytes);

            return bytes;
        }

        ByteArray32 generateKeyFromSeedAndIndex(const ByteArray32& seed, uint32_t index)
        {
            ByteArray32 key{};

            if (seed.size() == 32)
            {
                blake2b_state hash;
                blake2b_init(&hash, 32);
                blake2b_update(&hash, seed.data(), seed.size());
                blake2b_update(&hash, reinterpret_cast<uint8_t *> (&index), sizeof(unsigned int));
                blake2b_final(&hash, key.data(), 32);

                QMessageLogger().debug() << "New key generated.";
            }
            else
            {
                QMessageLogger().warning() << "Seed is not 32 bytes long.";
            }

            return key;
        }

        ByteArray32 generatePublicKeyFromPrivateKey(const ByteArray32& privateKey)
        {
            ByteArray32 publicKey;

            ed25519_publickey(reinterpret_cast<const unsigned char*>(privateKey.data()), reinterpret_cast<unsigned char*>(publicKey.data()));

            return publicKey;
        }

        std::string encodePublicKeyToAddress(const ByteArray32& publicKey)
        {
            return encodePublicKeyToAccountIdImpl(publicKey);
        }

        bool decodeAccountIdToPublicKey(const std::string & accountId, ByteArray32& publicKey)
        {
            return decodeAccountIdToPublicKeyImpl(accountId , publicKey);
        }

        ByteArray32 blockHash(const std::vector<ByteArray32>& vStr)
        {
            ByteArray32 output;
            blake2b_state hash;
            blake2b_init(&hash, 32);
            for (const auto str : vStr)
            {
                blake2b_update(&hash, str.data(), str.size());
            }
            blake2b_final(&hash, output.data(), 32);
            return output;
        }

        ByteArray64 signMessage(const ByteArray32& privateKey, const ByteArray32& publicKey, const ByteArray32& hashedMessage)
        {
            ByteArray64 result;
            ed25519_sign(reinterpret_cast<const unsigned char*>(hashedMessage.data()),
                         sizeof(hashedMessage),
                         reinterpret_cast<const unsigned char*>(privateKey.data()),
                         reinterpret_cast<const unsigned char*>(publicKey.data()),
                         reinterpret_cast<unsigned char*>(result.data()));
            return result;
        }
    }
}
