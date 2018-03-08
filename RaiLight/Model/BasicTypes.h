#pragma once

#include <boost\multiprecision\cpp_int.hpp>

#include <stdint.h>

namespace rail
{
    using uInt128_t = boost::multiprecision::uint128_t;
    using uInt256_t = boost::multiprecision::uint256_t;
    using uInt512_t = boost::multiprecision::uint512_t;

    using ByteArray16 = std::array<std::byte, 16>;
    using ByteArray32 = std::array<std::byte, 32>;
    using ByteArray64 = std::array<std::byte, 64>;
}
