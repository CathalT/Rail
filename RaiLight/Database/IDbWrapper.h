#pragma once

#include "RaiLight\Model\BasicTypes.h"

#include <optional>
#include <vector>
#include <cstddef>

namespace rail
{
    class IDbWrapper
    {
    public:
        virtual ~IDbWrapper() = default;

        virtual std::optional< std::vector<std::byte> > getVectorOfBytes(const char* key) = 0;
        virtual std::optional< ByteArray16 > getByteArray16(const char* key) = 0;
        virtual std::optional< ByteArray32 > getByteArray32(const char* key) = 0;

        virtual std::optional< uint32_t > get32UInt(const char* key) = 0;
        virtual std::optional< bool > getBool(const char* key, const uint32_t index) = 0;

        virtual bool storeVectorOfBytes(const char* key, const std::vector<std::byte>& bytes, bool safeWrite) = 0;
        virtual bool storeByteArray16(const char* key, const ByteArray16& bytes, bool safeWrite) = 0;
        virtual bool storeByteArray32(const char* key, const uint32_t index, const ByteArray32& bytes, bool safeWrite) = 0;
        virtual bool storeByteArray32(const char* key, const ByteArray32& bytes, bool safeWrite) = 0;

        virtual bool storeBool(const char* key, const uint32_t index, const bool result, bool safeWrite) = 0;
        virtual bool store32UInt(const char* key, const uint32_t num) = 0;

        virtual bool deleteValue(const char* key, const uint32_t index, const bool safeWrite) = 0; 
    };
}