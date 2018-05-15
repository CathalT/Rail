#include "Utilities/MemoryFan.h"

#include "Crypto/RandomData.h"

#include <cryptopp/misc.h>


namespace rail
{
    //MemoryFan::~MemoryFan() = default;

    MemoryFan::MemoryFan(ByteArray32 & key, const size_t count_a)
    {
        std::unique_ptr<ByteArray32> first(new ByteArray32(key));

        CryptoPP::SecureWipeArray(key.data(), key.size());

        for (size_t a(1); a < count_a; ++a)
        {
            std::unique_ptr<ByteArray32> entry(new ByteArray32);
            CryptoUtils::getRandomData(reinterpret_cast<byte*>(entry->data()), entry->size());

            std::transform(first->begin(), first->end(), entry->begin(), first->begin(), [](std::byte& left, std::byte& right)
            {
                return left ^ right;
            });

            values.push_back(std::move(entry));
        }
        values.push_back(std::move(first));
    }

    ByteArray32 MemoryFan::getValue()
    {
        ByteArray32 returnVal;
        returnVal.fill(std::byte());

        for (auto & v : values)
        {
            std::transform(returnVal.begin(), returnVal.end(), v->begin(), returnVal.begin(), [](const std::byte& left, const std::byte& right)
            {
                return left ^ right;
            });
        }

        return returnVal;
    }
}
