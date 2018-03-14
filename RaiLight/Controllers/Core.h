#pragma once

#include "Controllers\ICore.h"

#include <string>
#include <memory>
#include <thread>

namespace rail
{
    class RailDb;
    class WorkLoop;
    class Marshaller;

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
            Core(const std::string& _nodeIp);
            virtual ~Core();

            virtual IBank* getBank() override;
            virtual endpoint::IEndpoint* getEndpoint() override;
            virtual WorkLoop* getWorkLoop() override;
            virtual Marshaller* getMarshaller() override;
            virtual RailDb * getDatabase() override;
           
        private:
            std::unique_ptr<WorkLoop>             workLoop;
            std::unique_ptr<IBank>                bank;
            std::unique_ptr<Marshaller>           marshaller;
            std::unique_ptr<endpoint::IEndpoint>  endpoint;
            std::unique_ptr<RailDb>               database;
        };
    }
}
