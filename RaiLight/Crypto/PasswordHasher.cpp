#include "RaiLight\Crypto\PasswordHasher.h"

#include "RaiLight\Crypto\RandomData.h"

#include "RaiLight\Database\IDbWrapper.h"
#include "RaiLight\Database\DatabaseKeys.h"

#include <cryptopp\misc.h>
#include <argon2.h>

#ifdef TEST_MODE
constexpr uint32_t T_COST = 1;            // 1-pass computation
constexpr uint32_t M_COST = (1 << 8);    // 32 mebibytes memory usage
constexpr uint32_t PARALLELISM = 1;       // number of threads and lanes
#else
constexpr uint32_t T_COST = 4;            // 4-pass computation
constexpr uint32_t M_COST = (1 << 16);    // 64 mebibytes memory usage
constexpr uint32_t PARALLELISM = 1;       // number of threads and lanes
#endif


namespace rail::CryptoUtils
{
    void hashPasswordAndStore(rail::IDbWrapper * database, const std::string & password)
    {
        ByteArray32 salt;
        CryptoUtils::getRandomData(salt.data(), salt.size());

        ByteArray32 hash = hashPassword(password, salt);

        database->storeByteArray32(key::bytes::PASS_SALT, salt, true);
        database->storeByteArray32(key::bytes::PASS_HASH, hash, true);
    }

    ByteArray32 generateKeyFromPassword(rail::IDbWrapper * database, const std::string & password)
    {
        ByteArray32 salt;
        if (auto prevSalt = database->getByteArray32(key::bytes::KEY_SALT))
        {
            salt = *prevSalt;
            CryptoPP::SecureWipeArray((*prevSalt).data(), (*prevSalt).size());
        }
        else
        {
            CryptoUtils::getRandomData(salt.data(), salt.size());
            database->storeByteArray32(key::bytes::KEY_SALT, salt, true);
        }

        return hashPassword(password, salt);
    }

    ByteArray32 hashPassword(const std::string & password, const ByteArray32& salt)
    {
        ByteArray32 hash;

        argon2i_hash_raw(T_COST, M_COST, PARALLELISM, password.data(), password.size(), salt.data(), salt.size(), hash.data(), hash.size());

        return hash;
    }
}
