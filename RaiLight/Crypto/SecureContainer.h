#pragma once

#include "RaiLight\Crypto\SecureData.h"

namespace rail
{
    class MemoryFan;
    template<typename Container>
    class SecureContainer : public SecureData<Container>
    {
    public:
        SecureContainer() = default;
        SecureContainer(Container& _inputContainer, MemoryFan* _obfuscatedKey);

        SecureContainer(const SecureContainer& other) = default;
        SecureContainer(SecureContainer&& other) = default;
        SecureContainer& operator= (const SecureContainer& other) = default;
        SecureContainer& operator= (SecureContainer&& other) = default;

        Container getContainer();
    };
}

#include "RaiLight\Crypto\SecureContainerImpl.h"
