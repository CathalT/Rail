#pragma once

#include "RaiLight\Crypto\SecureData.h"
#include "RaiLight\Utilities\MemoryFan.h"

#include <cryptopp\misc.h>

namespace rail
{
    template<typename Container>
    inline SecureContainer<Container>::SecureContainer(Container & _inputContainer, MemoryFan* _obfuscatedKey) : SecureData<Container>(_inputContainer, _obfuscatedKey)
    {
        CryptoPP::SecureWipeArray(_inputContainer.data(), _inputContainer.size());
    }

    template<typename Container>
    inline Container SecureContainer<Container>::getContainer()
    {
        auto rawData = getData();

        Container returnContainer;
        std::copy(std::begin(rawData), std::end(rawData), std::begin(returnContainer));

        CryptoPP::SecureWipeArray(rawData.data(), rawData.size());

        return returnContainer;
    }
}
