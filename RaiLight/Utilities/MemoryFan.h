#pragma once

#include "Model\BasicTypes.h"

namespace rail
{
    class MemoryFan
    {
    public:
        MemoryFan(ByteArray32 & key, const size_t count_a);
        //~MemoryFan();
        ByteArray32 getValue();

    private:
        std::vector< std::unique_ptr< ByteArray32 > > values;
    };
}

