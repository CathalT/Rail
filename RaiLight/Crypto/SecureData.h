#pragma once

#include <vector>

namespace rail
{
    template<typename DataType>
    class SecureData
    {
    public:
        SecureData(DataType& data);
        ~SecureData();

    protected:
        std::vector<std::byte> getData();

    private:

        size_t dataSize{ 0 };
        std::vector<std::byte> secureData;
    };
}

#include "Crypto\SecureDataImpl.h"