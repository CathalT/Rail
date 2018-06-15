#include "Model\ChangeBlock.h"

#include "Crypto\CryptoUtils.h"

namespace rail
{
    namespace blocks
    {
        Change::Change(const ByteArray32 & _previousBlock, const ByteArray32 & _representative, const SecureContainer<ByteArray32> & _privateKey, const ByteArray32 & _publicKey) :
            Block(_previousBlock, _privateKey, _publicKey, { _previousBlock, _representative }),
            representative(_representative)
        {

        }

        std::string Change::getRepresentativeAddress() const
        {
            return CryptoUtils::encodePublicKeyToAddress(representative);
        }
    }
}

