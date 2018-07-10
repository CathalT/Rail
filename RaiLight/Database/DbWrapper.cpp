#include "RaiLight\Database\DbWrapper.h"
#include "RaiLight\Database\RailDb.h"

namespace rail
{
    DbWrapper::DbWrapper() : dbImpl(std::make_unique<RailDb>())
    {
    }

    DbWrapper::~DbWrapper()
    {
    }

    std::optional< std::vector<std::byte> > DbWrapper::getVectorOfBytes(const char* key)
    {
        return dbImpl->getDynamicValue<std::vector<std::byte>>(key);
    }

    std::optional< ByteArray16 > DbWrapper::getByteArray16(const char* key)
    {
        return dbImpl->getValue<ByteArray16>(key);
    }

    std::optional< ByteArray32 > DbWrapper::getByteArray32(const char* key)
    {
        return dbImpl->getValue<ByteArray32>(key);
    }

    std::optional< uint32_t > DbWrapper::get32UInt(const char* key)
    {
        return dbImpl->getValue<uint32_t>(key);
    }

    std::optional< bool > DbWrapper::getBool(const char* key, const uint32_t index)
    {
        return dbImpl->getValue<bool>(key, index);
    }

    bool DbWrapper::storeVectorOfBytes(const char* key, const std::vector<std::byte>& bytes, bool safeWrite)
    {
        return dbImpl->storeDynamicValue(key, bytes, bytes.size(), safeWrite);
    }

    bool DbWrapper::storeByteArray16(const char* key, const ByteArray16& bytes, bool safeWrite)
    {
        return dbImpl->storeValue(key, bytes, safeWrite);
    }

    bool DbWrapper::storeByteArray32(const char* key, const uint32_t index, const ByteArray32& bytes, bool safeWrite)
    {
        return dbImpl->storeValue(key, index, bytes, safeWrite);
    }

    bool DbWrapper::storeByteArray32(const char* key, const ByteArray32& bytes, bool safeWrite)
    {
        return dbImpl->storeValue(key, bytes, safeWrite);
    }

    bool DbWrapper::store32UInt(const char* key, const uint32_t num)
    {
        return dbImpl->storeValue(key, num, false);
    }

    bool DbWrapper::storeBool(const char* key, const uint32_t index, const bool result, bool safeWrite)
    {
        return dbImpl->storeValue(key, index, result, safeWrite);
    }

    bool DbWrapper::deleteValue(const char* key, const uint32_t index, const bool safeWrite)
    {
        return dbImpl->deleteValue(key, index, safeWrite);
    }
}
