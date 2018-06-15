#include "Model\OpenBlock.h"

#include "Utilities\Conversions.h"
#include "Crypto\CryptoUtils.h"

namespace rail
{
    namespace blocks
    {
        Open::Open(const SecureContainer<ByteArray32> & _privateKey, const ByteArray32 & _publicKey, const ByteArray32 & _sourceBlock, const ByteArray32 & _representative) :
            Block(_sourceBlock, _privateKey, _publicKey, { _sourceBlock , _representative, _publicKey } ),
            sourceBlock(_sourceBlock),
            representative(_representative)
        {
        }
        std::string Open::getRepresentative() const
        {
            return CryptoUtils::encodePublicKeyToAddress(representative);
        }
        std::string Open::getSourceBlock() const
        {
            return Conversions::encodeToHexStr(sourceBlock);
        }
    }
}
