#include "RaiLight\Crypto\SecureString.h"

namespace rail
{
    SecureString::SecureString(std::string & _inputStr, MemoryFan* _obfuscatedKey) : SecureData<std::string>(_inputStr, _obfuscatedKey)
    {
        CryptoPP::SecureWipeArray(_inputStr.c_str(), _inputStr.size());
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
