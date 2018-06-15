#include <cryptopp\osrng.h>

namespace rail
{
    namespace CryptoUtils
    {
        void getRandomData(void * out, size_t outlen);

        template<typename ByteArray>
        void fillWithRandomData(ByteArray& outputData)
        {
            CryptoPP::AutoSeededRandomPool randPool;

            randPool.GenerateBlock(reinterpret_cast<CryptoPP::byte*>(outputData.data()), outputData.size());
        }
    }
}
