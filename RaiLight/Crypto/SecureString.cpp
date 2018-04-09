#include "Crypto\SecureString.h"


SecureString::SecureString(std::string & inputStr) : SecureData<std::string>(inputStr)
{
    CryptoPP::SecureWipeArray(inputStr.c_str(), inputStr.size());
}

SecureString::~SecureString()
{
    CryptoPP::SecureWipeArray(visibleString.c_str(), visibleString.size());
}

std::string& SecureString::getString()
{
    auto bytes = getData();

    visibleString.assign(reinterpret_cast<char*>(bytes.data()), bytes.size());

    return visibleString;
}
