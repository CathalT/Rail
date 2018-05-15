#include "Crypto\SecureString.h"

namespace rail
{
    SecureString::SecureString(std::string & inputStr) : SecureData<std::string>(inputStr)
    {
        CryptoPP::SecureWipeArray(inputStr.c_str(), inputStr.size());
    }

    std::string SecureString::getString()
    {
        auto bytes = getData();

        std::string visibleString;
        visibleString.assign(reinterpret_cast<char*>(bytes.data()), bytes.size());

        CryptoPP::SecureWipeArray(bytes.data(), bytes.size());

        return visibleString;
    }
}
