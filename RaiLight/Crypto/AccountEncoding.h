#pragma once

#include "Model\BasicTypes.h"

namespace rail
{
    namespace CryptoUtils
    {
        bool decodeAccountIdToPublicKeyImpl(const std::string & accoundId, ByteArray32& publicKey);
        std::string encodePublicKeyToAccountIdImpl(const ByteArray32& publicKey);
    }
}