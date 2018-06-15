#include "Crypto\PasswordVerifier.h"

#include "Crypto\PasswordHasher.h"

#include "Controllers\ICore.h"

#include "Database\RailDb.h"
#include "Database\DatabaseKeys.h"

#include <cryptopp\misc.h>

rail::PasswordVerifier::PasswordVerifier(rail::control::ICore * _coreController, const std::string & password)
{
    const auto db = _coreController->getDatabase();
    const auto salt = db->getValue<ByteArray32>(key::bytes::PASS_SALT);
    const auto hash = db->getValue<ByteArray32>(key::bytes::PASS_HASH);

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
