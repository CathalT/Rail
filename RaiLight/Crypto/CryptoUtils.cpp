#include "RaiLight\Crypto\CryptoUtils.h"

#include "RaiLight\Crypto\AccountEncoding.h"
#include "RaiLight\Crypto\RandomData.h"

#include "RaiLight\Utilities\Conversions.h"
#include "RaiLight\Utilities\MemoryFan.h"

#include <ed25519-donna\ed25519.h>
#include <blake2\blake2.h>

#include <cryptopp\cryptlib.h>
#include <cryptopp\filters.h>
#include <cryptopp\files.h>
#include <cryptopp\modes.h>

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

        ByteArray32 AESDecryptByteArray32(const std::vector<std::byte> & encryptedData, MemoryFan* obfuscatedKey, const ByteArray16& iv)
        {
            CryptoPP::ByteQueue secureData;
            CryptoPP::ByteQueue recover;
            secureData.Put(reinterpret_cast<const CryptoPP::byte*>(encryptedData.data()), encryptedData.size());
 
            auto key = obfuscatedKey->getValue();

            CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec;
            dec.SetKeyWithIV(reinterpret_cast<CryptoPP::byte*>(key.data()), key.size(), reinterpret_cast<const CryptoPP::byte*>(iv.data()), iv.size());

            CryptoPP::SecureWipeArray(key.data(), key.size());

            CryptoPP::StreamTransformationFilter f2(dec, new CryptoPP::Redirector(recover));
            secureData.CopyTo(f2);
            f2.MessageEnd();

            ByteArray32 outputBytes;
            recover.Get(reinterpret_cast<CryptoPP::byte*>(&outputBytes[0]), outputBytes.size());
            recover.Clear();

            return outputBytes;
        }

        std::optional<ByteArray32> decodeAccountIdToPublicKey(const std::string & accountId)
        {
            return decodeAccountIdToPublicKeyImpl(accountId);
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
