#pragma once

#include "Conversions.h"

#include <boost/multiprecision/cpp_int.hpp>

namespace rail
{
    template<typename Num, typename ByteArray>
    Num Conversions::hexBytesToInt(const ByteArray& input)
    {
        Num outputInteger(0);
        auto shiftBy = 0;
        for (auto byte : input)
        {
            outputInteger <<= shiftBy;
            outputInteger |= byte;
            shiftBy = CHAR_BIT;
        }
        return outputInteger;
    }
}