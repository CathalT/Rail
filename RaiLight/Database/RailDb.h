#pragma once

#include "RaiLight\Model\BasicTypes.h"
#include <optional>

namespace leveldb
{
    class DB;
}

namespace rail
{
    class RailDb
    {
    public: 
        RailDb();
        ~RailDb();

        bool deleteValue(const char* key, const bool safeWrite);
        bool deleteValue(const char* key, const uint32_t index, const bool safeWrite);

        template<typename T>
        bool storeValue(const char* key, const T& val, const bool safeWrite);

        template<typename T>
        bool storeValue(const char* key, const uint32_t index, const T& val, const bool safe);

        template<typename T>
        bool storeDynamicValue(const char* key, const T& val, const size_t size, const bool safeWrite);

        template<typename T>
        std::optional<T> getValue(const char* key);

        template<typename T>
        std::optional<T> getValue(const char* key, const uint32_t index);

        template<typename T>
        std::optional<T> getDynamicValue(const char* key);
        
    private:
        const char* keyWithIndex(const char* key, const uint32_t index);
    private:
        leveldb::DB* db{ nullptr };
    };
}

#include "RaiLight\Database\DatabaseImpl.h"
