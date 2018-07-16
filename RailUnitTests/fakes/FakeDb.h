#pragma once

#include "RaiLight\Database\IDbWrapper.h"
#include "RaiLight\Model\BasicTypes.h"

#include <optional>
#include <map>

class FakeDb : public rail::IDbWrapper
{
public:

    FakeDb();

    std::optional<std::vector<std::byte>> getVectorOfBytes(const char* key) override;
    std::optional<rail::ByteArray16> getByteArray16(const char* key) override;
    std::optional<rail::ByteArray32> getByteArray32(const char* key) override;
    std::optional<uint32_t> get32UInt(const char* key) override;
    std::optional<bool> getBool(const char* key, const uint32_t index) override;

    bool storeVectorOfBytes(const char* key, const std::vector<std::byte>& bytes, bool safeWrite) override;
    bool storeByteArray16(const char* key, const rail::ByteArray16& bytes, bool safeWrite) override;
    bool storeByteArray32(const char* key, const uint32_t index, const rail::ByteArray32& bytes, bool safeWrite) override;
    bool storeByteArray32(const char* key, const rail::ByteArray32& bytes, bool safeWrite) override;
    bool storeBool(const char* key, const uint32_t index, const bool result, bool safeWrite) override;
    bool store32UInt(const char* key, const uint32_t num) override;

    bool deleteValue(const char* key, const uint32_t index, const bool safeWrite) override;

public:
    std::map<std::string, bool> boolMap;
    std::map<std::string, uint32_t> uint32Map;
    std::map<std::string, rail::ByteArray16> byte16Map;
    std::map<std::string, rail::ByteArray32> byte32Map;
    std::map<std::string, std::vector<std::byte>> vectorByteMap;
};