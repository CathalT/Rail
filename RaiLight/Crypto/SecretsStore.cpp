#include "RaiLight\Crypto\SecretsStore.h"

#include "RaiLight\Controllers\ICore.h"
#include "RaiLight\Crypto\PasswordHasher.h"

#include "RaiLight\Database\IDbWrapper.h"
#include "RaiLight\Database\DatabaseKeys.h"

#include "RaiLight\Utilities\MemoryFan.h"
#include "RaiLight\Utilities\Conversions.h"

#include <cryptopp\misc.h>

namespace rail
{
    SecretsStore::SecretsStore(control::ICore* _coreController, ByteArray32 & _generatedKey, const std::string& _seed) : coreController(_coreController)
    {
        generatedKey = std::make_unique<MemoryFan>(_generatedKey, 1024);

        if (!_seed.empty())
        {
            auto decodedSeed = Conversions::decodeHexFromString(_seed);
            CryptoPP::SecureWipeArray(_seed.c_str(), _seed.size());

            if (decodedSeed)
            {
                std::lock_guard<std::shared_mutex> lock(ssMutex);

                //Temporarily encrypt seed with generated key until password has been input.
                secureSeed = std::make_unique<SecureContainer<ByteArray32>>(*decodedSeed, generatedKey.get());
            }
        }
    }

    SecretsStore::~SecretsStore()
    {
    }

    void SecretsStore::setSeed(ByteArray32& seed)
    {
        {
            std::lock_guard<std::shared_mutex> lock(ssMutex);
            secureSeed = std::make_unique<SecureContainer<ByteArray32>>(seed, passwordKey ? passwordKey.get() : generatedKey.get());
        }
        
        CryptoPP::SecureWipeArray(seed.data(), seed.size());
    }

    void SecretsStore::setSeed(const std::string& seed)
    {
        if (!seed.empty())
        {
            auto decodedSeed = Conversions::decodeHexFromString(seed);
            CryptoPP::SecureWipeArray(seed.c_str(), seed.size());

            if (decodedSeed)
            {
                {
                    std::lock_guard<std::shared_mutex> lock(ssMutex);
                    secureSeed = std::make_unique<SecureContainer<ByteArray32>>(*decodedSeed, passwordKey ? passwordKey.get() : generatedKey.get());
                }
            }
        }
    }

    bool SecretsStore::isSeedSet()
    {
        std::shared_lock<std::shared_mutex> lock(ssMutex);
        return secureSeed ? true : false;
    }

    void SecretsStore::setPassword(const std::string& password)
    {
        rail::CryptoUtils::hashPasswordAndStore(coreController->getDatabase(), password);
        auto keyBytes = rail::CryptoUtils::generateKeyFromPassword(coreController->getDatabase(), password);
        passwordKey = std::make_unique<MemoryFan>(keyBytes, 1024);
        
        {
            std::lock_guard<std::shared_mutex> lock(ssMutex);
            if (secureSeed)
            {
                auto tempSeed = secureSeed->getContainer();
                secureSeed = std::make_unique<SecureContainer<ByteArray32>>(tempSeed, passwordKey.get());

                auto encryptedSeed = secureSeed->getEncryptedData();

                coreController->getDatabase()->storeVectorOfBytes(key::bytes::SEED, encryptedSeed, true);
                coreController->getDatabase()->storeByteArray16(key::bytes::SEED_IV, secureSeed->getIv(), true);
            }
        }

        CryptoPP::SecureWipeArray(password.c_str(), password.size());
    }

    MemoryFan * SecretsStore::getGeneratedKey() const
    {
        return generatedKey.get();
    }

    MemoryFan * SecretsStore::getPasswordKey() const
    {
        return passwordKey.get();
    }

    std::optional<ByteArray32> SecretsStore::getSeed()
    {
        std::optional<ByteArray32> returnSeed;

        {
            std::lock_guard<std::shared_mutex> lock(ssMutex);
            if (secureSeed)
            {
                returnSeed = secureSeed->getContainer();
            }
        }

        return returnSeed;
    }
}

