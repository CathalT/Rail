#pragma once

#include <cryptopp/config.h>
#include <cryptopp/aes.h>
#include <cryptopp/queue.h>

#include <vector>
#include <memory>

namespace rail
{
    class MemoryFan;

    template<typename DataType>
    class SecureData
    {
    public:
        SecureData(DataType& data);

        std::vector<std::byte> getEncryptedData();

    protected:
        std::vector<std::byte> getData();

    private:

        std::unique_ptr<MemoryFan> keyFan;

        byte key[CryptoPP::AES::MAX_KEYLENGTH];
        byte iv[CryptoPP::AES::BLOCKSIZE];

        size_t dataSize{ 0 };

        CryptoPP::ByteQueue secureData;
    };
}

#include "Crypto\SecureDataImpl.h"
