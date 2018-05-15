#pragma once

#include "Crypto\SecureData.h"

namespace rail
{
    class SecureString : public SecureData<std::string>
    {
    public:
        SecureString(std::string& inputStr);
        std::string getString();
    };
}
