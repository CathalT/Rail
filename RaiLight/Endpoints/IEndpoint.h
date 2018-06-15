#pragma once

#include <string>

//https://www.nanode.co/dashboard

namespace rail
{
    namespace blocks
    {
        class Send;
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

            virtual std::string send(blocks::Send& sendBlock) = 0;
            virtual std::string receive(blocks::Receive& recieveBlock) = 0;
            virtual std::string open(blocks::Open& openBlock) = 0;
            virtual std::string change(blocks::Change& changeBlock) = 0;
        };
    }
}
