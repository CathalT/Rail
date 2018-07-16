#include "RailUnitTests\fakes\FakeDb.h"

FakeDb::FakeDb()
{

}

std::optional<std::vector<std::byte>> FakeDb::getVectorOfBytes(const char* key)
{
    std::optional<std::vector<std::byte>> returnVal;

    auto itr = vectorByteMap.find(key);
    if (itr != vectorByteMap.end())
    {
        returnVal = itr->second;
    }

    return returnVal;
}

std::optional<rail::ByteArray16> FakeDb::getByteArray16(const char* key)
{
    std::optional<rail::ByteArray16> returnVal;

    auto itr = byte16Map.find(key);
    if (itr != byte16Map.end())
    {
        returnVal = itr->second;
    }

    return returnVal;
}

std::optional<rail::ByteArray32> FakeDb::getByteArray32(const char* key)
{
    std::optional<rail::ByteArray32> returnVal;

    auto itr = byte32Map.find(key);
    if (itr != byte32Map.end())
    {
        returnVal = itr->second;
    }

    return returnVal;
}

std::optional<uint32_t> FakeDb::get32UInt(const char* key)
{
    std::optional<uint32_t> returnVal;

    auto itr = uint32Map.find(key);
    if (itr != uint32Map.end())
    {
        returnVal = itr->second;
    }

    return returnVal;
}

std::optional<bool> FakeDb::getBool(const char* key, const uint32_t index)
{
    std::optional<bool> returnVal;

    auto newKey = std::string(key + std::to_string(index));

    auto itr = uint32Map.find(newKey);
    if (itr != uint32Map.end())
    {
        returnVal = itr->second;
    }

    return returnVal;
}

bool FakeDb::storeVectorOfBytes(const char* key, const std::vector<std::byte>& bytes, bool /*safeWrite*/)
{
    auto success = vectorByteMap.insert({ key, bytes });
    return success.second;
}

bool FakeDb::storeByteArray16(const char* key, const rail::ByteArray16& bytes, bool /*safeWrite*/)
{
    auto success = byte16Map.insert({ key, bytes });
    return success.second;
}

bool FakeDb::storeByteArray32(const char* key, const uint32_t index, const rail::ByteArray32& bytes, bool /*safeWrite*/)
{
    auto strKey = std::string(key + std::to_string(index));
    auto success = byte32Map.insert({ strKey, bytes });
    return success.second;
}

bool FakeDb::storeByteArray32(const char* key, const rail::ByteArray32& bytes, bool /*safeWrite*/)
{
    auto success = byte32Map.insert({ key, bytes });
    return success.second;
}

bool FakeDb::storeBool(const char* key, const uint32_t index, const bool result, bool /*safeWrite*/)
{
    auto strKey = std::string(key + std::to_string(index));
    auto success = boolMap.insert({ strKey, result });
    return success.second;
}

bool FakeDb::store32UInt(const char* key, const uint32_t num)
{
    auto success = uint32Map.insert({ key, num });
    return success.second;
}

bool FakeDb::deleteValue(const char* key, const uint32_t index, const bool /*safeWrite*/)
{
    auto strKey = std::string(key + std::to_string(index));

    bool result = true;

    if (!boolMap.erase(strKey))
    {
        if (!uint32Map.erase(strKey))
        {
            if (!byte16Map.erase(strKey))
            {
                if (!byte32Map.erase(strKey))
                {
                    if (!vectorByteMap.erase(strKey))
                    {
                        result = false;
                    }
                }
            }
        }
    }

    return result;
}

