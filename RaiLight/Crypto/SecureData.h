#pragma once

#include <cryptopp\config.h>
#include <cryptopp\aes.h>
#include <cryptopp\queue.h>

#include <vector>
#include <memory>

namespace rail
{
    class MemoryFan;

    template<typename DataType>
    class SecureData
    {
    public:
        SecureData() = default;
        SecureData(DataType& _data, MemoryFan* _obfuscatedKey);
        ~SecureData();
        std::vector<std::byte> getEncryptedData();
        std::array<std::byte, CryptoPP::AES::BLOCKSIZE> getIv() const;

        SecureData(const SecureData& other);
        SecureData(SecureData&& other);
        SecureData& operator= (const SecureData& other);
        SecureData& operator= (SecureData&& other);

    protected:
        std::vector<std::byte> getData();

    private:
        MemoryFan * obfuscatedKey{ nullptr };

        std::array<std::byte, CryptoPP::AES::BLOCKSIZE> iv;

        CryptoPP::ByteQueue secureData;
    };
}

#include "RaiLight\Crypto\SecureDataImpl.h"
