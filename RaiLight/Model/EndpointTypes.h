#pragma once

#include <string>
#include <vector>

namespace rail
{
    namespace endpoint
    {
        struct BlockCounts
        {
            std::string count;
            std::string unchecked;
        };

        struct AccountStatus
        {
            bool     isValid{ false };
            bool     error{ false };
            uint64_t blockCount{ 0 };
        };
    }
}
