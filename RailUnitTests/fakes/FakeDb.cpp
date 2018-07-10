#include "RailUnitTests\fakes\FakeDb.h"

std::optional<std::vector<std::byte>> FakeDb::getVectorOfBytes(const char* /*key*/)
{
    return std::optional<std::vector<std::byte>>();
}

std::optional<rail::ByteArray16> FakeDb::getByteArray16(const char* /*key*/)
{
    return std::optional<rail::ByteArray16>();
}

std::optional<rail::ByteArray32> FakeDb::getByteArray32(const char* /*key*/)
{
    return std::optional<rail::ByteArray32>();
}

std::optional<uint32_t> FakeDb::get32UInt(const char* /*key*/)
{
    return std::optional<uint32_t>();
}

std::optional<bool> FakeDb::getBool(const char* /*key*/, const uint32_t /*index*/)
{
    return std::optional<bool>();
}

bool FakeDb::storeVectorOfBytes(const char* /*key*/, const std::vector<std::byte>& /*bytes*/, bool /*safeWrite*/)
{
    return true;
}

bool FakeDb::storeByteArray16(const char* /*key*/, const rail::ByteArray16& /*bytes*/, bool /*safeWrite*/)
{
    return true;
}

bool FakeDb::storeByteArray32(const char* /*key*/, const uint32_t /*index*/, const rail::ByteArray32& /*bytes*/, bool /*safeWrite*/)
{
    return true;
}

bool FakeDb::storeByteArray32(const char* /*key*/, const rail::ByteArray32& /*bytes*/, bool /*safeWrite*/)
{
    return true;
}

bool FakeDb::storeBool(const char* /*key*/, const uint32_t /*index*/, const bool /*result*/, bool /*safeWrite*/)
{
    return true;
}

bool FakeDb::store32UInt(const char* /*key*/, const uint32_t /*num*/)
{
    return true;
}

bool FakeDb::deleteValue(const char* /*key*/, const uint32_t /*index*/, const bool /*safeWrite*/)
{
    return true;
}

