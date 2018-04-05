#pragma once

#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/modes.h>
#include <cryptopp/queue.h>
#include <cryptopp/hex.h>
#include <cryptopp/aes.h>

namespace rail
{
    template<typename DataType>
    inline SecureData<DataType>::SecureData(DataType & data) : dataSize(data.size())
    {
        byte key[CryptoPP::AES::MAX_KEYLENGTH];
        byte iv[CryptoPP::AES::BLOCKSIZE];

        memset(key, 0x00, sizeof(key));
        memset(iv, 0x00, sizeof(iv));

        CryptoPP::ByteQueue plain, cipher, recover;

        plain.Put(reinterpret_cast<const byte*>(data.data()), data.size());


        /////////////////////////////////////////////////////////////

        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc;
        enc.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

        CryptoPP::StreamTransformationFilter f1(enc, new CryptoPP::Redirector(cipher));
        plain.CopyTo(f1);
        f1.MessageEnd();

        /////////////////////////////////////////////////////////////

        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec;
        dec.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

        CryptoPP::StreamTransformationFilter f2(dec, new CryptoPP::Redirector(recover));
        cipher.CopyTo(f2);
        f2.MessageEnd();

        secureData.resize(dataSize);
        recover.Get(reinterpret_cast<byte*>(&secureData[0]), dataSize);
    }

    template<typename DataType>
    inline SecureData<DataType>::~SecureData()
    {
    }

    template<typename DataType>
    inline std::vector<std::byte> SecureData<DataType>::getData()
    {
        return secureData;
    }
}
