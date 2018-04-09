#pragma once

#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>

template<typename DataType>
inline SecureData<DataType>::SecureData(DataType & data) : dataSize(data.size())
{
    memset(key, 0x00, sizeof(key));
    memset(iv, 0x00, sizeof(iv));

    CryptoPP::ByteQueue plain; 

    plain.Put(reinterpret_cast<const byte*>(data.data()), data.size());

    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

    CryptoPP::StreamTransformationFilter f1(enc, new CryptoPP::Redirector(secureData));
    plain.CopyTo(f1);
    f1.MessageEnd();

    plain.Clear();
}

template<typename DataType>
inline SecureData<DataType>::~SecureData()
{
    CryptoPP::SecureWipeArray(outputBytes.data(), outputBytes.size());
}

template<typename DataType>
inline std::vector<std::byte> SecureData<DataType>::getData()
{
    CryptoPP::ByteQueue recover;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec;
    dec.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

    CryptoPP::StreamTransformationFilter f2(dec, new CryptoPP::Redirector(recover));
    secureData.CopyTo(f2);
    f2.MessageEnd();

    outputBytes.resize(dataSize);
    recover.Get(reinterpret_cast<byte*>(&outputBytes[0]), dataSize);

    recover.Clear();

    return outputBytes;
}