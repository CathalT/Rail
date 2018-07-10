#pragma once

#include "RaiLight\Utilities\MemoryFan.h"
#include "RaiLight\Crypto\RandomData.h"

#include <cryptopp\cryptlib.h>
#include <cryptopp\filters.h>
#include <cryptopp\files.h>
#include <cryptopp\modes.h>
#include <cryptopp\hex.h>
#include "RaiLight\Crypto\SecureData.h"


namespace rail
{

    template<typename DataType>
    inline SecureData<DataType>::SecureData(DataType & _data, MemoryFan* _obfuscatedKey) : obfuscatedKey(_obfuscatedKey)
    {
        CryptoUtils::getRandomData(iv.data(), iv.size());

        CryptoPP::ByteQueue plain;

        plain.Put(reinterpret_cast<const CryptoPP::byte*>(_data.data()), _data.size());

        auto key = obfuscatedKey->getValue();

        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc;
        enc.SetKeyWithIV(reinterpret_cast<CryptoPP::byte*>(key.data()), key.size(), reinterpret_cast<const CryptoPP::byte*>(iv.data()), iv.size());

        CryptoPP::SecureWipeArray(key.data(), key.size());

        CryptoPP::StreamTransformationFilter f1(enc, new CryptoPP::Redirector(secureData));
        plain.CopyTo(f1);
        f1.MessageEnd();

        plain.Clear();
    }

    template<typename DataType>
    inline SecureData<DataType>::~SecureData()
    {
        obfuscatedKey = nullptr;
        secureData.Clear();
        CryptoPP::SecureWipeArray(iv.data(), iv.size());
    }

    template<typename DataType>
    inline SecureData<DataType>::SecureData(const SecureData<DataType> & other)
    {
        this->obfuscatedKey = other.obfuscatedKey;
        this->iv = other.iv;
        this->secureData = other.secureData;
    }

    template<typename DataType>
    inline SecureData<DataType>::SecureData(SecureData<DataType> && other)
    {
        this->obfuscatedKey = other.obfuscatedKey;
        this->iv = other.iv;
        this->secureData = other.secureData;
        
        other.obfuscatedKey = nullptr;
        CryptoPP::SecureWipeArray(other.iv.data(), other.iv.size());
        other.secureData.Clear();
    }

    template<typename DataType>
    inline SecureData<DataType> & SecureData<DataType>::operator=(const SecureData<DataType> & other)
    {
        SecureData<DataType> tmp(other);
        *this = std::move(tmp);
        return *this;
    }

    template<typename DataType>
    inline  SecureData<DataType> & SecureData<DataType>::operator=(SecureData<DataType> && other)
    {
        if (this == &other)
        {
            // take precautions against `foo = std::move(foo)`
            return *this;
        }

        this->obfuscatedKey = nullptr;
        CryptoPP::SecureWipeArray(this->iv.data(), this->iv.size());
        this->secureData.Clear();

        this->obfuscatedKey = other.obfuscatedKey;
        this->iv = other.iv;
        this->secureData = other.secureData;

        other.obfuscatedKey = nullptr;
        CryptoPP::SecureWipeArray(other.iv.data(), other.iv.size());
        other.secureData.Clear();

        return *this;
    }

    template<typename DataType>
    inline std::array<std::byte, CryptoPP::AES::BLOCKSIZE> SecureData<DataType>::getIv() const
    {
        return iv;
    }

    template<typename DataType>
    inline std::vector<std::byte> SecureData<DataType>::getEncryptedData()
    {
        std::vector<std::byte> outputBytes;

        outputBytes.resize(static_cast<const unsigned int>(secureData.CurrentSize()));

        secureData.Peek(reinterpret_cast<CryptoPP::byte*>(&outputBytes[0]), static_cast<size_t>(secureData.CurrentSize()));

        return outputBytes;
    }

    template<typename DataType>
    inline std::vector<std::byte> SecureData<DataType>::getData()
    {
        CryptoPP::ByteQueue recover;

        auto key = obfuscatedKey->getValue();

        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec;
        dec.SetKeyWithIV(reinterpret_cast<CryptoPP::byte*>(key.data()), key.size(), reinterpret_cast<const CryptoPP::byte*>(iv.data()), iv.size());

        CryptoPP::SecureWipeArray(key.data(), key.size());

        CryptoPP::StreamTransformationFilter f2(dec, new CryptoPP::Redirector(recover));
        secureData.CopyTo(f2);
        f2.MessageEnd();

        std::vector<std::byte> outputBytes;
        outputBytes.resize(static_cast<const unsigned int>(recover.CurrentSize()));

        recover.Get(reinterpret_cast<CryptoPP::byte*>(&outputBytes[0]), static_cast<size_t>(recover.CurrentSize()));

        recover.Clear();

        return outputBytes;
    }
}
