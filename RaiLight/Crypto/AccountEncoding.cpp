#include "RaiLight\Crypto\AccountEncoding.h"

#include "RaiLight\Utilities\Conversions.h"
#include "RaiLight\Model\BasicTypes.h"

#include <blake2\blake2.h>

static constexpr uint8_t ASCII_ZERO_CHAR = 0x30;
static constexpr uint8_t ASCII_END_VAL = 0x80;
static constexpr uint8_t ACCOUNT_SIZE = 64;
static constexpr uint8_t NUM_BITS_IN_CHECKSUM = 40;

namespace rail
{
    namespace CryptoUtils
    {
        static uint64_t expectedChecksum(const ByteArray32& pubKey)
        {
            uint64_t validation(0);

            blake2b_state hash;
            blake2b_init(&hash, 5);
            blake2b_update(&hash, pubKey.data(), pubKey.size());
            blake2b_final(&hash, reinterpret_cast<uint8_t *> (&validation), 5);

            return validation;
        }

        static bool validatePublicKey(const uInt512_t& checksum, const ByteArray32& pubKey)
        {
            //uint64_t check(checksum.convert_to<uint64_t>());
            uint64_t check = static_cast<uint64_t>(checksum & static_cast<uint64_t>(0xffffffffff));

            return check == expectedChecksum(pubKey);
        }

        static uInt256_t getPublicKeyFromNum(const uInt512_t& pubkeyAndChecksum)
        {
            uInt256_t pubKey = (pubkeyAndChecksum >> NUM_BITS_IN_CHECKSUM).convert_to<uInt256_t>();
            return pubKey;
        }

        static bool decodeAndParseByte(const uint8_t byte, uInt512_t& output)
        {
            bool succeeded{ true };

            uint8_t decodedByte(Conversions::decodeByte(byte));

            if (decodedByte != '~')
            {
                output <<= 5;
                output += decodedByte;
            }
            else
            {
                succeeded = false;
            }

            return succeeded;
        }

        static bool isValidAsciiChar(const uint8_t ch)
        {
            return (ch >= ASCII_ZERO_CHAR) && (ch < ASCII_END_VAL);
        }

        static bool decodeStrToBytes(const std::string account, uInt512_t& output)
        {
            bool succeeded{ true };

            for (const auto c : account)
            {
                if (isValidAsciiChar(c))
                {
                    if (decodeAndParseByte(c, output))
                    {
                        continue;
                    }
                }
                succeeded = false;
                break;
            }

            return succeeded;
        }

        static std::string stripAccountPrefix(const std::string& accountId)
        {
            std::string accountCopy;

            if (accountId[0] == 'x' && accountId[1] == 'r' && accountId[2] == 'b' && (accountId[3] == '_' || accountId[3] == '-'))
            {
                accountCopy = std::string(accountId.begin() + 4, accountId.end());
            }

            return accountCopy;
        }

        std::optional<ByteArray32> decodeAccountIdToPublicKeyImpl(const std::string & accountId)
        {
            std::optional<ByteArray32> pubKey;

            if (accountId.size() == ACCOUNT_SIZE)
            {
                const auto accountCopy = stripAccountPrefix(accountId);
                if (!accountCopy.empty())
                {
                    uInt512_t accountNum(0);   
                    if (decodeStrToBytes(accountCopy, accountNum))
                    {
                        auto pk = Conversions::intToByteArray<ByteArray32>(getPublicKeyFromNum(accountNum));
                        if (validatePublicKey(accountNum, pk))
                        {
                            pubKey = pk;
                        }
                    }
                }
            }

            return pubKey;
        }

        std::string encodePublicKeyToAccountIdImpl(const ByteArray32 & publicKey)
        {
            std::string accountId;
            accountId.reserve(64);

            uint64_t check(0);
            blake2b_state hash;
            blake2b_init(&hash, 5);
            blake2b_update(&hash, publicKey.data(), publicKey.size());
            blake2b_final(&hash, reinterpret_cast<uint8_t *> (&check), 5);

            auto binary = Conversions::hexBytesToInt<uInt512_t>(publicKey);
            binary <<= NUM_BITS_IN_CHECKSUM;
            binary |= uInt512_t(check);

            for (auto i(0); i < 60; ++i)
            {
                uint8_t r(binary & static_cast<uint8_t> (0x1f));
                binary >>= 5;
                accountId.push_back(Conversions::encodeByte(static_cast<uint8_t>(r)));
            }

            accountId.append("_brx");
            std::reverse(accountId.begin(), accountId.end());

            return accountId;
        }
    }
}
