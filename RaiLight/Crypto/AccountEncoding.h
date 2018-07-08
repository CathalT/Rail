#pragma once

#include "Model\BasicTypes.h"

#include <optional>

namespace rail
{
    namespace CryptoUtils
    {
        std::optional<ByteArray32> decodeAccountIdToPublicKeyImpl(const std::string & accoundId);
        std::string encodePublicKeyToAccountIdImpl(const ByteArray32& publicKey);
    }
}