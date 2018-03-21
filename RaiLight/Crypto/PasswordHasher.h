#pragma once

#include "Model\BasicTypes.h"

#include <string>

namespace rail
{
    class RailDb;
}

namespace rail::CryptoUtils
{
    void hashAndStorePassord(rail::RailDb * database, const std::string & password);
    ByteArray32 hashPassword(const std::string & password, const ByteArray32& salt);
}
