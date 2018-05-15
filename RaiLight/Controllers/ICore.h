#pragma once

namespace rail
{
    class Marshaller;
    class WorkLoop;
    class RailDb;
    class SecretsStore;

    namespace endpoint
    {
        class IEndpoint;
    }

    namespace blocks
    {
        class IBlockStore;
    }

    namespace control
    {
        class IBank;

        class ICore
        {
        public:
            virtual endpoint::IEndpoint* getEndpoint() = 0;
            virtual IBank* getBank() = 0;
            virtual Marshaller* getMarshaller() = 0;
            virtual WorkLoop* getWorkLoop() = 0;
            virtual RailDb * getDatabase() = 0;
            virtual SecretsStore * getSecretsStore() = 0;

            virtual ~ICore() = default;
        };
    }
}
