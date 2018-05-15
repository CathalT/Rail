#include "Crypto\SecretsStore.h"

#include "Controllers\ICore.h"
#include "Crypto\PasswordHasher.h"
#include "Database\RailDb.h"

#include "Utilities\MemoryFan.h"
#include "Utilities\Conversions.h"

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
                secureSeed = std::make_unique<SecureContainer<ByteArray32>>(*decodedSeed);
            }
        }
    }

    SecretsStore::~SecretsStore()
    {
    }

    void SecretsStore::setSeed(ByteArray32& seed)
    {
        std::lock_guard<std::shared_mutex> lock(ssMutex);
        secureSeed = std::make_unique<SecureContainer<ByteArray32>>(seed);
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
                std::lock_guard<std::shared_mutex> lock(ssMutex);
                secureSeed = std::make_unique<SecureContainer<ByteArray32>>(*decodedSeed);
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

        if (secureSeed)
        {
            std::lock_guard<std::shared_mutex> lock(ssMutex);
            returnSeed = secureSeed->getContainer();
        }

        return returnSeed;
    }
}

