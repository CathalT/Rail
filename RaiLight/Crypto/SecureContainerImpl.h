#pragma once

#include <Crypto/SecureData.h>

#include <cryptopp/misc.h>

namespace rail
{
    template<typename Container>
    inline SecureContainer<Container>::SecureContainer(Container & inputContainer) : SecureData<Container>(inputContainer)
    {
        CryptoPP::SecureWipeArray(inputContainer.data(), inputContainer.size());
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
