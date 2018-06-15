#include "Crypto\RandomData.h"

void rail::CryptoUtils::getRandomData(void * out, size_t outlen)
{
    CryptoPP::AutoSeededRandomPool randPool;

    randPool.GenerateBlock(static_cast<CryptoPP::byte*>(out), outlen);
}
