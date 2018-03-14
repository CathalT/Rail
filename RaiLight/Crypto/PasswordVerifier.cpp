#include "Crypto\PasswordVerifier.h"

#include "Controllers\ICore.h"
#include "Database\RailDb.h"
#include "Database\DatabaseKeys.h"

#include <cryptopp\misc.h>

#include <argon2.h>

rail::PasswordVerifier::PasswordVerifier(rail::control::ICore * _coreController, const std::string & password)
{
    const auto db = _coreController->getDatabase();
    const auto salt = db->getValue<ByteArray32>(key::bytes::PASS_SALT);
    const auto hash = db->getValue<ByteArray32>(key::bytes::PASS_HASH);

    if (salt && hash)
    {
        ByteArray32 newHash;

        uint32_t t_cost = 2;            // 2-pass computation
        uint32_t m_cost = (1 << 16);    // 64 mebibytes memory usage
        uint32_t parallelism = 1;       // number of threads and lanes

        argon2i_hash_raw(t_cost, m_cost, parallelism, password.data(), password.size(), salt.value().data(), salt.value().size(), newHash.data(), newHash.size());

        if (newHash.size() == hash->size())
        {
            valid = CryptoPP::VerifyBufsEqual(reinterpret_cast<const byte*>(newHash.data()), reinterpret_cast<const byte*>(hash->data()), newHash.size());
        }
    }
}

bool rail::PasswordVerifier::isValid() const
{
    return valid;
}
