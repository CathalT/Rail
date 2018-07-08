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
            std::string frontier;
            std::string openBlock;
            std::string representativeBlock;
            std::string balance;
            std::string modifiedTimestamp;
            std::string representative;
            std::string weight;
            std::string pending;
        };
    }
}
