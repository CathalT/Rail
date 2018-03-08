#pragma once

#include "Model\BasicTypes.h"
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

        bool deleteValue(const char* key, const bool safeWrite = false);
        bool deleteValue(const char* key, const uint32_t index, const bool safeWrite = false);

        template<typename T>
        bool storeValue(const char* key, const T& val, const bool safeWrite = false);

        template<typename T>
        bool storeValue(const char* key, const uint32_t index, const T& val, const bool safeWrite = false);

        template<typename T>
        std::optional<T> getValue(const char* key);

        template<typename T>
        std::optional<T> getValue(const char* key, const uint32_t index);
    private:
        const char* keyWithIndex(const char* key, const uint32_t index);
    private:
        leveldb::DB* db{ nullptr };
    };
}

#include "Database\DatabaseImpl.h"
