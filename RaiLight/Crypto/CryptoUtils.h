#pragma once

#include "Model\BasicTypes.h"

#include <string>
#include <vector>
#include <optional>

namespace rail
{
    class MemoryFan;
    namespace CryptoUtils
    {
        ByteArray64 signMessage(const ByteArray32& privateKey, const ByteArray32& publicKey, const ByteArray32& hashedMessage);
        ByteArray32 getRandom32ByteBlock();
        ByteArray32 generateKeyFromSeedAndIndex(const ByteArray32& seed, const uint32_t index);
        ByteArray32 generatePublicKeyFromPrivateKey(const ByteArray32& privateKey);
        ByteArray32 blockHash(const std::vector<ByteArray32>& vStr);
        std::optional<ByteArray32> decodeAccountIdToPublicKey(const std::string & accoundId);
        std::string encodePublicKeyToAddress(const ByteArray32& publicKey);
        ByteArray32 AESDecryptByteArray32(const std::vector<std::byte> & encryptedData, MemoryFan* obfuscatedKey, const ByteArray16& iv);
    }
}
