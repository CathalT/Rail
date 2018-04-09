#include <vector>

#include <cryptopp/config.h>
#include <cryptopp/aes.h>
#include <cryptopp/queue.h>

template<typename DataType>
class SecureData
{
public:
    SecureData(DataType& data);
    ~SecureData();

protected:
    std::vector<std::byte> getData();

private:

    byte key[CryptoPP::AES::MAX_KEYLENGTH];
    byte iv[CryptoPP::AES::BLOCKSIZE];

    size_t dataSize{ 0 };

    std::vector<std::byte> outputBytes;
    CryptoPP::ByteQueue secureData;
};

#include "Crypto\SecureDataImpl.h"