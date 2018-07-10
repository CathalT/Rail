#pragma once

#include "RaiLight\Database\IDbWrapper.h"

#include "RaiLight\Model\BasicTypes.h"
#include <optional>

namespace rail
{
    class RailDb;

    class DbWrapper : public IDbWrapper
    {
    public:
        DbWrapper();
        ~DbWrapper();

        std::optional< std::vector<std::byte> > getVectorOfBytes(const char* key) override;
        std::optional< ByteArray16 > getByteArray16(const char* key) override;
        std::optional< ByteArray32 > getByteArray32(const char* key) override;

        std::optional< uint32_t > get32UInt(const char* key) override;
        std::optional< bool > getBool(const char* key, const uint32_t index) override;

        bool storeVectorOfBytes(const char* key, const std::vector<std::byte>& bytes, bool safeWrite) override;
        bool storeByteArray16(const char* key, const ByteArray16& bytes, bool safeWrite) override;

        bool storeByteArray32(const char* key, const uint32_t index, const ByteArray32& bytes, bool safeWrite) override;
        bool storeByteArray32(const char* key, const ByteArray32& bytes, bool safeWrite) override;

        bool store32UInt(const char* key, const uint32_t num) override;
        bool storeBool(const char* key, const uint32_t index, const bool result, bool safeWrite) override;

        bool deleteValue(const char* key, const uint32_t index, const bool safeWrite) override;

    private:

        std::unique_ptr<RailDb> dbImpl;
    };
}