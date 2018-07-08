#pragma once

#include <string>

//https://www.nanode.co/dashboard

namespace rail
{
    namespace blocks
    {
        class StateBlock;
        class Receive;
        class Open;
        class Change;
    }

    namespace endpoint
    {
        struct BlockCounts;
        struct AccountStatus;

        class IEndpoint
        {
        public:
            virtual ~IEndpoint() = default;

            virtual BlockCounts     getLedgerBlockCountSync() = 0;
            virtual AccountStatus   getAccountStatusSync(const std::string& account) = 0;
            virtual std::string     getFrontiersSync(const std::string& account) = 0;
            virtual bool            arePendingBlocksSync(const std::string& address) = 0;

            virtual void            getPendingBlocks(const std::string& account, const uint32_t count) = 0; 
            virtual void            getAccountHistory(const std::string& account, const uint32_t count) = 0;
            virtual void            getAccountBalance(const std::string& account) = 0;
            virtual void            startWebServer() = 0;

            virtual std::string sendStateBlock(blocks::StateBlock& sendBlock) = 0;
        };
    }
}
