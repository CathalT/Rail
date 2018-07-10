#include "RaiLight\Crypto\PasswordVerifier.h"

#include "RaiLight\Crypto\PasswordHasher.h"

#include "RaiLight\Controllers\ICore.h"

#include "RaiLight\Database\IDbWrapper.h"
#include "RaiLight\Database\DatabaseKeys.h"

#include <cryptopp\misc.h>

rail::PasswordVerifier::PasswordVerifier(rail::control::ICore * _coreController, const std::string & password)
{
    const auto db = _coreController->getDatabase();
    const auto salt = db->getByteArray32(key::bytes::PASS_SALT);
    const auto hash = db->getByteArray32(key::bytes::PASS_HASH);

    if (salt && hash)
    {
        ByteArray32 newHash = CryptoUtils::hashPassword(password, salt.value());

        if (newHash.size() == hash->size())
        {
            valid = CryptoPP::VerifyBufsEqual(reinterpret_cast<const CryptoPP::byte*>(newHash.data()), reinterpret_cast<const CryptoPP::byte*>(hash->data()), newHash.size());
        }
    }
}

bool rail::PasswordVerifier::isValid() const
{
    return valid;
}
