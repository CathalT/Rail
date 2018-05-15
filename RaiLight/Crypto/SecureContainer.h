#pragma once

#include "Crypto\SecureData.h"

namespace rail
{
    template<typename Container>
    class SecureContainer : public SecureData<Container>
    {
    public:
        SecureContainer(Container& inputContainer);
        Container getContainer();
    };
}

#include "Crypto\SecureContainerImpl.h"
