#pragma once
#include <leveldb\db.h>

namespace rail
{
    template<typename T>
    bool RailDb::storeValue(const char* key, const T& val, const bool safeWrite)
    {
        auto writeOptions = leveldb::WriteOptions();
        writeOptions.sync = safeWrite;
        const auto status = db->Put(writeOptions, key, leveldb::Slice(reinterpret_cast<const char*>(&val), sizeof(val)));
        return status.ok();
    }

    template<typename T>
    bool RailDb::storeValue(const char* key, const uint32_t index, const T& val, const bool safeWrite)
    {
        return storeValue(keyWithIndex(key, index), val, safeWrite);
    }

    template<typename T>
    bool RailDb::storeDynamicValue(const char* key, const T& val, const size_t size, const bool safeWrite)
    {
        auto writeOptions = leveldb::WriteOptions();
        writeOptions.sync = safeWrite;
        const auto status = db->Put(writeOptions, key, leveldb::Slice(reinterpret_cast<const char*>(&val[0]), size));
        return status.ok();
    }

    template<typename T>
    std::optional<T> RailDb::getValue(const char* key)
    {
        std::optional<T> returnVal;
        std::string outStr;
        const auto status = db->Get(leveldb::ReadOptions(), key, &outStr);

        if (status.ok())
        {
            returnVal = *(reinterpret_cast<T*>(outStr.data()));
        }

        return returnVal;
    }

    template<typename T>
    std::optional<T> RailDb::getValue(const char* key, const uint32_t index)
    {
        return getValue<T>(keyWithIndex(key, index));
    }

    template<typename T>
    std::optional<T> RailDb::getDynamicValue(const char* key)
    {
        std::optional<T> returnVal;
        std::string outStr;
        const auto status = db->Get(leveldb::ReadOptions(), key, &outStr);

        if (status.ok())
        {
            auto rawData = reinterpret_cast<T::value_type*>(outStr.data());
            T container;
            container.assign(rawData, rawData + outStr.size());
            returnVal = container;
        }

        return returnVal;
    }
}
