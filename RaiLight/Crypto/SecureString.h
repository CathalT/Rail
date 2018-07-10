#pragma once

#include "RaiLight\Crypto\SecureData.h"

namespace rail
{
    class MemoryFan;
    class SecureString : public SecureData<std::string>
    {
    public:
        SecureString(std::string& _inputStr, MemoryFan* _obfuscatedKey);
        std::string getString();
    };
}
