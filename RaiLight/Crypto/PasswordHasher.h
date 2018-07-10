#pragma once

#include "RaiLight\Model\BasicTypes.h"

#include <string>

namespace rail
{
    class IDbWrapper;
}

namespace rail::CryptoUtils
{
    void hashPasswordAndStore(rail::IDbWrapper * database, const std::string & password);
    ByteArray32 generateKeyFromPassword(rail::IDbWrapper * database, const std::string & password);
    ByteArray32 hashPassword(const std::string & password, const ByteArray32& salt);
}
