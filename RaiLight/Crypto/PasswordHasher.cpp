#include "Crypto\PasswordHasher.h"

#include "Crypto\RandomData.h"

#include "Database\RailDb.h"
#include "Database\DatabaseKeys.h"

#include <cryptopp\misc.h>
#include <argon2.h>

namespace rail::CryptoUtils
{
    void hashPasswordAndStore(rail::RailDb * database, const std::string & password)
    {
        ByteArray32 salt;
        CryptoUtils::getRandomData(salt.data(), salt.size());

        ByteArray32 hash = hashPassword(password, salt);

        database->storeValue(key::bytes::PASS_SALT, salt, true);
        database->storeValue(key::bytes::PASS_HASH, hash, true);
    }

    ByteArray32 generateKeyFromPassword(rail::RailDb * database, const std::string & password)
    {
        ByteArray32 salt;
        if (auto prevSalt = database->getValue<ByteArray32>(key::bytes::KEY_SALT))
        {
            salt = *prevSalt;
            CryptoPP::SecureWipeArray((*prevSalt).data(), (*prevSalt).size());
        }
        else
        {
            CryptoUtils::getRandomData(salt.data(), salt.size());
            database->storeValue(key::bytes::KEY_SALT, salt, true);
        }

        return hashPassword(password, salt);
    }

    ByteArray32 hashPassword(const std::string & password, const ByteArray32& salt)
    {
        ByteArray32 hash;

        uint32_t t_cost = 4;            // 4-pass computation
        uint32_t m_cost = (1 << 16);    // 64 mebibytes memory usage
        uint32_t parallelism = 1;       // number of threads and lanes

        argon2i_hash_raw(t_cost, m_cost, parallelism, password.data(), password.size(), salt.data(), salt.size(), hash.data(), hash.size());

        return hash;
    }
}
