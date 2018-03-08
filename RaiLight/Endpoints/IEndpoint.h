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

            virtual BlockCounts     getLedgerBlockCount() = 0;
            virtual AccountStatus   getAccountStatus(const std::string& account) = 0;
            virtual std::string     getFrontiers(const std::string& account) = 0;
            virtual bool            arePendingBlocks(const std::string& address) = 0;

            virtual void            getPendingBlocks(const std::string& account, const uint32_t count) = 0; 
            virtual void            getAccountHistory(const std::string& account, const uint32_t count) = 0;
            virtual void            getAccountBalance(const std::string& account) = 0;
            virtual void            startWebServer() = 0;

            virtual std::string send(const blocks::Send& sendBlock) = 0;
            virtual std::string receive(const blocks::Receive& recieveBlock) = 0;
            virtual std::string open(const blocks::Open& openBlock) = 0;
            virtual std::string change(const blocks::Change& changeBlock) = 0;

            //utility::string_t processBlock(const ByteArray32& privateKey, const ByteArray32& publicKey, const std::string& accoundId);
            //utility::string_t getPendingBlock();
        };
    }
}
