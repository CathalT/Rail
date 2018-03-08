#pragma once

#include "Endpoints\IEndpoint.h"

#include "Model\BasicTypes.h"
#include <cpprest\details\basic_types.h>

#include <memory>

namespace web::http::client
{
    class http_client;
}

namespace rail
{
    class Marshaller;

    namespace blocks
    {
        class Send;
        class Receive;
        class Open;
        class Change;
    }

    namespace control
    {
        class ICore;
    }

    namespace endpoint
    {
        class Webserver;
        class RaiRCPClient : public endpoint::IEndpoint
        {
        public:
            explicit RaiRCPClient(const std::string & _endpoint, rail::control::ICore* _coreController);
            virtual ~RaiRCPClient();

            virtual BlockCounts getLedgerBlockCount() override;
            
            virtual AccountStatus getAccountStatus(const std::string & address) override;
            virtual std::string getFrontiers(const std::string& address) override;
            virtual bool arePendingBlocks(const std::string& address) override;
            virtual void getPendingBlocks(const std::string& address, const uint32_t count) override;
            virtual void getAccountHistory(const std::string& address, const uint32_t count) override;
            virtual void getAccountBalance(const std::string & address) override;
            virtual void startWebServer() override;

            virtual std::string send(const rail::blocks::Send& sendBlock) override;
            virtual std::string receive(const rail::blocks::Receive& receiveBlock) override;
            virtual std::string open(const rail::blocks::Open& openBlock) override;
            virtual std::string change(const rail::blocks::Change& changeBlock) override;

        private:
            utility::string_t processBlock(const utility::string_t& block);

        private:
            std::unique_ptr<web::http::client::http_client> httpClient;
            std::unique_ptr<Webserver>                      webserver;

            rail::control::ICore*                           coreController;
        };
    }
}
