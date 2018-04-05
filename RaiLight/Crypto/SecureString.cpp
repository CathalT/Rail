#include "Crypto\SecureString.h"

namespace rail
{
    SecureString::SecureString(std::string & inputStr) : SecureData<std::string>(inputStr)
    {

    }

    std::string SecureString::getString()
    {
        auto bytes = getData();
        return std::string();
    }
}