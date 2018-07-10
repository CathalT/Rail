#pragma once

#include "RaiLight\Controllers\ICore.h"

#include <string>
#include <memory>
#include <thread>

namespace rail
{
    class IDbWrapper;
    class IWorkLoop;
    class Marshaller;
    class SecretsStore;

    namespace endpoint
    {
        class IEndpoint;
    }

    namespace control
    {
        class IBank;
        class Core : public ICore
        {
        public:
            Core(const std::string& _nodeIp, const std::string& _listenPort, const std::string& seed);
            virtual ~Core();

            virtual IBank* getBank() override;
            virtual endpoint::IEndpoint* getEndpoint() override;
            virtual IWorkLoop* getWorkLoop() override;
            virtual Marshaller* getMarshaller() override;
            virtual IDbWrapper* getDatabase() override;
            virtual SecretsStore * getSecretsStore() override;


        private:
            std::unique_ptr<IWorkLoop>            workLoop;
            std::unique_ptr<IBank>                bank;
            std::unique_ptr<Marshaller>           marshaller;
            std::unique_ptr<endpoint::IEndpoint>  endpoint;
            std::unique_ptr<IDbWrapper>           database;
            std::unique_ptr<SecretsStore>         secretsStore;
        };
    }
}
