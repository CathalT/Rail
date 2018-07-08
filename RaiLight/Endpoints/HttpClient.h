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
        class StateBlock;
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
            explicit RaiRCPClient(const std::string & _endpoint, const std::string & _listenPort, rail::control::ICore* _coreController);
            virtual ~RaiRCPClient();

            virtual BlockCounts getLedgerBlockCountSync() override;
            
            virtual AccountStatus getAccountStatusSync(const std::string & address) override;
            virtual std::string getFrontiersSync(const std::string& address) override;
            virtual bool arePendingBlocksSync(const std::string& address) override;
            virtual void getPendingBlocks(const std::string& address, const uint32_t count) override;
            virtual void getAccountHistory(const std::string& address, const uint32_t count) override;
            virtual void getAccountBalance(const std::string & address) override;
            virtual void startWebServer() override;

            virtual std::string sendStateBlock(rail::blocks::StateBlock& stateBlock) override;

        private:
            utility::string_t processBlockSync(const utility::string_t& block);

        private:
            std::unique_ptr<web::http::client::http_client> httpClient;
            std::unique_ptr<Webserver>                      webserver;

            rail::control::ICore*                           coreController;
        };
    }
}
